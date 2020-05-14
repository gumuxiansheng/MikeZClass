#include "audioshare.h"
#include <QDebug>

AudioShare::AudioShare(QObject *parent) : QObject(parent)
{
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::UnSignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(!info.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }

}

AudioShare::~AudioShare()
{
    delete input;
    delete output;
}

void AudioShare::handleInputStateChanged(QAudio::State newState)
{
    qWarning() << "Input: " << newState;
    switch (newState) {
        case QAudio::StoppedState:
//            if (input->error() != QAudio::NoError) {
//                // Error handling
//                qWarning() << input->error();
//            } else {
//                // Finished recording
//            }
            break;

        case QAudio::ActiveState:
            // Started recording - read from IO device
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}

void AudioShare::handleOutputStateChanged(QAudio::State newState)
{
    qWarning() << "Output: " << newState;
    switch (newState) {
        case QAudio::StoppedState:
//            qWarning() << input->error();
//            if (input->error() != QAudio::NoError) {
//                // Error handling
//                qWarning() << input->error();
//            } else {
//                // Finished recording
//            }
            break;

        case QAudio::ActiveState:
            // Started recording - read from IO device
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}

void AudioShare::startInput()
{
    input = new QAudioInput(format,this);
    connect(input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleInputStateChanged(QAudio::State)));

    inputDevice = input->start();
}

void AudioShare::startOutput()
{
    output = new QAudioOutput(format,this);
    connect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleOutputStateChanged(QAudio::State)));

    outputDevice = output->start();//开始播放
}

void AudioShare::stop()
{
    if (nullptr != input)
    {
        input->stop();
    }
    if (nullptr != output)
    {
        output->stop();
    }
    if (nullptr != outputDevice && outputDevice->isOpen())
    {
        outputDevice->close();
    }
    if (nullptr != inputDevice && inputDevice->isOpen())
    {
        inputDevice->close();
    }
}

void AudioShare::grabAudio()
{
    connect(inputDevice, SIGNAL(readyRead()), this, SLOT(onAudioReady()));//send
}

void AudioShare::playAudio(QByteArray data)
{
    qDebug("playAudio size: %d", data.size());
    if (nullptr != outputDevice && outputDevice->isOpen())
    {
        outputDevice->write(data.constData(), data.size());
    }
}

void AudioShare::onAudioReady()
{
    AudioData audio_data;
    //读取音频
    audio_data.data = inputDevice->readAll();
    audio_data.lens = audio_data.data.size();

    emit audioReady(audio_data);
}
