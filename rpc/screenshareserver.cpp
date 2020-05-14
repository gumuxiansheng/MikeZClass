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
    mutex.lock();
    for (clientI = clientsList.begin(); clientI != clientsList.end(); ++clientI)
    {
        clientI.value()->close();
    }
    server->close();
    mutex.unlock();
}

void ScreenShareServer::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionclosed()));

    QString msg = "connected to the server";
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out<<(quint8)0; // indicates info message
    out<<(quint32)msg.size();
    block.append(msg.toLocal8Bit().constData());
    socket->write(block);
    socket->flush();

    mutex.lock();
    clientsList.insert(socket->peerName(), socket);
    mutex.unlock();


}

void ScreenShareServer::connectionclosed()
{
    qDebug() << sender() << "Closing connection!";
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

bool ScreenShareServer::isSendingAudio()
{
    return sendingAudio;
}

void ScreenShareServer::sendScreenImage(QPixmap pixmap)
{
    if (clientsList.size() == 0)
    {
//        qDebug() << "clientsList empty";
        return;
    }
    //    QTime time;
    //    time.start();
    sendingImage = true;
    pixmap = pixmap.scaled(pixmap.width() / 2, pixmap.height() / 2);
    pixmap.save(&buffer, "JPG");
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out<<(qint8)1; // indicates image type
    out<<(quint32)buffer.data().size();
    block.append(buffer.data());
    mutex.tryLock(100);
//    qDebug() << "sendingImage true; ";
    for (clientI = clientsList.begin(); clientI != clientsList.end(); ++clientI)
    {
        if (clientI.value()->state() == QAbstractSocket::ConnectedState)
        {
            qDebug() << "sendingImage xxxx; " << clientI.value()->state();
            qint64 write_len = clientI.value()->write(block);
//            clientI.value()->flush();
            qDebug("datalen send expect: %d, actual: %d",block.size(), write_len);
        }

    }
    //    qDebug() << "Encode time:"  << time.elapsed();
    mutex.unlock();
    buffer.close();
    sendingImage = false;
    qDebug("sendingImage false");
}

void ScreenShareServer::sendAudio(AudioShare::AudioData audio_data)
{
    if (clientsList.size() == 0)
    {
        return;
    }
    //    QTime time;
    //    time.start();
    sendingAudio = true;
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out<<(qint8)audio_data.type;
    out<<(quint32)audio_data.lens;
    block.append(audio_data.data);
    mutex.tryLock(100);
//    qDebug() << "sendingAudio true; " << clientsList.size();
    for (clientI = clientsList.begin(); clientI != clientsList.end(); ++clientI)
    {
        if (clientI.value()->isOpen())
        {
//            qDebug() << "sendingAudio xxxx; ";
            qint64 write_len = clientI.value()->write(block);
//            clientI.value()->flush();
            qDebug("datalen send expect: %d, actual: %d",block.size(), write_len);
        }

    }
    //    qDebug() << "Encode time:"  << time.elapsed();
    mutex.unlock();
    sendingAudio = false;
    qDebug("sendingAudio false");
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
