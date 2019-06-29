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


#ifndef __AUDIOVISUAL__CONFIG_CONFIG__H
#define __AUDIOVISUAL__CONFIG_CONFIG__H

#include <QtCore/QString>
#include <QtMultimedia/QAudioFormat>

class Config {

private:
    Config();

    static Config *instance;

    QString audioDevice;
    int audioChannels;
    int audioSampleRate;
    int audioSampleSize;
    QAudioFormat::SampleType audioSampleType;

    QString audioOutputDevice;
    int audioOutputChannels;
    int audioOutputSampleRate;
    int audioOutputSampleSize;
    QAudioFormat::SampleType audioOutputSampleType;

public:
    static Config *getInstance();

    const QString &getAudioDevice() const;

    void setAudioDevice(const QString &value);

    int getAudioChannels() const;

    void setAudioChannels(int value);

    int getAudioSampleRate() const;

    void setAudioSampleRate(int value);

    int getAudioSampleSize() const;

    void setAudioSampleSize(int value);

    QAudioFormat::SampleType getAudioSampleType() const;

    void setAudioSampleType(QAudioFormat::SampleType value);

    const QString &getAudioOutputDevice() const;

    void setAudioOutputDevice(const QString &audioOutputDevice);

    int getAudioOutputChannels() const;

    void setAudioOutputChannels(int audioOutputChannels);

    int getAudioOutputSampleRate() const;

    void setAudioOutputSampleRate(int audioOutputSampleRate);

    int getAudioOutputSampleSize() const;

    void setAudioOutputSampleSize(int audioOutputSampleSize);

    QAudioFormat::SampleType getAudioOutputSampleType() const;

    void setAudioOutputSampleType(QAudioFormat::SampleType audioOutputSampleType);
};

#endif
