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

#include <complex>
#include <fftw3.h>

class FFT1D {

public:

    FFT1D(const unsigned int &size);

    ~FFT1D();

    QList<double> execute(const QList<double> &data) const;

    unsigned int getMax() const;

    void setMax(unsigned int max);

private:
    const unsigned int size;
    unsigned int max;

    double *input;
    fftw_complex *output;

    fftw_plan plan;

};

#endif
