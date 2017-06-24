#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtCookieSyn>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>
#include "appsettings.h"
#include <QSslSocket>
#include <QCryptographicHash>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtWebView::initialize();

    QQmlApplicationEngine engine;

    START_COOKIE_SYN(engine);

    QQmlContext *context = engine.rootContext();
    AppSettings* setting = new AppSettings();
    context->setContextProperty("settings", setting);
    qDebug()<<"here"<<QSslSocket::supportsSsl()<<QSslSocket::sslLibraryVersionString();
    engine.load(QUrl(QStringLiteral("qrc:/qml_local/main.qml")));
    return app.exec();
}
