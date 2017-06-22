#include "androidfunction.h"

void AndroidFunction::ApplicationMessages(QString text){
    QAndroidJniObject activity = QtAndroid::androidActivity();//return main activity in C++ object
    QAndroidJniObject jtext = QAndroidJniObject::fromString(text);//creat a java string
    activity.callMethod<void>("ShowMakeText",
                                              "(Ljava/lang/String;)V",
                                               jtext.object<jstring>());
}

void AndroidFunction::NotificationMessage(QString text){
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject jtext = QAndroidJniObject::fromString(text);
    activity.callMethod<void>("ShowNotification",
                              "(Ljava/lang/String;)V",
                              jtext.object<jstring>());
}

void AndroidFunction::ApplicationShare(){
    QAndroidJniObject jTitle = QAndroidJniObject::fromString("分享到");
    QAndroidJniObject jSubject = QAndroidJniObject::fromString("应用分享");
    QAndroidJniObject jContent = QAndroidJniObject::fromString("我正在使用宝星项目信息体验手机端应用,应用地址:" APP_STORE_ADDRESS "\n (来自信息中心安卓客户端)");
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>("com/bxlts/example/WBAuthorizedLogin",
                                "shareText",
                                "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;"
                                        "Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
                                jTitle.object<jstring>(),
                                jSubject.object<jstring>(),
                                jContent.object<jstring>(),
                                activity.object<jobject>());
}

QString AndroidFunction::GetAccessToken(){
    QAndroidJniObject  string=JniObject.callObjectMethod<jstring>("GetAccessToken");
    return string.toString();
}

int AndroidFunction::GetActivityStatusHeight(){
    jint j_int= QAndroidJniObject::callStaticMethod<jint>("com/bxlts/example/MActivityClass", "getStatusHeight");
    return j_int;
}

void AndroidFunction::play(){
    QAndroidJniObject::callStaticMethod<void>("com/bxlts/example/MActivityClass","playClickSound");
}

QString AndroidFunction::GetAccessUid(){
    QAndroidJniObject  string=JniObject.callObjectMethod<jstring>("GetAccessUid");
    return string.toString();
}

QString AndroidFunction::GetPackageVersionName(){
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject  string = QAndroidJniObject::callStaticObjectMethod("com/bxlts/example/MActivityClass",
                                                                                 "getAppVersionName",
                                                                                 "(Lorg/qtproject/qt5/android/bindings/QtActivity;)Ljava/lang/String;",
                                                                                 activity.object<jobject>());
    return string.toString();
}

QString AndroidFunction::GetStoragePath(){
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject  string = QAndroidJniObject::callStaticObjectMethod("com/bxlts/example/MActivityClass",
                                                                                 "getStoragePath",
                                                                                 "(Lorg/qtproject/qt5/android/bindings/QtActivity;)Ljava/lang/String;",
                                                                                 activity.object<jobject>());
    return string.toString();
}

void AndroidFunction::SinaWeiboLogin(){
    QAndroidJniObject activity = QtAndroid::androidActivity();//返回这个应用程序的主要活动
    QAndroidJniObject object("com/bxlts/example/WBAuthorizedLogin");
    JniObject = object;
            object.callMethod<void>("CreateWBAuthActivity",                             //成员函数
            "(Lorg/qtproject/qt5/android/bindings/QtActivity;)V",                       //参数类型:QtActivity , V:没有返回值
            activity.object<jobject>());                                                                      //参数
}

void AndroidFunction::NetworkWIFISettings(){
    QAndroidJniObject action = QAndroidJniObject::fromString("android.settings.WIFI_SETTINGS");
    QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", action.object<jstring>());
    QtAndroid::startActivity(intent,0);
}

void AndroidFunction::NetworkSettings(){
    QAndroidJniObject action = QAndroidJniObject::fromString("android.settings.SETTINGS");
    QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;)V", action.object<jstring>());
    QtAndroid::startActivity(intent,0);
}

void AndroidFunction::startMapActivity(){
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>("com/bxlts/example/MActivityClass",
                                        "mStartActivity",
                                        "(Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
                                        activity.object<jobject>());
//    QAndroidJniObject main_activity("com/bxlts/example/MainActivity");
//    QAndroidJniObject intent("android/content/Intent","(Lorg/qtproject/qt5/android/bindings/QtActivity;Lcom/bxlts/example/MainActivity;)V",
//                                            activity.object<jobject>(),main_activity.object<jobject>());
//    QtAndroid::startActivity(intent, 0);
}

void AndroidFunction::StartPackageInstallation(QString fileName){
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject file = QAndroidJniObject::fromString(fileName);
    QAndroidJniObject::callStaticMethod<void>("com/bxlts/example/MActivityClass",
                                "packageInstallation",
                                "(Lorg/qtproject/qt5/android/bindings/QtActivity;Ljava/lang/String;)V",
                                activity.object<jobject>(),
                                file.object<jstring>());
}

void AndroidFunction::SaveProjectInfo(QString name,QString city,QString address,QString start_time,QString end_time,QString schedule,QString event,QString manager){
    QAndroidJniObject _name = QAndroidJniObject::fromString(name);
    QAndroidJniObject _city = QAndroidJniObject::fromString(city);
    QAndroidJniObject _address = QAndroidJniObject::fromString(address);
    QAndroidJniObject _start_time = QAndroidJniObject::fromString(start_time);
    QAndroidJniObject _end_time = QAndroidJniObject::fromString(end_time);
    QAndroidJniObject _schedule = QAndroidJniObject::fromString(schedule);
    QAndroidJniObject _event = QAndroidJniObject::fromString(event);
    QAndroidJniObject _manager = QAndroidJniObject::fromString(manager);
    QAndroidJniObject::callStaticMethod<void>("com/bxlts/example/MActivityMap",
                                "SaveProjectInfo",
                                "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                                _name.object<jstring>(),_city.object<jstring>(), _address.object<jstring>(),_start_time.object<jstring>(),_end_time.object<jstring>(),
                                _schedule.object<jstring>(),_event.object<jstring>(),_manager.object<jstring>());
}

void AndroidFunction::ClearSaveProjectInfo(){
    QAndroidJniObject::callStaticMethod<void>("com/bxlts/example/MActivityMap","ClearProjectInfo");
}
