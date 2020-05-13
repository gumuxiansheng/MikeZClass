#ifndef SHARE_REFRESHER_H
#define SHARE_REFRESHER_H

#include <QObject>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include "rpc/screenshareserver.h"

class ShareRefresher : public QObject
{
    Q_OBJECT
    QTimer *timer;
    QScreen *screen;

public:
    explicit ShareRefresher(QObject *parent = nullptr);
    ~ShareRefresher(){
        delete timer;
        delete screen;
        delete server;
    }
    void startShareServer()
    {
        server = new ScreenShareServer;
        QString serverIp = server->getHostIpAddress();
        QHostAddress hostAddr;
        hostAddr.setAddress(serverIp);
        server->runServer(hostAddr, 8910);
        qDebug() << "ServerIp: " + serverIp << "; Port: 8910";
    }

public slots:
    void timerProcess()
    {
        emit uiChanged();
    }
    void shareScreen()
    {
        startShareServer();
        timer = new QTimer(this);
        timer->setInterval(180);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerProcess()));
        timer->start();
        return;
    }
    void sendScreenImage(QPixmap pixmap)
    {
        if (!server->isSendingImage())
        {
            server->sendScreenImage(pixmap);
        }
    }
    void quit()
    {
        server->close();
        timer->stop();
    }

signals:
    void uiChanged();

private:
    ScreenShareServer *server;
};

#endif // SHARE_REFRESHER_H
