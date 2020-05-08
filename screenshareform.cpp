#include "screenshareform.h"
#include "ui_screenshareform.h"

ScreenShareForm::ScreenShareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShareForm)
{
    ui->setupUi(this);
}

ScreenShareForm::~ScreenShareForm()
{
    delete ui;
}
