#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "screenshareform.h"
#include "shareviewform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startMeetingBtn, SIGNAL(clicked()), this, SLOT(clickStartMeetingBtn()));
    connect(ui->joinMeetingBtn, SIGNAL(clicked()), this, SLOT(clickJoinMeetingBtn()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickStartMeetingBtn()
{
    startMeeting();
}

void MainWindow::clickJoinMeetingBtn()
{
    joinMeeting();
}

void MainWindow::startMeeting()
{
    ScreenShareForm *screenShareForm = new ScreenShareForm();
    screenShareForm->show();
    return;
}

void MainWindow::joinMeeting()
{
    ShareViewForm *shareViewForm = new ShareViewForm();
    shareViewForm->show();
    return;
}
