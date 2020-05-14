#ifndef AUDIOSHARE_H
#define AUDIOSHARE_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QFile>

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
    QAudioOutput *output = nullptr;
    QIODevice *outputDevice = nullptr;

    //in
    QAudioInput *input = nullptr;
    QIODevice *inputDevice = nullptr;

    QFile destinationFile;

public slots:
    void onAudioReady();
    void handleInputStateChanged(QAudio::State newState);
    void handleOutputStateChanged(QAudio::State newState);

signals:
    void audioReady(const AudioData &audio_data);

};

#endif // AUDIOSHARE_H
