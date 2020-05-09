#ifndef SCREENSHARECLIENT_H
#define SCREENSHARECLIENT_H

#include <QTcpSocket>
#include <QObject>
#include <QPixmap>
#include <QMutex>

class ScreenShareClient : public QObject
{
    Q_OBJECT
    QTcpSocket *socket;
    quint32 data_len = 0;
    quint32 len = 0xffffffff;
    QByteArray array;
    QPixmap pixmap;
    QMutex mutex;
public:
    explicit ScreenShareClient(QObject *parent = nullptr);
    ~ScreenShareClient();
    void close();

signals:
    void imageRead(const QPixmap &pixmap);

public slots:
    void readData();

};

#endif // SCREENSHARECLIENT_H
