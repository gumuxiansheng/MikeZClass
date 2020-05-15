#ifndef SCREENSHARE_H
#define SCREENSHARE_H

#include <QObject>
#include <QScreen>
#include <QPixmap>
#include <QApplication>
#include <QThread>

class ScreenShare : public QObject
{
    Q_OBJECT
public:
    static ScreenShare * GetInstance()
    {
        if(m_pInstance == NULL)
        {
            m_pInstance = new ScreenShare();
        }
        return NewInstance(m_pInstance);
    }
    QPixmap grabScreen(WId window);

signals:

private:
    explicit ScreenShare(QObject *parent = nullptr);
    static ScreenShare *m_pInstance;
    static ScreenShare * NewInstance(ScreenShare * screenShare)
    {
        QScreen * screen = screenShare->screen;
        m_pInstance = new ScreenShare();
        m_pInstance->screen = screen;
        return m_pInstance;
    }

    QScreen *screen;

};

#endif // SCREENSHARE_H
