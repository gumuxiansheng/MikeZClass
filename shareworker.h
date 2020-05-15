#ifndef SHARE_REFRESHER_H
#define SHARE_REFRESHER_H

#include <QObject>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include "rpc/screenshareserver.h"
#include "module/audioshare.h"

class ShareWorker : public QObject
{
    Q_OBJECT

public:
    explicit ShareWorker(QObject *parent = nullptr);
    ~ShareWorker(){
        delete screen_share_timer;
        delete audio_share_timer;
        delete server;
        delete audioShare;
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
        emit notifyUiChanged();
    }
    void shareScreen()
    {
        screen_share_timer = new QTimer(this);
        screen_share_timer->setInterval(180);
        connect(screen_share_timer, SIGNAL(timeout()), this, SLOT(timerProcess()));
        screen_share_timer->start();
        return;
    }
    void sendScreenImage(QPixmap pixmap)
    {
        if (!server->isSendingImage())
        {
            server->sendScreenImage(pixmap);
        }
    }
    void stopScreen()
    {
        screen_share_timer->stop();
    }
    void shareAudio()
    {
        audioShare = AudioShare::GetInstance();
        audioShare->startInput();
        audioShare->grabAudio();
        connect(audioShare, &AudioShare::audioReady, this, &ShareWorker::sendAudio);
    }
    void sendAudio(const AudioShare::AudioData &audio_data)
    {
        if (!server->isSendingAudio())
        {
            server->sendAudio(audio_data);
        }
    }
    void stopAudio()
    {
        audioShare->stop();
    }
    void quit()
    {
        stopAudio();
        stopScreen();
        server->close();
    }

signals:
    void notifyUiChanged();

private:
    ScreenShareServer *server;
    QTimer *screen_share_timer;
    QTimer *audio_share_timer;
    AudioShare *audioShare;
};

#endif // SHARE_REFRESHER_H
