#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtCookieSyn>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>
#include "appsettings.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtWebView::initialize();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml_local/main.qml")));

    START_COOKIE_SYN(engine,55555);

    QQmlContext *context = engine.rootContext();
    AppSettings* setting = new AppSettings();
    context->setContextProperty("settings", setting);

    return app.exec();
}
