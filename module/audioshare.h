#ifndef AUDIOSHARE_H
#define AUDIOSHARE_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>

class AudioShare : public QObject
{
    Q_OBJECT
public:
    explicit AudioShare(QObject *parent = nullptr);
    ~AudioShare();
    void grabAudio();
    void playAudio(QByteArray data);
    void startInput();
    void startOutput();
    void stop();
    struct AudioData{
        const qint8 type = 2;
        QByteArray data;
        quint32 lens;
    };

private:
    QAudioFormat format;

    //out
    QAudioOutput *output;
    QIODevice *outputDevice;

    //in
    QAudioInput *input;
    QIODevice *inputDevice;

public slots:
    void onAudioReady();

signals:
    void audioReady(const AudioData &audio_data);

};

#endif // AUDIOSHARE_H
