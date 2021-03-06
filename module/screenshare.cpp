#include "screenshare.h"

ScreenShare* ScreenShare::m_pInstance = NULL;

ScreenShare::ScreenShare(QObject *parent) : QObject(parent)
{
    screen = QGuiApplication::primaryScreen();
}

QPixmap ScreenShare::grabScreen(WId window)
{
    return screen->grabWindow(window);
}
