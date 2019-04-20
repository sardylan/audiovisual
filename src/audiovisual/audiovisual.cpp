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

#include <QDebug>
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
    connect(audioWorker, &AudioWorker::newAudioData, this, &AudioVisual::newAudioData);

    connect(mainWindow, &MainWindow::showConfiguration, this, &AudioVisual::showConfiguration);
    connect(mainWindow, &MainWindow::toggleRunning, this, &AudioVisual::toggleRun);
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
        for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
            if (audioDeviceInfo.deviceName() == config->getAudioDevice()) {
                audioWorker->setDeviceInfo(audioDeviceInfo);

                QAudioFormat audioFormat;
                audioFormat.setChannelCount(config->getAudioChannels());
                audioFormat.setSampleRate(config->getAudioSampleRate());
                audioFormat.setSampleSize(config->getAudioSampleSize());
                audioFormat.setSampleType(config->getAudioSampleType());

                const QAudioFormat &nearestFormat = audioDeviceInfo.nearestFormat(audioFormat);
                qDebug() << audioFormat;
                qDebug() << nearestFormat;

                ggg = qPow(256, (int) (nearestFormat.sampleSize() / 8));
                mainWindow->updateVuMeterMax(ggg);

                audioWorker->setFormat(nearestFormat);
                audioWorker->start();

                status->setRunning(true);
            }
        }
    } else {
        audioWorker->stop();
        status->setRunning(false);
    }
}

void AudioVisual::newAudioData(const QByteArray &data) {
    int channels = config->getAudioChannels();
    int bytes = config->getAudioSampleSize() / 8;
    int increment = channels + bytes;

    double sum = 0;
    int samples = 0;

    for (int i = 0; i < data.length(); i += increment) {
        for (int c = 0; c < channels; c++) {
            for (int b = 0; b < bytes; b++) {
                sum += qPow(data[i + (c * bytes) + b] * (b * 256), 2);
                samples++;
            }
        }

    }

    double mid = qSqrt(sum);

    mainWindow->updateVuMeter(mid);

    int level = (int) (1024 * (mid / ggg));
    QList<double> wfData;
    wfData.append(level);
    mainWindow->updateWaterfall(wfData);
}
