/*
 * AudioVisual
 * Copyright (C) 2019  Luca Cireddu
 * sardylan@gmail.com
 * https://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QtCore/QDebug>
#include <QtCore/QtMath>

#include "audiovisual.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    AudioVisual qFM1000(argc, argv);
    qFM1000.prepare();
    return qFM1000.run();
}

AudioVisual::AudioVisual(int &argc, char **argv) : QApplication(argc, argv) {
    status = Status::getInstance();
    config = Config::getInstance();

    audioWorker = new AudioWorker();

    mainWindow = new MainWindow();
    configWindow = new ConfigWindow();
}

AudioVisual::~AudioVisual() {
    delete audioWorker;

    delete configWindow;
    delete mainWindow;
}

void AudioVisual::prepare() {
    connect(status, &Status::updateRunning, mainWindow, &MainWindow::updateRunning);

    connect(audioWorker, &AudioWorker::newStatus, status, &Status::setRunning);
    connect(audioWorker, &AudioWorker::newMaxFrequency, this, &AudioVisual::newMaxFrequency);
    connect(audioWorker, &AudioWorker::newAudioRms, this, &AudioVisual::newAudioRms);
    connect(audioWorker, &AudioWorker::newAudioFFT, this, &AudioVisual::newAudioFFT);

    connect(mainWindow, &MainWindow::showConfiguration, this, &AudioVisual::showConfiguration);
    connect(mainWindow, &MainWindow::toggleRunning, this, &AudioVisual::toggleRun);
    connect(mainWindow, &MainWindow::newGainValue, this, &AudioVisual::newGainValue);
}

int AudioVisual::run() {
    mainWindow->show();

    return QApplication::exec();
}

void AudioVisual::showConfiguration() {
    configWindow->exec();
}

void AudioVisual::toggleRun(bool value) {
    if (value) {
        QAudioDeviceInfo audioInputDeviceInfo;
        QAudioDeviceInfo audioOutputDeviceInfo;

        for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
            if (audioDeviceInfo.deviceName() == config->getAudioDevice())
                audioInputDeviceInfo = audioDeviceInfo;

        for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
            if (audioDeviceInfo.deviceName() == config->getAudioOutputDevice())
                audioOutputDeviceInfo = audioDeviceInfo;

        audioWorker->setDeviceInfo(audioInputDeviceInfo);
        audioWorker->setDeviceOutputInfo(audioOutputDeviceInfo);

        QAudioFormat audioInputFormat;
        audioInputFormat.setChannelCount(config->getAudioChannels());
        audioInputFormat.setSampleRate(config->getAudioSampleRate());
        audioInputFormat.setSampleSize(config->getAudioSampleSize());
        audioInputFormat.setSampleType(config->getAudioSampleType());
        const QAudioFormat &nearestInputFormat = audioInputDeviceInfo.nearestFormat(audioInputFormat);

        QAudioFormat audioOutputFormat;
        audioOutputFormat.setChannelCount(config->getAudioChannels());
        audioOutputFormat.setSampleRate(config->getAudioSampleRate());
        audioOutputFormat.setSampleSize(config->getAudioSampleSize());
        audioOutputFormat.setSampleType(config->getAudioSampleType());
        const QAudioFormat &nearestOutputFormat = audioOutputDeviceInfo.nearestFormat(audioOutputFormat);

        qDebug() << audioInputFormat;
        qDebug() << nearestInputFormat;
        qDebug() << audioOutputFormat;
        qDebug() << nearestOutputFormat;

        audioMaxValue = qPow(2, nearestInputFormat.sampleSize());
        mainWindow->updateVuMeterMax(audioMaxValue);

        audioWorker->setFormat(nearestInputFormat);
        audioWorker->setOutputFormat(nearestOutputFormat);

        audioWorker->start();

        status->setRunning(true);
    } else {
        audioWorker->stop();
        status->setRunning(false);
    }
}

void AudioVisual::newGainValue(double value) {
    audioWorker->setGain(value);
}

void AudioVisual::newMaxFrequency(const unsigned int &maxFrequency) {
    mainWindow->updateWaterfallMaxFrequency(maxFrequency);
}

void AudioVisual::newAudioRms(const double &rms) {
    mainWindow->updateVuMeter(rms);
}

void AudioVisual::newAudioFFT(const QList<double> &fft) {
    mainWindow->updateWaterfall(fft);
}
