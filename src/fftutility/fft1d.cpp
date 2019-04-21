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

#include <complex>
#include <fftw3.h>

#include "fft1d.hpp"

#define DEFAULT_MAX 1024

FFT1D::FFT1D(const unsigned int &size) : size(size) {
    max = DEFAULT_MAX;

    input = (double *) fftw_malloc(sizeof(double) * size);
    output = (fftw_complex *) fftw_malloc((sizeof(fftw_complex) * (size / 2) + 1));

    plan = fftw_plan_dft_r2c_1d(size, input, output, FFTW_MEASURE);
}

FFT1D::~FFT1D() {
    fftw_destroy_plan(plan);

    fftw_free(input);
    fftw_free(output);
}

unsigned int FFT1D::getMax() const {
    return max;
}

void FFT1D::setMax(unsigned int value) {
    FFT1D::max = value;
}

QList<double> FFT1D::execute(const QList<double> &data) const {
    for (int i = 0; i < size; i++)
        input[i] = data[i];

    fftw_execute(plan);

    QList<double> fft;
    for (int i = 0; i < size; i++)
        fft.append((qSqrt(qPow(output[i][0], 2) + qPow(output[i][1], 2)) / (size / 2)) * max);

    return fft;
}
