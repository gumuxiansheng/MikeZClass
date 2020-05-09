#ifndef SHAREVIEWFORM_H
#define SHAREVIEWFORM_H

#include <QWidget>
#include <QThread>
#include "rpc/screenshareclient.h"

namespace Ui {
class ShareViewForm;
}

class ShareViewForm : public QWidget
{
    Q_OBJECT
    QPixmap pixmap;
    ScreenShareClient *client;

public:
    explicit ShareViewForm(QWidget *parent = nullptr);
    ~ShareViewForm();
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::ShareViewForm *ui;

public slots:
    void updateView(const QPixmap &pixmap);
};

#endif // SHAREVIEWFORM_H
