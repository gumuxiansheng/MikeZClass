#include "audioshare.h"
#include <QDebug>

AudioShare::AudioShare(QObject *parent) : QObject(parent)
{
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }

    output = new QAudioOutput(format,this);
    input = new QAudioInput(format,this);

}

AudioShare::~AudioShare()
{
    delete input;
    delete output;
}

void AudioShare::startInput()
{
    inputDevice = input->start();
}

void AudioShare::startOutput()
{
    outputDevice = output->start();//开始播放
}

void AudioShare::stop()
{
    input->stop();
    output->stop();
    outputDevice->close();
    inputDevice->close();
}

void AudioShare::grabAudio()
{
    connect(inputDevice, SIGNAL(readyRead()), this, SLOT(onAudioReady()));//send
}

void AudioShare::playAudio(QByteArray data)
{
    qDebug("playAudio size: %d", data.size());
    outputDevice->write(data.constData(), data.size());
}

void AudioShare::onAudioReady()
{
    AudioData audio_data;
    //读取音频
    char audiodata[65536];
    int lens = inputDevice->read(audiodata, 65536);
//    audio_data.data = inputDevice->read(1024);
//    audio_data.lens = audio_data.data.size();

    outputDevice = output->start();
//    outputDevice->write(audio_data.data);
    outputDevice->write(audiodata, lens);
    qDebug() << audiodata;
    emit audioReady(audio_data);
}
