#include <QUrl>
#include <QGuiApplication>
#include <QStyleHints>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>
#include <QtCookieSyn>

#include "usrmanager.h"
#include "appsettings.h"
#include "rtdata.h"
#include "rtdatahistory.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtWebView::initialize();
    QGuiApplication::setApplicationDisplayName(QObject::tr("QYY DATA CENTER"));

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    //////////////////////////////////////////////// to set cookies syn module.
    START_COOKIE_SYN(engine,12345);
    ////////////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////// to set the initial windows size for desktop application.
    QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();
    if (!QGuiApplication::styleHints()->showIsFullScreen()) {
        const QSize size = geometry.size() * 4 / 5;
        const QSize offset = (geometry.size() - size) / 2;
        const QPoint pos = geometry.topLeft() + QPoint(offset.width(), offset.height());
        geometry = QRect(pos, size);
    }
    context->setContextProperty(QStringLiteral("initialX"), geometry.x());
    context->setContextProperty(QStringLiteral("initialY"), geometry.y());
    context->setContextProperty(QStringLiteral("initialWidth"), geometry.width());
    context->setContextProperty(QStringLiteral("initialHeight"), geometry.height());
    ////////////////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////// to set the settings.
    AppSettings* setting = new AppSettings();
    UsrManager* usrMgr = UsrManager::Instance();
    ////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////// to set the data.
    RTData* data = RTData::Instance();
//    RTDataHistory * historyData = RTDataHistory::Instance();

    data->dataInit();
    context->setContextProperty("dataSource", data);
    context->setContextProperty("usrManager", usrMgr);
//    context->setContextProperty("dataHistory", historyData);
    context->setContextProperty("settings", setting);
    ////////////////////////////////////////////////////////////////////////////////////


    engine.load(QUrl(QStringLiteral("qrc:/qml/dataCenter.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();

}
