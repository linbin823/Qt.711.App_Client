#ifndef APPSETTINGS_H
#define APPSETTINGS_H

//fufill app's framwork-level functions.

#include <QObject>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "iloadsaveprocessor.h"

#define MAJOR_VER   0
#define MAIN_VER    1
#define MINOR_VER   0
#define DEV_VER     17040701


#ifdef  Q_OS_ANDROID
#define APP_FILE_NAME        "dataCenter.apk"
#endif
#ifdef   Q_OS_IOS
//change it!
#define APP_FILE_NAME        "dataCenter.apk"
//end of change it!
#endif
#ifdef   Q_OS_OSX
#define APP_FILE_NAME        "dataCenter"
#endif
#ifdef   Q_OS_LINUX

#ifndef  Q_OS_ANDROID
//change it!
#define APP_FILE_NAME        "dataCenter"
//end of change it!
#endif
#endif

#ifdef   Q_OS_WINDOWS
#define APP_FILE_NAME        "dataCenter.exe"
#endif

#define CONFIG_FILE_NAME    "dataCenter.xml"
#define SERVER_HOST         "app.csic711.net"
#define SERVER_PATH         "/"
#define LOCAL_STORAGE_PATH   "./settings/"

class AppSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hasNewVersion READ hasNewVersion  NOTIFY hasNewVersionChanged)
    Q_PROPERTY(int updateProgress READ updateProgress NOTIFY updateProgressChanged)
    Q_PROPERTY(QString errMsg READ errMsg NOTIFY errMsgChanged)
    Q_PROPERTY(bool accsessConfig READ accsessConfig  NOTIFY accsessConfigChanged)
    Q_PROPERTY(bool accsessApp    READ accsessApp     NOTIFY accsessAppChanged)
public:
    explicit AppSettings(QObject *parent = 0);

public slots:
    int  updateProgress(){ return _updateProgress; }
    bool hasNewVersion() { return _hasNewVersion; }
    bool accsessConfig() { return _accsessConfig; }
    bool accsessApp() { return _accsessApp; }
    QString errMsg(){ return _errMsg; }
    QString localVersion(){ return _localVersion; }
    QString serverVersion(){ return _serverVersion; }
    int sizeTotalKB(){ return _serverTotalSizeKBYTES; }
    int sizeFinishedKB(){ return _serverFinishedSizeKBYTES; }

    void checkNewVersion();
    void startDownload();
    void abortDownload();

    void generateConfigFile();

signals:
    void hasNewVersionChanged();
    void accsessConfigChanged();
    void accsessAppChanged();
    void updateProgressChanged();
    void errMsgChanged();

private:
    QString _localVersion, _serverVersion;
    QString _errMsg;
    QFile *_pFile;
    iLoadSaveProcessor* _pXmlProcessor;
    QUrl _serverConfigFileUrl,_serverAppUrl;
    QNetworkAccessManager *_pAccessManager;
    QNetworkRequest _request;
    QNetworkReply *_pReply;
    bool _initOK;
    bool _hasNewVersion;
    bool _accsessConfig;
    bool _accsessApp;

    int _updateProgress;
    qint64 _serverTotalSizeKBYTES,_serverFinishedSizeKBYTES;

private slots:
    void _replyFinished(QNetworkReply*reply);
    void _onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void _versionCheck();
};

#endif // APPSETTINGS_H
