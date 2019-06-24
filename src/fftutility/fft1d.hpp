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


#ifndef __FFTUTILITY__FFT__H
#define __FFTUTILITY__FFT__H

#include <QtCore/QSemaphore>

#ifdef __cplusplus
extern "C" {
#endif

#include <complex.h>
#include <fftw3.h>

#ifdef __cplusplus
}
#endif

class FFT1D {

public:

    FFT1D(const unsigned int &size, const double &inputMaxValue, const double &outputMaxValue);

    ~FFT1D();

    QList<double> execute(const QList<double> &data);

private:
    const unsigned int size;
    const double inputMaxValue;
    const double outputMaxValue;

    double *input;
    double *output;

    fftw_plan plan;

    QSemaphore fftLock;
};

#endif
