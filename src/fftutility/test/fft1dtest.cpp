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

#include <iostream>
#include <math.h>

#include <QtCore/QList>

#include <fft1d.hpp>

#include "fft1dtest.hpp"

#define TEST_TONE_FREQUENCY 1000
#define TEST_TONE_DURATION 2000
#define TEST_SAMPLE_RATE 48000
#define TEST_RESOLUTION 1

QTEST_MAIN(FFT1DTest)

void FFT1DTest::testComputeRow() {
    QList<double> input;
    QList<double> expected;
    QList<double> actual;

    double samplesPerMillisecond = (double) TEST_SAMPLE_RATE / 1000;
    unsigned long samples = (unsigned int) samplesPerMillisecond * TEST_TONE_DURATION;
    double samplesPerPeriod = (double) TEST_SAMPLE_RATE / TEST_TONE_FREQUENCY;

    for (unsigned long i = 0; i < samples; i++)
        input.append(qSin((i / samplesPerPeriod) * (2 * M_PI)));

    FFT1D fft1D(TEST_SAMPLE_RATE / TEST_RESOLUTION);

    actual = fft1D.execute(input);

    for (unsigned int i = 0; i < (TEST_SAMPLE_RATE / TEST_RESOLUTION) / 2; i++)
        printf("%u Hz\t%.06f\n", i * TEST_RESOLUTION, actual[i]);

    //    QCOMPARE(actual, expected);
}
