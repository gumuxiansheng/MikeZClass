#include "screenshareform.h"
#include "ui_screenshareform.h"
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include <QPainter>

ScreenShareForm::ScreenShareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShareForm)
{
    ui->setupUi(this);

    startShareScreen();

}

ScreenShareForm::~ScreenShareForm()
{
    delete ui;
}

void ScreenShareForm::updateUi()
{
    pixmap = screen->grabWindow(QApplication::desktop()->winId());//截取桌面
    emit screenPrepared(pixmap);
}

void ScreenShareForm::closeEvent(QCloseEvent *)
{
    emit closed();
}

void ScreenShareForm::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
    update();
    return;
}

void ScreenShareForm::startShareScreen()
{
    screen = QGuiApplication::primaryScreen();

    worker = new ShareRefresher;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ScreenShareForm::operate, worker, &ShareRefresher::shareScreen); // start grab screen every Interval time
    connect(this, &ScreenShareForm::screenPrepared, worker, &ShareRefresher::sendScreenImage); // when screen image prepared, use worker thread to send screen image through rpc
    connect(worker, &ShareRefresher::uiChanged, this, &ScreenShareForm::updateUi); // once timer reached, update the screen image
    connect(this, &ScreenShareForm::closed, worker, &ShareRefresher::quit);
    workerThread.start();
    emit operate();
}
