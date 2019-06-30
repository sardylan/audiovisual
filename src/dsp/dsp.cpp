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


#include <QtCore/QtMath>
#include <QtCore/QList>

#include "dsp.hpp"

DSP::DSP() = default;

DSP::~DSP() = default;

QList<double> DSP::multiply(QList<double> signal, QList<double> beat) {
    QList<double> output;

    for (int i = 0; i < signal.length() && i < beat.length(); i++)
        output.append(signal[i] * beat[i]);

    return output;
}

QList<double> DSP::generateSine(int sampleRate, unsigned int frequency, double phase, int len) {
    const double pulse = 2 * M_PI * frequency;

    QList<double> sine;

    for (int i = 0; i < len; i++) {
        double angle = pulse * ((double) i / sampleRate);
        sine.append(qSin(angle + phase));
    }

    return sine;
}

double DSP::getPhaseForNextGeneration(QList<double> signal) {
    const double lastItem = signal.last();
    return qAsin(lastItem);
}
