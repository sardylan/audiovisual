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
    gain = 1;
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

    fftSize = format.sampleRate() / 10;
}

void AudioWorker::start() {
    fft1D = new FFT1D(fftSize, qPow(2, format.sampleSize()) / 2, 1024);

    audioThread = new QThread(this);
    connect(audioThread, &QThread::finished, audioThread, &QThread::deleteLater);

    audioInput = new QAudioInput(deviceInfo, format);
    audioInput->moveToThread(audioThread);

    ioDevice = audioInput->start();
    connect(ioDevice, &QIODevice::readyRead, this, &AudioWorker::readAvailableData);

    audioThread->start();

    emit newMaxFrequency(format.sampleRate());

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
        char c;
        qint64 bytesRead = ioDevice->read(&c, 1);
        if (bytesRead <= 0)
            return;

        rawData.append(c);
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
            QByteArray byteValue;

            for (int b = 0; b < bytes; b++)
                byteValue.append(payloadData[i + (c * b) + b]);

            if (format.byteOrder() == QAudioFormat::LittleEndian)
                v += qFromLittleEndian<qint16>(byteValue);
            else
                v += qFromBigEndian<qint16>(byteValue);
        }

        v /= channels;

        v *= gain;

        if (v > qPow(256, bytes))
            v = qPow(256, bytes);
        if (v < -qPow(256, bytes))
            v = -qPow(256, bytes);

        values.append(v);
    }

    QtConcurrent::run(this, &AudioWorker::computeRMS, values);
    QtConcurrent::run(this, &AudioWorker::computeFFT, values);
}

void AudioWorker::computeRMS(QList<double> &values) {
    double sum = 0;

    for (const double &v: values)
        sum += v * v;

    double rms = qSqrt(sum / values.length());
    emit newAudioRms(rms);
}

void AudioWorker::computeFFT(QList<double> &values) {
    QList<double> fft = fft1D->execute(values);

    emit newAudioFFT(fft);
}

double AudioWorker::getGain() const {
    return gain;
}

void AudioWorker::setGain(double value) {
    AudioWorker::gain = value;
}
