#include "audioshare.h"
#include <QDebug>

AudioShare* AudioShare::m_pInstance = NULL;

AudioShare::AudioShare(QObject *parent) : QObject(parent)
{
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::UnSignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);

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
        break;

    case QAudio::ActiveState:
        // Started recording - read from IO device
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

QStringList AudioShare::getInputDevices()
{
    QStringList aDeviceListI;
    QList<QAudioDeviceInfo> audioDeviceListI = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListI)
    {
        QString strName = devInfo.deviceName();
        if (devInfo.isNull())
        {
            continue;
        }
        if (strName[0] == 65533)
        {
            continue;
        }
        bool bFound = false;
        foreach (QString dev, aDeviceListI)
        {
            if (strName == dev)
            {
                bFound = true;
            }
        }
        if (bFound == true)
        {
            continue;
        }
        aDeviceListI.push_back(strName);
    }

    return aDeviceListI;
}

void AudioShare::selectInputDevice(QString deviceName)
{
    QList<QAudioDeviceInfo> audioDeviceListI = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListI)
    {
        QString strName = devInfo.deviceName();
        if (deviceName == strName)
        {
            selectedInputDevice = devInfo;
            return;
        }
    }
}

void AudioShare::selectOutputDevice(QString deviceName)
{
    QList<QAudioDeviceInfo> audioDeviceListO = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListO)
    {
        QString strName = devInfo.deviceName();
        if (deviceName == strName)
        {
            selectedOutputDevice = devInfo;
            return;
        }
    }
}

QStringList AudioShare::getOutputDevices()
{
    //获取输出音频设备名称
    QStringList aDeviceListO;
    QList<QAudioDeviceInfo> audioDeviceListO = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    foreach (QAudioDeviceInfo devInfo, audioDeviceListO)
    {
        QString strName = devInfo.deviceName();
        if (devInfo.isNull())
        {
            continue;
        }
        if (strName[0] == 65533)
        {
            continue;
        }
        bool bFound = false;
        foreach (QString dev, aDeviceListO)
        {
            if (strName == dev)
            {
                bFound = true;
            }
        }
        if (bFound == true)
        {
            continue;
        }
        aDeviceListO.push_back(strName);
    }
    return  aDeviceListO;
}

void AudioShare::startInput()
{
    if (selectedInputDevice.isNull())
    {
        selectedInputDevice = QAudioDeviceInfo::defaultInputDevice();
    }
    if(!selectedInputDevice.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = selectedInputDevice.nearestFormat(format);
    }

    input = new QAudioInput(format,this);
    connect(input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleInputStateChanged(QAudio::State)));

    inputDevice = input->start();
}

void AudioShare::startOutput()
{
    if (selectedOutputDevice.isNull())
    {
        selectedOutputDevice = QAudioDeviceInfo::defaultOutputDevice();
    }
    if(!selectedOutputDevice.isFormatSupported(format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = selectedOutputDevice.nearestFormat(format);
    }

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
