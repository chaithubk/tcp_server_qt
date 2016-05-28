#include "mytcpserver.h"
#include <QTime>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>

QString MyTcpServer::simData[VS_ID_LAST] =
{
    "60",       //  0 - Heart Rate
    "27.2",		//  1 - Temperature
};

void MyTcpServer::setConnectionStatus(QString status)
{
    m_connectionStatus = status;
    qDebug() << m_connectionStatus;
    emit connectionStatusChanged();
}

void MyTcpServer::setWriteStatus(QString status)
{
    m_writeStatus = status;
    qDebug() << m_writeStatus;
    emit writeStatusChanged();
}

void MyTcpServer::setServerStatus(QString status)
{
    m_serverStatus = status;
    qDebug() << m_serverStatus;
    emit serverStatusChanged();
}


MyTcpServer::MyTcpServer(QObject *parent) :
    QObject(parent)
  , m_server()
  , m_socket(NULL)
  , m_delayTimer()
{
    connect(&m_delayTimer, SIGNAL(timeout()), this, SLOT(delayTimeoutHanlder()));
    m_server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!m_server->listen(QHostAddress::Any, 2346))
        setServerStatus("Server could not start");
    else
        setServerStatus("Server started!");

    if(m_server->isListening())
        setServerStatus("Server is listening on Port: " + QString::number(m_server->serverPort()) );

    m_delayTimer.start(200);

    for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
        m_vsData[i] = "---";
}

MyTcpServer::~MyTcpServer()
{
    delete m_server;
    delete m_socket;
}

void MyTcpServer::delayTimeoutHanlder()
{
    writeToSocket();
}

// This function will get called everytime a new telnet connection is established
void MyTcpServer::newConnection()
{
    // If there are no pending connections, bail
    if(m_server->hasPendingConnections())
    {
        m_socket = m_server->nextPendingConnection();
        if(m_socket)
        {
            m_localIP = m_socket->localAddress().toString();
            qDebug() << "Local IP: " << m_localIP;
            emit localIPChanged();
            setConnectionStatus("New connection established: " + m_socket->peerAddress().toString() + " (Port: " + QString::number(m_socket->peerPort()) + ")");
            for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
                m_vsData[i] = simData[i];
        }
        else
            setConnectionStatus( "Failed to establish new connection: " + m_socket->peerAddress().toString() + " (Port: " + QString::number(m_socket->peerPort()) + ")" );
    }
    else
    {
        setConnectionStatus("No Pending Connections");
    }
}

void MyTcpServer::writeToSocket()
{
    char data[100];
    static int index = VS_ID_HR;
    static int jitter = 1;

    if(!m_socket)
        return;

    if(m_socket)
    {
        sprintf(data, "%d^%s", index, m_vsData[index].toStdString().data());

        if(++index >= VS_ID_LAST)
        {
            index = VS_ID_HR;

            for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
                m_vsData[i] = QString::number(simData[i].toDouble() + jitter);

            if(++jitter >= 10)
                jitter = 0;
        }

        if( -1 == m_socket->write(data) )
        {
            setWriteStatus("Socket Write error: " + m_socket->peerAddress().toString() + " (Port: " + QString::number(m_socket->peerPort()) +")");

            if(m_socket != NULL)
            {
                m_socket->close();
                delete m_socket;
                m_socket = NULL;

                for(int i = VS_ID_HR; i < VS_ID_LAST; i++)
                    m_vsData[i] = "---";
            }

            setConnectionStatus("Connection Closed !!!");

            return;
        }

        setWriteStatus( "Writing to socket: " + m_socket->peerAddress().toString() + " (Port: " + QString::number(m_socket->peerPort()) +")" );

        m_socket->flush();
//        m_socket->waitForBytesWritten(500);
    }
}
