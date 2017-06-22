#include <QDir>
#include <QIODevice>
#include <QFileInfo>
#include "loadsaveprocessorxml.h"
#include "appsettings.h"

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    _initOK = false;
    _serverTotalSizeKBYTES = 0;
    _serverFinishedSizeKBYTES =0;
    _accsessConfig = false;
    emit accsessConfigChanged();
    _accsessApp = false;
    emit accsessAppChanged();
    _hasNewVersion = false;
    emit hasNewVersionChanged();
    _updateProgress = 0;
    emit updateProgressChanged();


    //init error message
    _errMsg = tr("");
    emit errMsgChanged();

    //instantiate
    _pFile = new QFile(this);

    _pAccessManager = new QNetworkAccessManager(this);
    connect(_pAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(_replyFinished(QNetworkReply*)));

    //generate local version
    _localVersion = QString::number(MAJOR_VER) +
            "." + QString::number(MAIN_VER) +
            "." + QString::number(MINOR_VER) +
            "." + QString::number(DEV_VER);

    //generate a local config file
    generateConfigFile();

    //generate a server config file url
    _serverConfigFileUrl.setScheme("http");
    _serverConfigFileUrl.setHost(QString(SERVER_HOST));
    _serverConfigFileUrl.setPath(QString(SERVER_PATH) + QString(CONFIG_FILE_NAME));
    if(!_serverConfigFileUrl.isValid()){
        _errMsg = tr("invalid server config file url");
        emit errMsgChanged();
        return;
    }

    //generate a server application url
    _serverAppUrl.setScheme("http");
    _serverAppUrl.setHost(QString(SERVER_HOST));
    _serverAppUrl.setPath(QString(SERVER_PATH) + QString(APP_FILE_NAME));
    if(!_serverAppUrl.isValid()){
        _errMsg = tr("invalid server application url");
        emit errMsgChanged();
        return;
    }

    //initial successful
    _initOK = true;
}

void AppSettings::checkNewVersion(){
    if(!_initOK){
        _errMsg += tr("/ncan't check new version!");
        emit errMsgChanged();
        return;
    }
    if(_accsessApp || _accsessConfig){
        //file downloading, exclusive
        return;
    }
    //start access
    _request.setUrl(_serverConfigFileUrl);
    _pReply=_pAccessManager->get(_request);//通过_pAccessManager发送数据，返回值保存在_pReply指针里.
    connect(_pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( _onDownloadProgress(qint64 ,qint64 )));

    _accsessConfig = true;
    emit accsessConfigChanged();
}
void AppSettings::startDownload(){
    if(!_initOK){
        _errMsg += tr("/ncan't check new version!");
        emit errMsgChanged();
        return;
    }
    if(_accsessApp || _accsessConfig){
        //file downloading, exclusive
        return;
    }
    //start access
    _request.setUrl(_serverAppUrl);
    _pReply=_pAccessManager->get(_request);//通过_pAccessManager发送数据，返回值保存在_pReply指针里.
    connect(_pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( _onDownloadProgress(qint64 ,qint64 )));

    _accsessApp = true;
    emit accsessAppChanged();
}
void AppSettings::abortDownload(){
    if(_accsessApp){
        _pReply->abort();//to avoid memeory leakage
        _pReply->deleteLater();//要删除reply，但是不能直接delete，要调用deletelater;
        _accsessApp = false;
        emit accsessAppChanged();
    }
}

void AppSettings::generateConfigFile(){
    //for grabing new version app's xml config file.
    QDir localStorage;
    bool ret;
    localStorage.setPath(QString(LOCAL_STORAGE_PATH));
    if(!localStorage.exists()){
        ret = localStorage.mkpath(QString(LOCAL_STORAGE_PATH));
        if(!ret){
            _errMsg = tr("invalid local storage path");
            emit errMsgChanged();
            return;
        }
    }
    _pXmlProcessor = new loadSaveProcessorXml(this,false);
    _pXmlProcessor->init();
    _pXmlProcessor->writeValue("version",_localVersion);
    _pXmlProcessor->saveFile(QString(LOCAL_STORAGE_PATH) + QString(CONFIG_FILE_NAME));
    delete _pXmlProcessor;
}
void AppSettings::_replyFinished(QNetworkReply* reply){
    //qDebug()<<"AppSettings::_replyFinished"<<reply->error()<<reply->errorString();
    if(reply->error() == QNetworkReply::NoError){
//receive success
        QByteArray bytes = reply->readAll();
        //qDebug()<<bytes.size()<<bytes;

        if(_accsessConfig){
            //xml analyise
            _pXmlProcessor = new loadSaveProcessorXml(this,false);
            _pXmlProcessor->loadByteArray(bytes);
            _pXmlProcessor->readValue("version",_serverVersion);
            delete _pXmlProcessor;
            _versionCheck();
        }
        else if(_accsessApp){
            if(_pFile->isOpen()){
                _pFile->close();
            }
            _pFile->setFileName(QString(LOCAL_STORAGE_PATH) + QString(APP_FILE_NAME));
            if( _pFile->open(QIODevice::WriteOnly) ){
//file write success
                _pFile->write(bytes);
                _pFile->close();
            }
            else{
//file open fail
                errMsg() = tr("app file write failure");
                emit errMsgChanged();
            }
        }
    }
    else{
//receive error
        _errMsg = reply->errorString();
        emit errMsgChanged();
    }

//clean enviroment
    if(_accsessConfig){
        _accsessConfig = false;
        emit accsessConfigChanged();
    }
    else if(_accsessApp){
        _accsessApp = false;
        emit accsessAppChanged();
    }
    reply->abort();//to avoid memeory leakage
    reply->deleteLater();//要删除reply，但是不能直接delete，要调用deletelater;
}

void AppSettings::_onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal){
    if(_accsessApp){
        _serverTotalSizeKBYTES = bytesTotal;
        _serverFinishedSizeKBYTES =bytesReceived;
        _updateProgress = bytesReceived / bytesTotal;
        updateProgressChanged();
    }
}

void AppSettings::_versionCheck(){
    QStringList versions = _serverVersion.split(".");
    if(versions.size() != 4){
        return;
    }
    if(versions.at(0).toInt() > MAJOR_VER){
        _hasNewVersion = true;
        hasNewVersionChanged();
        return;
    }
    if(versions.at(1).toInt() > MAIN_VER){
        _hasNewVersion = true;
        hasNewVersionChanged();
        return;
    }
    if(versions.at(2).toInt() > MINOR_VER){
        _hasNewVersion = true;
        hasNewVersionChanged();
        return;
    }
    if(versions.at(3).toInt() > DEV_VER){
        _hasNewVersion = true;
        hasNewVersionChanged();
        return;
    }
}
