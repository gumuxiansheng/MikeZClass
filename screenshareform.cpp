#include "screenshareform.h"
#include "ui_screenshareform.h"
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include <QPainter>
#include <sharerefresher.h>

ScreenShareForm::ScreenShareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShareForm)
{
    ui->setupUi(this);

    startShareServer();
    startShareScreen();

}

ScreenShareForm::~ScreenShareForm()
{
    delete ui;
    delete server;
}

void ScreenShareForm::updateUi()
{
    pixmap = screen->grabWindow(QApplication::desktop()->winId());//截取桌面
    if (!server->isSendingImage())
    {
        server->sendScreenImage(pixmap);
    }

}

void ScreenShareForm::closeEvent(QCloseEvent *)
{
    server->close();
}

void ScreenShareForm::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
    update();
    return;
}

void ScreenShareForm::startShareServer()
{
    server = new ScreenShareServer;
    QString serverIp = server->getHostIpAddress();
    QHostAddress hostAddr;
    hostAddr.setAddress(serverIp);
    server->runServer(hostAddr, 8910);
    qDebug() << "ServerIp: " + serverIp << "; Port: 8910";
}

void ScreenShareForm::startShareScreen()
{
    screen = QGuiApplication::primaryScreen();

    ShareRefresher *worker = new ShareRefresher;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ScreenShareForm::operate, worker, &ShareRefresher::shareScreen);
    connect(worker, &ShareRefresher::uiChanged, this, &ScreenShareForm::updateUi);
    workerThread.start();
    emit operate();
}
