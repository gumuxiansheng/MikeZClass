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
    static AudioShare * GetInstance()
    {
        if(m_pInstance == NULL)  //判断是否第一次调用
            m_pInstance = new AudioShare();
        return m_pInstance;
    }
    ~AudioShare();
    void grabAudio();
    void playAudio(QByteArray data);
    QStringList getInputDevices();
    QStringList getOutputDevices();
    void selectInputDevice(QString deviceName);
    void selectOutputDevice(QString deviceName);
    void startInput();
    void startOutput();
    void stop();
    struct AudioData{
        const qint8 type = 2;
        QByteArray data;
        quint32 lens;
    };

private:
    explicit AudioShare(QObject *parent = nullptr);
    static AudioShare *m_pInstance;

    QAudioFormat format;

    QAudioDeviceInfo selectedInputDevice;
    QAudioDeviceInfo selectedOutputDevice;

    //out
    QAudioOutput *output = nullptr;
    QIODevice *outputDevice = nullptr;

    //in
    QAudioInput *input = nullptr;
    QIODevice *inputDevice = nullptr;

public slots:
    void onAudioReady();
    void handleInputStateChanged(QAudio::State newState);
    void handleOutputStateChanged(QAudio::State newState);

signals:
    void audioReady(const AudioData &audio_data);

};

#endif // AUDIOSHARE_H
