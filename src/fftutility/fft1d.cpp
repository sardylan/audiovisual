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

#include "fft1d.hpp"

FFT1D::FFT1D(const unsigned int &size,
             const double &inputMaxValue,
             const double &outputMaxValue) :
        size(size),
        inputMaxValue(inputMaxValue),
        outputMaxValue(outputMaxValue),
        fftLock(1) {

    input = (double *) fftw_malloc(sizeof(double) * this->size);
    output = (double *) fftw_malloc(sizeof(double) * this->size);

    plan = fftw_plan_r2r_1d(this->size, this->input, this->output, FFTW_R2HC, FFTW_ESTIMATE);
}

FFT1D::~FFT1D() {
    fftw_destroy_plan(plan);

    fftw_free(input);
    fftw_free(output);
}

QList<double> FFT1D::execute(const QList<double> &data) {
    fftLock.acquire();

    for (int i = 0; i < size; i++)
        input[i] = data[i] / inputMaxValue;

    fftw_execute(plan);

    QList<double> fft;

    int samples = size / 2;

    for (int i = 0; i <= samples; i++) {
        double re = output[i];
        double im = output[size - i];
        double magnitude = qSqrt((re * re) + (im * im));
        double magnitudeUnity = magnitude / samples;

        double logArg = 1 + (magnitudeUnity * 9);
        double logValue = log10(logArg);
        double magnitudeNormalized = logValue * outputMaxValue;

        if (magnitudeNormalized > outputMaxValue) {
            qDebug() << i << re << im << magnitude << outputMaxValue << magnitudeNormalized;
            magnitudeNormalized = outputMaxValue;
        }

        fft.append(magnitudeNormalized);
    }

    fftLock.release();

    return fft;
}
