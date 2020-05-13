#ifndef SCREENSHARE_H
#define SCREENSHARE_H

#include <QObject>
#include <QScreen>
#include <QPixmap>
#include <QApplication>

class ScreenShare : public QObject
{
    Q_OBJECT
public:
    explicit ScreenShare(QObject *parent = nullptr);
    QPixmap grabScreen(WId window);

signals:

private:
    QScreen *screen;

};

#endif // SCREENSHARE_H
