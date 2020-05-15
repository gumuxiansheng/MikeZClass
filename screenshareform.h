#ifndef SCREENSHAREFORM_H
#define SCREENSHAREFORM_H

#include <QWidget>
#include <QThread>
#include "rpc/screenshareserver.h"
#include "shareworker.h"
#include "module/screenshare.h"

namespace Ui {
class ScreenShareForm;
}

class ScreenShareForm : public QWidget
{
    Q_OBJECT
    QThread workerThread;
    ScreenShare *screenShare;
    AudioShare *audioShare;
    QPixmap pixmap;

public:
    explicit ScreenShareForm(QWidget *parent = nullptr);
    ~ScreenShareForm();
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::ScreenShareForm *ui;
    ShareWorker *worker;

    void startShareServer();
    void startShareScreen();
    void startShareAudio();

public slots:
    void updateUi();

signals:
    void operateScreen();
    void operateAudio();
    void closed();
    void screenPrepared(QPixmap pixmap);
    void audioPrepared(AudioShare::AudioData audio_data);
};

#endif // SCREENSHAREFORM_H
