#include "screenshareform.h"
#include "ui_screenshareform.h"
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QThread>
#include <QPainter>
#include <QInputDialog>

ScreenShareForm::ScreenShareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShareForm)
{
    ui->setupUi(this);

    startShareServer();
    startShareScreen();
    startShareAudio();

}

ScreenShareForm::~ScreenShareForm()
{
    delete ui;
    delete screenShare;
}

void ScreenShareForm::updateUi()
{
    pixmap = screenShare->grabScreen(QApplication::desktop()->winId());//截取桌面
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

void ScreenShareForm::startShareServer()
{
    worker = new ShareWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::started, worker, &ShareWorker::startShareServer); // start listening server while thread started
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater); // delete worker after thread is finished
    connect(this, &ScreenShareForm::operateScreen, worker, &ShareWorker::shareScreen); // start grab screen every Interval time
    connect(this, &ScreenShareForm::screenPrepared, worker, &ShareWorker::sendScreenImage); // when screen image prepared, use worker thread to send screen image through rpc
    connect(worker, &ShareWorker::notifyUiChanged, this, &ScreenShareForm::updateUi); // once timer reached, update the screen image
    connect(this, &ScreenShareForm::operateAudio, worker, &ShareWorker::shareAudio);
    connect(this, &ScreenShareForm::closed, worker, &ShareWorker::quit);
    workerThread.start();
}

void ScreenShareForm::startShareScreen()
{
    screenShare = ScreenShare::GetInstance();
    emit operateScreen();
}

void ScreenShareForm::startShareAudio()
{
    audioShare = AudioShare::GetInstance();
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Select Audio Device"),
                                         tr("Your audio device:"), audioShare->getInputDevices(), 0, false, &ok);
    if (ok && !item.isEmpty())
    {
        audioShare->selectInputDevice(item);
    }
    emit operateAudio();
}
