#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QTimer>

class MyTcpServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serverStatus READ serverStatus NOTIFY serverStatusChanged)
    Q_PROPERTY(QString writeStatus READ writeStatus NOTIFY writeStatusChanged)
    Q_PROPERTY(QString connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(QString localIP READ localIP NOTIFY localIPChanged)

public:
    MyTcpServer(QObject *parent = 0);
    ~MyTcpServer();

    enum vsId_t
    {
        VS_ID_HR,					//  0 - Heart Rate
        VS_ID_TEMP,                 //  1 - Temperature
        VS_ID_LAST
    } vsId;

    QString connectionStatus() { return m_connectionStatus; }
    QString writeStatus() { return m_writeStatus; }
    QString serverStatus() { return m_serverStatus; }
    QString localIP() { return m_localIP; }

    void writeToSocket();
    void setConnectionStatus(QString status);
    void setWriteStatus(QString status);
    void setServerStatus(QString status);
    
signals:
    void serverStatusChanged();
    void connectionStatusChanged();
    void writeStatusChanged();
    void localIPChanged();
    
public slots:
    void newConnection();
    void delayTimeoutHanlder();

private:
    static QString simData[VS_ID_LAST];

    QTcpServer *m_server;
    QTcpSocket *m_socket;
    QTimer  m_delayTimer;
    QString m_vsData[VS_ID_LAST];
    QString m_serverStatus;
    QString m_writeStatus;
    QString m_connectionStatus;
    QString m_localIP;
};

#endif // MYTCPSERVER_H
