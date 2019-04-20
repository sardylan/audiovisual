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

#include <QtCore/QSettings>

#include "manager.hpp"
#include "config.hpp"
#include "default.hpp"

void ConfigManager::load() {
    Config *config = Config::getInstance();
    QSettings settings;

    settings.beginGroup("audio");
    config->setAudioDevice(settings.value("device", CONFIG_AUDIO_DEVICE_DEFAULT).toString());
    config->setAudioChannels(settings.value("channels", CONFIG_AUDIO_CHANNELS_DEFAULT).toInt());
    config->setAudioSampleRate(settings.value("sampleRate", CONFIG_AUDIO_SAMPLE_RATE_DEFAULT).toInt());
    config->setAudioSampleSize(settings.value("sampleSize", CONFIG_AUDIO_SAMPLE_SIZE_DEFAULT).toInt());
    config->setAudioSampleType(
            settings.value("sampleType", CONFIG_AUDIO_SAMPLE_TYPE_DEFAULT).value<QAudioFormat::SampleType>());
    settings.endGroup();
}

void ConfigManager::save() {
    Config *config = Config::getInstance();
    QSettings settings;

    settings.beginGroup("audio");
    settings.setValue("device", config->getAudioDevice());
    settings.setValue("channels", config->getAudioChannels());
    settings.setValue("sampleRate", config->getAudioSampleRate());
    settings.setValue("sampleSize", config->getAudioSampleSize());
    settings.setValue("sampleType", config->getAudioSampleType());
    settings.endGroup();

    settings.sync();
}
