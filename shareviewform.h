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

public:
    explicit ShareViewForm(QWidget *parent = nullptr);
    ~ShareViewForm();
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::ShareViewForm *ui;
    QPixmap pixmap;
    ScreenShareClient *client;
    AudioShare *audioShare;

public slots:
    void updateView(const QPixmap &pixmap);
    void playAudio(const QByteArray &audio_data);
};

#endif // SHAREVIEWFORM_H
