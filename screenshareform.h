#ifndef SCREENSHAREFORM_H
#define SCREENSHAREFORM_H

#include <QWidget>
#include <QThread>
#include "rpc/screenshareserver.h"
#include "sharerefresher.h"

namespace Ui {
class ScreenShareForm;
}

class ScreenShareForm : public QWidget
{
    Q_OBJECT
    QThread workerThread;
    QScreen *screen;
    QPixmap pixmap;

public:
    explicit ScreenShareForm(QWidget *parent = nullptr);
    ~ScreenShareForm();
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::ScreenShareForm *ui;
    ShareRefresher *worker;

    void startShareScreen();

public slots:
    void updateUi();

signals:
    void operate();
    void closed();
    void screenPrepared(QPixmap pixmap);
};

#endif // SCREENSHAREFORM_H
