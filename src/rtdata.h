#ifndef RTDATA_H
#define RTDATA_H

#include <QObject>
#include <QSqlDatabase>
#include "taginfo.h"
#include "multicastprotocoldriver.h"


class RTData : public QObject
{
    Q_OBJECT
    //系统名称列表，QML用于筛选子系统
    Q_PROPERTY(QStringList sysNameList  READ sysNameList    NOTIFY sysNameListChanged)
    //测点名称列表，QML用于绑定测点
    Q_PROPERTY(QStringList tagNameList  READ tagNameList    NOTIFY tagNameListChanged)
    //所有测点，QML的数据源头
    Q_PROPERTY(QList<QObject*> tagList  READ tagList        NOTIFY tagListChanged)
    //与webservice的连接状态，QML显示状态
    Q_PROPERTY(int state    READ connectState   NOTIFY stateChanged)

public:
    //singleton
    static RTData* Instance();
    //初始化点表
    void dataInit();
    QStringList sysNameList()const{
        return _sysNameList;
    }
    int connectState(){
        return _pDriver->getState();
    }
    QList<QObject*> tagList(){
        QList<QObject*> tagList;
        foreach (TagInfo* it, _proxyData) {
            tagList << it;
        }
        return tagList;
    }
    QStringList tagNameList(){
        QStringList tagNameList;
        foreach (TagInfo* it, _proxyData) {
            tagNameList << it->tagName();
        }
        return tagNameList;
    }

public slots:
    //QML筛选功能
    void filter(const QString& projectName = QString::null,
                           const QString & stationName = QString::null,
                           const QString & systemName = QString::null);
    //QML获取单个测点对象
    QObject* getValue(int tagID);
    QObject* getValue(const QString& tagName){
        return getValue( getTagID(tagName) );
    }
    //QML使用，实现测点名称与测点ID互查
    QString getTagName(int tagID);
    int getTagID(const QString& tagName);

    //QML刷新测点数据
    void readOnce(const QList<int> &tagIDs);
    void readOnce(int tagID);

private:
    explicit RTData(QObject *parent = 0);
    static RTData* _singleton;
    MulticastProtocolDriver * _pDriver;

    //所有测点集合
    QList<TagInfo*> _data;
    //筛选后子系统测点集
    QList<TagInfo*> _proxyData;
    QStringList _sysNameList;
    QSqlDatabase* _pDb;

    void refreshSysNameList();
    static bool compareTagInfo(const TagInfo* t1, const TagInfo* t2);

signals:
    void sysNameListChanged();
    void stateChanged();
    void tagListChanged();
    void tagNameListChanged();
};

#endif // RTDATA_H
