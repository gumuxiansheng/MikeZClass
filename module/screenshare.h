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
    static ScreenShare * GetInstance()
    {
        if(m_pInstance == NULL)  //判断是否第一次调用
            m_pInstance = new ScreenShare();
        return m_pInstance;
    }
    QPixmap grabScreen(WId window);

signals:

private:
    explicit ScreenShare(QObject *parent = nullptr);
    static ScreenShare *m_pInstance;
    QScreen *screen;

};

#endif // SCREENSHARE_H
