#include "screenshareclient.h"
#include <QBuffer>
#include <QImageReader>
#include <QTime>

ScreenShareClient::ScreenShareClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    socket->setReadBufferSize(9000000);
//    socket->connectToHost("10.211.55.3", 8910);
    socket->connectToHost("0.0.0.0", 8910);
    socket->connected();
}

ScreenShareClient::~ScreenShareClient()
{
    delete socket;
}

void ScreenShareClient::close()
{
    socket->close();
    socket->deleteLater();
}

void ScreenShareClient::readData()
{
    mutex.lock();
    QTime time;
    time.start();
    quint32 bytesLen = socket->bytesAvailable();
    quint32 readLen = 0;
    qDebug("readData: %d", bytesLen);

    qint8 data_type = -1;
    while (readLen < bytesLen)
    {
        if(data_len == 0)
        {
            QDataStream in(socket);
            in >> data_type;
            in >> len;
            readLen += 5;
            qDebug("datatype: %d, datalen: %d", data_type, len);

        }
        array.append(socket->read(len - data_len));
        readLen += len - data_len;
        data_len = array.size();

        if (data_len == len)
        {
            if (data_type == 0)
            {
                qDebug() << "info read";
                qDebug() << array;
                array.clear();
                array.squeeze();
                array.reserve(data_len);
                len = 0xffffffff;
                data_len = 0;
            } else if (data_type == 1)
            {
                qDebug() << "imageRead";
                pixmap.loadFromData(array, "JPG");
                array.clear();
                array.squeeze();
                array.reserve(data_len);
                len = 0xffffffff;
                data_len = 0;
                qDebug() << "Decode time:" << time.elapsed();
                emit imageRead(pixmap);
            } else if (data_type == 2)
            {
                qDebug() << "audio read";
                emit audioRead(array);
                array.clear();
                array.squeeze();
                array.reserve(data_len);
                len = 0xffffffff;
                data_len = 0;

            }
        }
    }


    mutex.unlock();

}

void ScreenShareClient::connectionClosed()
{
    qDebug("server connectionClosed");
}
