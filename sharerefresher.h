#ifndef SHARE_REFRESHER_H
#define SHARE_REFRESHER_H

#include <QObject>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>

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
    }

public slots:
    void timerProcess()
    {
        emit uiChanged();
    }
    void shareScreen()
    {
        timer = new QTimer(this);
        timer->setInterval(180);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerProcess()));
        timer->start();
        return;
    }
    void quit()
    {
        timer->stop();
    }

signals:
    void uiChanged();

};

#endif // SHARE_REFRESHER_H
