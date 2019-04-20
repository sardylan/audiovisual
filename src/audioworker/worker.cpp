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
#include <QIODevice>

#include "worker.hpp"

AudioWorker::AudioWorker(QObject *parent) : QObject(parent) {
}

AudioWorker::~AudioWorker() = default;

const QAudioDeviceInfo &AudioWorker::getDeviceInfo() const {
    return deviceInfo;
}

void AudioWorker::setDeviceInfo(const QAudioDeviceInfo &value) {
    AudioWorker::deviceInfo = value;
}

const QAudioFormat &AudioWorker::getFormat() const {
    return format;
}

void AudioWorker::setFormat(const QAudioFormat &value) {
    AudioWorker::format = value;
}

void AudioWorker::start() {
    audioThread = new QThread(this);
    connect(audioThread, &QThread::finished, audioThread, &QThread::deleteLater);

    audioInput = new QAudioInput(deviceInfo, format);
    audioInput->moveToThread(audioThread);

    ioDevice = audioInput->start();
    connect(ioDevice, &QIODevice::readyRead, this, &AudioWorker::readAvailableData);

    audioThread->start();

    emit newStatus(true);
}

void AudioWorker::stop() {
    disconnect(ioDevice, &QIODevice::readyRead, this, &AudioWorker::readAvailableData);

    audioInput->stop();
    audioInput->deleteLater();

    audioThread->quit();
    audioThread->deleteLater();

    emit newStatus(false);
}

void AudioWorker::readAvailableData() {
    QByteArray data = ioDevice->readAll();

    if (data.size() > 0)
            emit newAudioData(data);
}
