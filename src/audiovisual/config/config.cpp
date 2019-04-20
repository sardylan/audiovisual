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

#include "config.hpp"
#include "default.hpp"

Config *Config::instance = nullptr;

Config *Config::getInstance() {
    if (instance == nullptr)
        instance = new Config();

    return instance;
}

Config::Config() {
    audioDevice = CONFIG_AUDIO_DEVICE_DEFAULT;
    audioChannels = CONFIG_AUDIO_CHANNELS_DEFAULT;
    audioSampleRate = CONFIG_AUDIO_SAMPLE_RATE_DEFAULT;
    audioSampleSize = CONFIG_AUDIO_SAMPLE_SIZE_DEFAULT;
    audioSampleType = CONFIG_AUDIO_SAMPLE_TYPE_DEFAULT;
}

const QString &Config::getAudioDevice() const {
    return audioDevice;
}

void Config::setAudioDevice(const QString &value) {
    Config::audioDevice = value;
}

int Config::getAudioChannels() const {
    return audioChannels;
}

void Config::setAudioChannels(int value) {
    Config::audioChannels = value;
}

int Config::getAudioSampleRate() const {
    return audioSampleRate;
}

void Config::setAudioSampleRate(int value) {
    Config::audioSampleRate = value;
}

int Config::getAudioSampleSize() const {
    return audioSampleSize;
}

void Config::setAudioSampleSize(int value) {
    Config::audioSampleSize = value;
}

QAudioFormat::SampleType Config::getAudioSampleType() const {
    return audioSampleType;
}

void Config::setAudioSampleType(QAudioFormat::SampleType value) {
    Config::audioSampleType = value;
}
