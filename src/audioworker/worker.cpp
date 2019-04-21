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

    if (data.size() == 0)
        return;

    emit newAudioData(data);

    int channels = format.channelCount();
    int bytes = format.sampleSize() / 8;
    int increment = channels + bytes;

    QList<double> values;

    for (int i = 0; i < data.length(); i += increment)
        for (int c = 0; c < channels; c++)
            for (int b = 0; b < bytes; b++)
                values.append(data[i + (c * bytes) + b] * (b * 256));

    double rms = computeRms(values);
    emit newAudioRms(rms);

    QList<double> fft = computeFFT(values);
    emit newAudioFFT(fft);
}

double AudioWorker::computeRms(QList<double> &values) {
    double sum = 0;

    for (const double &v: values)
        sum += qPow(v, 2);

    return qSqrt(sum);
}

QList<double> AudioWorker::computeFFT(QList<double> &values) {
    unsigned int n = values.size();

    auto *in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * n);
    auto *out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * n);

    for (int i = 0; i < n; i++) {
        in[i][0] = values[0];
        in[i][1] = 0;
    }

    fftw_plan p;
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(p);

    QList<double> fft;
    for (int i = 0; i < n; i++)
        fft.append(out[i][0]);

    fftw_destroy_plan(p);

    fftw_free(in);
    fftw_free(out);

    return fft;
}