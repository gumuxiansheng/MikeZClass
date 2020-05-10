#ifndef SCREENSHARESERVER_H
#define SCREENSHARESERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QBuffer>
#include <QMutex>

class ScreenShareServer : public QObject
{
    Q_OBJECT

public:
    explicit ScreenShareServer(QObject *parent = 0);
    ~ScreenShareServer();
    void runServer(QHostAddress address, int port);
    QString getHostIpAddress();
    void sendScreenImage(QPixmap pixmap);
    void close();
    bool isSendingImage();

private:
    QTcpServer *server;
    QHash<QString, QTcpSocket*> clientsList;
    QHash<QString, QTcpSocket*>::iterator clientI;
    QMutex mutex;
    QBuffer buffer;
    bool sendingImage = false;

signals:

public slots:
    void newConnection();
    void connectionclosed();
    void readData();
};

#endif // SCREENSHARESERVER_H
