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
#include <QtCore/QIODevice>

#include <complex>
#include <fftw3.h>
#include <QtConcurrent/QtConcurrent>

#include "worker.hpp"

AudioWorker::AudioWorker(QObject *parent) : QObject(parent) {
    rawData.clear();

    fftSize = 0;
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

    fftSize = format.sampleRate()/5;
}

void AudioWorker::start() {
    fft1D = new FFT1D(fftSize);
    fft1D->setMax(1024);

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
    int channels = format.channelCount();
    int bytes = format.sampleSize() / 8;
    int increment = channels * bytes;

    while (true) {
        QByteArray b = ioDevice->read(1);
        if (b.length() == 0)
            return;

        rawData.append(b);
        if (rawData.length() == (fftSize) * increment)
            break;
    }

    if (rawData.length() != (fftSize) * increment)
        return;

    parsePayload(rawData);

    rawData.clear();
}

void AudioWorker::parsePayload(const QByteArray &payloadData) {
    int channels = format.channelCount();
    int bytes = format.sampleSize() / 8;
    int increment = channels * bytes;

    emit newAudioData(payloadData);

    QList<double> values;

    for (int i = 0; i < payloadData.length(); i += increment) {
        double v = 0;

        for (int c = 0; c < channels; c++) {
            if (format.byteOrder() == QAudioFormat::LittleEndian) {
                for (int b = 0; b < bytes; b++)
                    v += payloadData[i + (c * b) + b] * qPow(256, b);
            } else {
                for (int b = bytes - 1; b >= 0; b--)
                    v += payloadData[i + (c * b) + b] * qPow(256, b);
            }
        }

        v /= channels;
        values.append(v);
    }

    QtConcurrent::run(this, &AudioWorker::computeRMS, values);
    QtConcurrent::run(this, &AudioWorker::computeFFT, values);
}

void AudioWorker::computeRMS(QList<double> &values) {
    double sum = 0;

    for (const double &v: values)
        sum += qPow(v, 2);

    double rms = qSqrt(sum);
    emit newAudioRms(rms);
}

void AudioWorker::computeFFT(QList<double> &values) {
    QList<double> fft = fft1D->execute(values);

    emit newAudioFFT(fft);
}
