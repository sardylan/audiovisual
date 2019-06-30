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


#ifndef __AUDIOVISUAL__DSP__DSP__H
#define __AUDIOVISUAL__DSP__DSP__H

#include <QtCore/QList>

class DSP {

public:

    DSP();

    ~DSP();

    static const QList<double> multiply(QList<double> &signal, QList<double> &beat);

    static const QList<double> generateSine(int sampleRate, unsigned int frequency, double phase, int len);

    static const double getPhaseForNextGeneration(QList<double> &signal);
};

#endif
