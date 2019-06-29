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


#ifndef __AUDIOVISUAL__CONFIG_DEFAULT__H
#define __AUDIOVISUAL__CONFIG_DEFAULT__H

#include <QtMultimedia/QAudioFormat>

#define CONFIG_AUDIO_DEVICE_DEFAULT ""
#define CONFIG_AUDIO_CHANNELS_DEFAULT 1
#define CONFIG_AUDIO_SAMPLE_RATE_DEFAULT 48000
#define CONFIG_AUDIO_SAMPLE_SIZE_DEFAULT 16
#define CONFIG_AUDIO_SAMPLE_TYPE_DEFAULT QAudioFormat::SignedInt

#define CONFIG_AUDIO_OUTPUT_DEVICE_DEFAULT ""
#define CONFIG_AUDIO_OUTPUT_CHANNELS_DEFAULT 1
#define CONFIG_AUDIO_OUTPUT_SAMPLE_RATE_DEFAULT 48000
#define CONFIG_AUDIO_OUTPUT_SAMPLE_SIZE_DEFAULT 16
#define CONFIG_AUDIO_OUTPUT_SAMPLE_TYPE_DEFAULT QAudioFormat::SignedInt

#endif
