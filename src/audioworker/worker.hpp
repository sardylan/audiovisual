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


#ifndef __AUDIOWORKER__WORKER__H
#define __AUDIOWORKER__WORKER__H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>

#include <fft1d.hpp>
#include <QtMultimedia/QAudioOutput>


class AudioWorker : public QObject {
Q_OBJECT

public:

    explicit AudioWorker(QObject *parent = nullptr);

    ~AudioWorker() override;

    const QAudioDeviceInfo &getDeviceInfo() const;

    void setDeviceInfo(const QAudioDeviceInfo &value);

    const QAudioFormat &getFormat() const;

    void setFormat(const QAudioFormat &value);

    double getGain() const;

    const QAudioDeviceInfo &getDeviceOutputInfo() const;

    void setDeviceOutputInfo(const QAudioDeviceInfo &value);

    const QAudioFormat &getOutputFormat() const;

    void setOutputFormat(const QAudioFormat &value);

    bool isBfoEnabled() const;

    unsigned int getBfoFrequency() const;

public slots:

    void start();

    void stop();

    void setGain(double value);

    void setBfoEnabled(bool value);

    void setBfoFrequency(unsigned int value);

private:
    QAudioDeviceInfo deviceInfo;
    QAudioFormat format;

    QAudioInput *audioInput;
    QIODevice *ioDevice;

    QThread *audioThread;
    QByteArray rawData;

    QAudioDeviceInfo deviceOutputInfo;
    QAudioFormat outputFormat;

    QAudioOutput *audioOutput;
    QIODevice *ioOutputDevice;

    QThread *audioOutputThread;

    FFT1D *fft1D;
    unsigned int fftSize;

    bool bfoEnabled;
    unsigned int bfoFrequency;
    double bfoAngle;

    double gain;

    void parsePayload(const QByteArray &payloadData);

    void computeRMS(QList<double> &values);

    void computeFFT(QList<double> &values);

    void sendOutputAudio(const QList<double> &value);

    void processBFO(QList<double> &values);

private slots:

    void readAvailableData();

signals:

    void newStatus(bool value);

    void newMaxFrequency(const unsigned int &maxFrequency);

    void newAudioData(const QByteArray &data);

    void newAudioRms(const double &rms);

    void newAudioFFT(const QList<double> &fft);

    void newBfoStatus(const bool &status);

    void newBfoFrequency(const unsigned int &frequency);
};

#endif
