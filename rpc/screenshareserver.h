#ifndef SCREENSHARESERVER_H
#define SCREENSHARESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QBuffer>
#include <QMutex>
#include "../module/audioshare.h"

class ScreenShareServer : public QObject
{
    Q_OBJECT

public:
    explicit ScreenShareServer(QObject *parent = 0);
    ~ScreenShareServer();
    void runServer(QHostAddress address, int port);
    QString getHostIpAddress();
    void close();
    void sendScreenImage(QPixmap pixmap);
    bool isSendingImage();
    void sendAudio(AudioShare::AudioData audio_data);
    bool isSendingAudio();

private:
    QTcpServer *server;
    QHash<QString, QTcpSocket*> clientsList;
    QHash<QString, QTcpSocket*>::iterator clientI;

    QMutex mutex;
    QBuffer buffer;
    bool sendingImage = false;
    bool sendingAudio = false;


signals:

public slots:
    void newConnection();
    void connectionclosed();
    void readData();
};

#endif // SCREENSHARESERVER_H
