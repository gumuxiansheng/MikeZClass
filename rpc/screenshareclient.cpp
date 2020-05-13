#include "screenshareclient.h"
#include <QBuffer>
#include <QImageReader>
#include <QTime>

ScreenShareClient::ScreenShareClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
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
    if (bytesLen < 100)
    {
        qDebug() << socket->readAll();
    } else
    { // It should be image
        while (socket->bytesAvailable()) {
            if(data_len == 0)
            {
                QDataStream in(socket);
                in >> len;
                qDebug() << "datalen:" << len;
                if (bytesLen - readLen > len + 1000)
                {
                    socket->read(len - data_len);
                    readLen += len - data_len;
                    array.clear();
                    array.squeeze();
                    array.reserve(data_len);
                    len = 0xffffffff;
                    data_len = 0;
                    continue;
                }
            }
            array.append(socket->read(len - data_len));
            readLen += len - data_len;
            data_len = array.size();

            if (data_len == len)
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
            }
        }
    }

    mutex.unlock();

}
