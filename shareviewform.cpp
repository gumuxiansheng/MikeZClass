#include "shareviewform.h"
#include "ui_shareviewform.h"
#include <QPainter>

ShareViewForm::ShareViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareViewForm)
{
    ui->setupUi(this);
    client = new ScreenShareClient;
    audioShare = new AudioShare;
    audioShare->startOutput();

    connect(client, &ScreenShareClient::imageRead, this, &ShareViewForm::updateView);
    connect(client, &ScreenShareClient::audioRead, this, &ShareViewForm::playAudio);
}

ShareViewForm::~ShareViewForm()
{
    delete ui;
    delete client;
}

void ShareViewForm::updateView(const QPixmap &pixmap)
{
    this->pixmap = pixmap;
}

void ShareViewForm::playAudio(const QByteArray &audio_data)
{
    audioShare->playAudio(audio_data);
}

void ShareViewForm::closeEvent(QCloseEvent *)
{
    client->close();
}

void ShareViewForm::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
    update();
    return;
}
