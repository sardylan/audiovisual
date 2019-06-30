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
#include <QtConcurrent/QtConcurrent>

#include <dsp.hpp>

#include "worker.hpp"

AudioWorker::AudioWorker(QObject *parent) : QObject(parent) {
    rawData.clear();

    fftSize = 0;
    gain = 1;

    bfoEnabled = false;
    bfoFrequency = 0;
    bfoAngle = 0;
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

const QAudioDeviceInfo &AudioWorker::getDeviceOutputInfo() const {
    return deviceOutputInfo;
}

void AudioWorker::setDeviceOutputInfo(const QAudioDeviceInfo &value) {
    AudioWorker::deviceOutputInfo = value;
}

const QAudioFormat &AudioWorker::getOutputFormat() const {
    return outputFormat;
}

void AudioWorker::setOutputFormat(const QAudioFormat &value) {
    outputFormat = value;
}

bool AudioWorker::isBfoEnabled() const {
    return bfoEnabled;
}

void AudioWorker::setBfoEnabled(bool value) {
    AudioWorker::bfoEnabled = value;
}

unsigned int AudioWorker::getBfoFrequency() const {
    return bfoFrequency;
}

void AudioWorker::setBfoFrequency(unsigned int value) {
    AudioWorker::bfoFrequency = value;
}

void AudioWorker::start() {
    fft1D = new FFT1D(fftSize, qPow(2, format.sampleSize()) / 2, 1024);

    audioThread = new QThread(this);
    audioOutputThread = new QThread(this);

    connect(audioThread, &QThread::finished, audioThread, &QThread::deleteLater);
    connect(audioOutputThread, &QThread::finished, audioOutputThread, &QThread::deleteLater);

    audioInput = new QAudioInput(deviceInfo, format, nullptr);
    audioOutput = new QAudioOutput(deviceOutputInfo, outputFormat, nullptr);

    audioInput->moveToThread(audioThread);
    audioOutput->moveToThread(audioOutputThread);

    ioDevice = audioInput->start();
    connect(ioDevice, &QIODevice::readyRead, this, &AudioWorker::readAvailableData);

    ioOutputDevice = audioOutput->start();

    audioThread->start();
    audioOutputThread->start();

    emit newMaxFrequency(format.sampleRate());

    emit newStatus(true);
}

void AudioWorker::stop() {
    disconnect(ioDevice, &QIODevice::readyRead, this, &AudioWorker::readAvailableData);

    audioInput->stop();
    audioInput->deleteLater();

    audioThread->quit();
    audioThread->deleteLater();

    audioOutput->stop();
    audioOutput->deleteLater();

    audioOutputThread->quit();
    audioOutputThread->deleteLater();

    emit newStatus(false);
}

void AudioWorker::readAvailableData() {
    int channels = format.channelCount();
    int bytes = format.sampleSize() / 8;
    int increment = channels * bytes;

    unsigned int shift = (fftSize) * increment;

    while (true) {
        char c;
        qint64 bytesRead = ioDevice->read(&c, 1);
        if (bytesRead <= 0)
            return;

        rawData.append(c);
        if (rawData.length() == shift)
            break;
    }

    if (rawData.length() != shift)
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

    QList<double> outputValues;

    if (bfoEnabled) {
        QList<double> beatValues = DSP::generateSine(format.sampleRate(), bfoFrequency, bfoAngle, values.length());
        bfoAngle = DSP::getPhaseForNextGeneration(beatValues);
        outputValues = DSP::multiply(values, beatValues);
    } else {
        outputValues = values;
    }

    QtConcurrent::run(this, &AudioWorker::sendOutputAudio, outputValues);

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

void AudioWorker::sendOutputAudio(const QList<double> &value) {
    QByteArray data;

    int bytesPerSample = format.sampleSize() / 8;

    char bytes[bytesPerSample];

    for (double v : value) {
        if (format.sampleSize() == 16
            && format.sampleType() == QAudioFormat::SignedInt
            && format.byteOrder() == QAudioFormat::LittleEndian) {
            auto b = static_cast<qint16>(v);
            qToLittleEndian<qint16>(b, bytes);
        } else
            continue;

        for (int c = 0; c < format.channelCount(); c++)
            for (int j = 0; j < bytesPerSample; j++)
                data.append(bytes[j]);
    }

    if (data.length() == 0)
        return;

    ioOutputDevice->write(data);
}
