#ifndef AUDIOSHARE_H
#define AUDIOSHARE_H

#include <QObject>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QThread>

class AudioShare : public QObject
{
    Q_OBJECT
public:
    static AudioShare * GetInstance()
    {
        // this instance should be only used in main thread
        if(m_pInstance == NULL)
        {
            m_pInstance = new AudioShare();
        }
        return NewInstance(m_pInstance);
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
    static AudioShare * NewInstance(AudioShare * audioShare)
    {
        QAudioFormat format = audioShare->format;

        QAudioDeviceInfo selectedInputDevice = audioShare->selectedInputDevice;
        QAudioDeviceInfo selectedOutputDevice = audioShare->selectedOutputDevice;

        //out
        QAudioOutput *output = audioShare->output;
        QIODevice *outputDevice = audioShare->outputDevice;

        //in
        QAudioInput *input = audioShare->input;
        QIODevice *inputDevice = audioShare->inputDevice;

        m_pInstance = new AudioShare();
        m_pInstance->format = format;
        m_pInstance->input = input;
        m_pInstance->output = output;
        m_pInstance->inputDevice = inputDevice;
        m_pInstance->outputDevice = outputDevice;
        m_pInstance->selectedInputDevice = selectedInputDevice;
        m_pInstance->selectedOutputDevice = selectedOutputDevice;
        return m_pInstance;
    }

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
