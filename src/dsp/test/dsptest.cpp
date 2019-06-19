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

#include "dsptest.hpp"

#include <fft1d.hpp>

QTEST_MAIN(DSPTest)

#define DSPTEST_SAMPLE_RATE 96000
#define DSPTEST_TONE_FREQUENCY_SIGNAL 17200
#define DSPTEST_TONE_FREQUENCY_BEAT 16500

void DSPTest::testDsp() {
    QList<double> signal;
    QList<double> beat;
    QList<double> input;
    QList<double> expected;
    QList<double> actual;

    double samplesPerPeriod;

    qDebug() << "Computing input signal";

    samplesPerPeriod = (double) DSPTEST_SAMPLE_RATE / DSPTEST_TONE_FREQUENCY_SIGNAL;
    for (unsigned int i = 0; i < DSPTEST_SAMPLE_RATE; i++) {
        double angle = (2 * M_PI) * ((double) i / samplesPerPeriod);
        signal.append(qSin(angle));
    }

    qDebug() << "Computing beat signal";

    samplesPerPeriod = (double) DSPTEST_SAMPLE_RATE / DSPTEST_TONE_FREQUENCY_BEAT;
    for (unsigned int i = 0; i < DSPTEST_SAMPLE_RATE; i++) {
        double angle = (2 * M_PI) * ((double) i / samplesPerPeriod);
        beat.append(qSin(angle));
    }

    qDebug() << "Multiply signals";

    for (unsigned int i = 0; i < DSPTEST_SAMPLE_RATE; i++)
        input.append(signal[i] * beat[i]);

    qDebug() << "Computing FFT";

    FFT1D fft1D(DSPTEST_SAMPLE_RATE);
//    fft1D.setMax(1);

    actual = fft1D.execute(input);

    qDebug() << "Checking";

    for (unsigned int i = 0; i < DSPTEST_SAMPLE_RATE / 2; i++) {
        if (i == DSPTEST_TONE_FREQUENCY_SIGNAL - DSPTEST_TONE_FREQUENCY_BEAT
            || i == DSPTEST_TONE_FREQUENCY_SIGNAL + DSPTEST_TONE_FREQUENCY_BEAT)
            QVERIFY2(actual[i] > 0.0,
                     QString("Error at %1. Value is %2").arg(i).arg(actual[i]).toStdString().c_str());
        else
            QVERIFY2((int) actual[i] == 0.0,
                     QString("Error at %1. Value is %2").arg(i).arg(actual[i]).toStdString().c_str());
    }
}
