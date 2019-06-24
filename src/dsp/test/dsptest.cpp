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

#define DSPTEST_DURATION_MS 1000
#define DSPTEST_SAMPLE_RATE 96000
#define DSPTEST_SAMPLE_SIZE 24
#define DSPTEST_TONE_FREQUENCY_SIGNAL 17200
#define DSPTEST_TONE_FREQUENCY_BEAT 16500
#define DSPTEST_OUTPUT_MAX_VALUE 1024

void DSPTest::testDsp() {
    QList<double> signal;
    QList<double> beat;
    QList<double> input;
    QList<double> expected;
    QList<double> actual;

    const double signalPulse = 2 * M_PI * DSPTEST_TONE_FREQUENCY_SIGNAL;
    const double beatPulse = 2 * M_PI * DSPTEST_TONE_FREQUENCY_BEAT;

    const auto samples = (const unsigned int) (((double) DSPTEST_SAMPLE_RATE / 1000) * DSPTEST_DURATION_MS);
    const auto sampleSize = (double) qPow(2, DSPTEST_SAMPLE_SIZE) / 2;

    qDebug() << "Computing input signal";

    for (unsigned int i = 0; i < samples; i++) {
        double angle = signalPulse * ((double) i / DSPTEST_SAMPLE_RATE);
        signal.append(qSin(angle) * sampleSize);
    }

    qDebug() << "Computing beat signal";

    for (unsigned int i = 0; i < samples; i++) {
        double angle = beatPulse * ((double) i / DSPTEST_SAMPLE_RATE);
        beat.append(qSin(angle) * sampleSize);
    }

    qDebug() << "Multiply signals";

    for (unsigned int i = 0; i < samples; i++)
        input.append((signal[i] * beat[i]) / sampleSize);

    qDebug() << "Computing FFT";

    FFT1D fft1D((const unsigned int) samples, sampleSize, DSPTEST_OUTPUT_MAX_VALUE);

    actual = fft1D.execute(input);

    qDebug() << "Checking";

    const double factor = (double) 1000 / DSPTEST_DURATION_MS;

    for (unsigned int i = 0; i < (samples / 2); i++) {
        if (i == (unsigned int) ((DSPTEST_TONE_FREQUENCY_SIGNAL - DSPTEST_TONE_FREQUENCY_BEAT) / factor)
            || i == (unsigned int) (DSPTEST_TONE_FREQUENCY_SIGNAL / factor)
            || i == (unsigned int) ((DSPTEST_TONE_FREQUENCY_SIGNAL + DSPTEST_TONE_FREQUENCY_BEAT) / factor))
            qDebug() << QString("- at %1 Hz value is %2").arg(i * factor).arg((int) actual[i]).toStdString().c_str();

        if (i == (unsigned int) ((DSPTEST_TONE_FREQUENCY_SIGNAL - DSPTEST_TONE_FREQUENCY_BEAT) / factor)
            || i == (unsigned int) ((DSPTEST_TONE_FREQUENCY_SIGNAL + DSPTEST_TONE_FREQUENCY_BEAT) / factor)) {
            QVERIFY2((unsigned int) actual[i] > 0,
                     QString("Error at %1. Value is %2").arg(i).arg(actual[i]).toStdString().c_str());
        } else {
            QVERIFY2((unsigned int) actual[i] == 0,
                     QString("Error at %1. Value is %2").arg(i).arg(actual[i]).toStdString().c_str());
        }
    }
}
