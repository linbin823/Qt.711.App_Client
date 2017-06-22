#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T14:44:07
#
#-------------------------------------------------

QT       += core gui qml quick network xml sql quickcontrols2 webview

TARGET = DataCenter_app
TEMPLATE = app

HEADERS += \
    gSOAPGEN/soaprdLiteProxy.h \
    gSOAPGEN/rdLite.nsmap \
    gSOAPGEN/stdsoap2.h \
    taginfo.h \
    appsettings.h \
    rtdata.h \
    rtdriverwebservice.h \
    taghistory.h \
    rtdatahistory.h \
    multicastprotocoldriver.h

SOURCES += \
    main.cpp\
    gSOAPGEN/soapC.cpp \
    gSOAPGEN/soaprdLiteProxy.cpp \
    gSOAPGEN/stdsoap2.cpp \
    taginfo.cpp \
    appsettings.cpp \
    rtdata.cpp \
    rtdriverwebservice.cpp \
    taghistory.cpp \
    rtdatahistory.cpp \
    multicastprotocoldriver.cpp

CONFIG += mobility

RESOURCES += \
    res.qrc

CONFIG += c++11

#to serial/deserial objects in xml or jason fomat
#include(../lib/loadSaveProcessor/src/loadsaveProcessor.pri)
#to use cookies in the qml
include(../lib/QtCookiesSyn/src/qtcookiesyn.pri)
#to use usr management
include(../lib/usrManager/src/usrmanager.pri)


win32 {
    contains(QT_ARCH, i386) {
            message("32-bit windows")
        }else {
            message("64-bit windows")
    }
}

unix {
    linux*{

    }
}

macx {

}

android*{
    message("android")

    QT +=  androidextras

    HEADERS += \
        androidfunction.h
    SOURCES += \
        androidfunction.cpp

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    deployment.path = /assets/
    deployment.files += ConfigDB.db
    INSTALLS += deployment

#    INCLUDEPATH += /usr/include
    DISTFILES += \
        android/AndroidManifest.xml \
        android/assets/ConfigDB.db

}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../openssl/libcrypto.so \
        $$PWD/../../../openssl/libcrypto.so
}

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/com/smderi/datacenter/MessageReceiver.java \
    android/src/com/smderi/datacenter/DatacenterApplication.java \
    android/src/com/smderi/datacenter/DatacenterActivity.java
