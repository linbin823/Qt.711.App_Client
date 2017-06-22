#ifndef ANDROIDFUNCTION
#define ANDROIDFUNCTION
//----消息提示------
#include <QObject>
//The QtAndroid namespace provides miscellaneous functions to aid Android development.
#include <QtAndroid>
//jini implements an interface between java and c/c++
#include <jni.h>
//Provides APIs to call Java code from C++.
#include <QtAndroidExtras/QAndroidJniObject>

#define APP_STORE_ADDRESS "http://www.wandoujia.com/apps/com.bxlts.example"

class AndroidFunction :public QObject{
    Q_OBJECT
public slots:
    static void NotificationMessage(QString);
    static void ApplicationMessages(QString);
    //应用分享
    void ApplicationShare();
    //新浪微博授权登录
    void SinaWeiboLogin();
    QString GetAccessToken();
    QString GetAccessUid();
    int GetActivityStatusHeight();
    void play();
    static QString GetPackageVersionName();
    static QString GetStoragePath();
    void NetworkWIFISettings();//系统设置
    void NetworkSettings();//WIFI设置
    static void StartPackageInstallation(QString);
    static void startMapActivity();
    void SaveProjectInfo(QString name,QString city,QString address,QString start_time,QString end_time,QString schedule,QString event,QString manager);
    void ClearSaveProjectInfo();
private:
    QAndroidJniObject JniObject;
};

#endif // ANDROIDFUNCTION

