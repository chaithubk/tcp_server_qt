#include <QGuiApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include <QFont>
#include <QFontDatabase>
#include <QScreen>

#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Set the application name, which is displayed in the usage message.
    QGuiApplication::setApplicationName("TCP-Server");

    qmlRegisterType<MyTcpServer>("Tcp", 1, 0, "MyTcpServer");

    MyTcpServer *tcpServer = new MyTcpServer();

    QQmlContext *context = engine.rootContext();
    context->setContextProperty("TCPServer", tcpServer);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
