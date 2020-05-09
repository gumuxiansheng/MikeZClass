#include "screenshareserver.h"
#include <QNetworkInterface>
#include <QDataStream>
#include <QPixmap>
#include <QTime>

ScreenShareServer::ScreenShareServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
}

ScreenShareServer::~ScreenShareServer()
{
    delete server;
}

void ScreenShareServer::close()
{
    server->close();
}

void ScreenShareServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionclosed()));

    QString msg = "connected to the server";
    socket->write(msg.toLocal8Bit().constData());
    socket->flush();

    mutex.lock();
    clientsList.insert(socket->peerName(), socket);
    mutex.unlock();


}

void ScreenShareServer::connectionclosed()
{
    mutex.lock();
    clientsList.remove(((QTcpSocket*)sender())->peerName());
    mutex.unlock();

    qDebug() << sender() << "Closed connection!";
}

void ScreenShareServer::readData()
{

}

bool ScreenShareServer::isSendingImage()
{
    return sendingImage;
}

void ScreenShareServer::sendScreenImage(QPixmap pixmap)
{
    if (clientsList.size() == 0)
    {
        return;
    }
    mutex.lock();
//    QTime time;
//    time.start();
    sendingImage = true;
    pixmap = pixmap.scaled(pixmap.width() / 2, pixmap.height() / 2);
    pixmap.save(&buffer, "JPG");
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out<<(quint32)buffer.data().size();
    block.append(buffer.data());
    for (clientI = clientsList.begin(); clientI != clientsList.end(); ++clientI)
    {
//        qDebug("datalen send:%d",(quint32)buffer.data().size());
        clientI.value()->write(block);
    }
    buffer.close();
//    qDebug() << "Encode time:"  << time.elapsed();
    mutex.unlock();
    sendingImage = false;
}

void ScreenShareServer::runServer(QHostAddress address, int port)
{
    if(!server->listen(address,port))
    {
        qDebug() << "Server start failed!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

QString ScreenShareServer::getHostIpAddress()
{
    return "0.0.0.0";
}