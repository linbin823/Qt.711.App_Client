#include <QThread>
#include <QTimer>
#include <QQmlEngine>
#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include "rtdata.h"

RTData* RTData::_singleton = nullptr;

RTData* RTData::Instance(){
    if(_singleton == nullptr){
        _singleton = new RTData();
    }
    return _singleton;
}

RTData::RTData(QObject *parent) : QObject(parent)
{
    _pDriver = new MulticastProtocolDriver();
    connect(_pDriver,SIGNAL(stateChanged(int)),this,SIGNAL(stateChanged()));
    QThread* workThread = new QThread();
    _pDriver->moveToThread(workThread);
    connect(workThread,SIGNAL(started()),_pDriver,SLOT(start()));
    workThread->start();
    //qDebug()<<"RTData::RTData"<<workThread->isRunning();

    int iConnIdx = 0;
    QString _strConnName;
    while(1)
    {
        _strConnName = QString("MyDBProcessConn%1").arg(++iConnIdx);
        QSqlDatabase dbConn = QSqlDatabase::database(_strConnName, false);
        //to avoid connName confliction
        if(dbConn.isValid()){//存在连接
            continue;
        }
        _pDb = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", _strConnName));
        break;
    }
}

void RTData::dataInit(){

    foreach (TagInfo* it, _data) {
        delete it;
    }
    _data.clear();

#ifdef Q_OS_ANDROID
    QFile dbFile("assets:/ConfigDB.db");
    if( !dbFile.exists() ){
        qDebug()<<"cant find db file";
        return;
    }
    dbFile.copy("./ConfigDB.db");
    QFile::setPermissions("./ConfigDB.db",QFile::WriteOwner | QFile::ReadOwner);
#endif

    _pDb->setDatabaseName("./ConfigDB.db");
    if (!_pDb->open()){
        qDebug()<<"cant open db file";
        _pDb->close();
        return;
    }
    qDebug()<<"RTWebServiceDriver::refreshTagsInfo"<<"pdb is opened!";
    QSqlQuery* _pquery = new QSqlQuery(*_pDb);

    _pquery->exec("select TAG_ID,TESTPOINTNAME,UNIT,RANKCODE,PRJID,STATIONID,MODULEID,POINTID from TESTPOIT_CONFIG");
    while(_pquery->next()){
        if( !_pquery->isValid() )   continue;

        //ID从project、station、module、point中计算得到
        int tagIDAuto,projectID,stationID,moduleID,pointID;
        bool ok;
        projectID = _pquery->value(4).toInt(&ok);
        if(!ok) continue;
        stationID = _pquery->value(5).toInt(&ok);
        if(!ok) continue;
        moduleID = _pquery->value(6).toInt(&ok);
        if(!ok) continue;
        pointID = _pquery->value(7).toInt(&ok);
        if(!ok) continue;
        tagIDAuto = (projectID << 16) + (stationID << 9) + (moduleID << 5) + pointID;

        TagInfo* tag = new TagInfo(tagIDAuto);
        tag->setTagID( _pquery->value(0).toInt() );
        tag->setTagName( _pquery->value(1).toString() );
        tag->setTagUnit( _pquery->value(2).toString() );
        tag->setTagRankCode( _pquery->value(3).toInt());
        tag->setProjectName( QString("净港一号") );
        tag->setProjectID( projectID );
        tag->setStationID( stationID );
        tag->setModuleID( moduleID );
        tag->setPointID( pointID );

        connect(tag,SIGNAL(tagRWStrategyChanged(TagInfo*)),_pDriver, SLOT(tagRWSChanged(TagInfo*)));
        _data.append(tag);
    }

    _pquery->clear();
    _pquery->exec("select TAG_ID, PAGENM from CONSOLE_PAGE");
    while(_pquery->next()){
        int ID = _pquery->value(0).toInt();
        QString sysName = _pquery->value(1).toString();
        for(int j=0; j<_data.size(); j++){
            TagInfo* currentItem = _data.at(j);
            if(ID == currentItem->tagID() ){
                //qDebug()<<ID;
                currentItem->setSystemName( sysName );
            }
        }
    }

    //sort by tagID
    qSort(_data.begin(),_data.end(), compareTagInfo);

    refreshSysNameList();

    _pDriver->setAllTags(&_data);

    delete _pquery;
    _pDb->close();
}

bool RTData::compareTagInfo(const TagInfo* t1, const TagInfo* t2){
    return t1->tagIDAuto() < t2->tagIDAuto();
}

void RTData::refreshSysNameList(){
    _sysNameList.clear();
    _sysNameList.append("全部数据");
    QString sysName;
    for(int i=0; i<_data.size(); i++){
        TagInfo* currentItem = _data.at(i);
        sysName = currentItem->systemName();
        if(!_sysNameList.contains(sysName)){
            _sysNameList.append(sysName);
        }
    }
    emit sysNameListChanged();
}

void RTData::filter(const QString& projectName,
                       const QString& stationName,
                       const QString& systemName){
//    qDebug()<<"filter"<<projectName<<stationName<<systemName;
    _proxyData.clear();
    for(int i=0; i<_data.size();i++){
        TagInfo* currentItem = _data.at(i);
        if( (!projectName.isNull()) && !projectName.isEmpty() ){
            if( currentItem->projectName() != projectName && QString("全部数据") != projectName)
                continue;
        }
        if( (!stationName.isNull()) && !stationName.isEmpty() ){
            if( currentItem->stationName() != stationName && QString("全部数据") != stationName)
                continue;
        }
        if( (!systemName.isNull()) && !systemName.isEmpty() ){
            if( currentItem->systemName() != systemName && QString("全部数据") != systemName)
                continue;
        }
        _proxyData.append(currentItem);
    }
    emit tagListChanged();
    emit tagNameListChanged();
}

QObject *RTData::getValue(int tagID){
    for(int i=0; i<_data.size();i++){
        TagInfo* currentItem = _data.at(i);
        if(currentItem->tagID() == tagID){
            QQmlEngine::setObjectOwnership(_data.at(i),QQmlEngine::CppOwnership);
            return _data.at(i);
        }
    }
    return new TagInfo(tagID);
}

QString RTData::getTagName(int tagID){
    foreach (TagInfo* it, _proxyData) {
        if(it->tagID() == tagID){
            return it->tagName();
        }
    }
    return QString::null;
}

int RTData::getTagID(const QString& tagName){
    foreach (TagInfo* it, _proxyData) {
        if(it->tagName() == tagName){
            return it->tagID();
        }
    }
    return -1;
}

void RTData::readOnce(const QList<int>& tagIDs){
    foreach (int id, tagIDs) {
        foreach (TagInfo* tag, _data) {
            if(tag->tagID() == id){
                tag->setTagRWStrategy( TagInfo::RWS_READ_ONCE );
                break;
            }
        }
    }
}

void RTData::readOnce(int tagID){
    QList<int> ids;
    ids<<tagID;
    readOnce(ids);
}
