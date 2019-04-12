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

#include "waterfall.hpp"

#include <QPainter>
#include <QPaintEvent>

WaterFall::WaterFall(QWidget *parent) : QOpenGLWidget(parent) {
    setAutoFillBackground(false);
}

WaterFall::~WaterFall() = default;

void WaterFall::paintEvent(QPaintEvent *event) {
    QBrush whiteBrush = QBrush(Qt::white);
    QBrush blackBrush = QBrush(Qt::black);

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), whiteBrush);
    painter.translate(0, 0);

    int width = painter.window().width();
    int height = painter.window().height();

//    double pixelWidth = interval / width;
//
//    double valueThreshold = value / pixelWidth;
//    double warningThreshold = warning / pixelWidth;
//    double alertThreshold = alert / pixelWidth;

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<quint8> line = dataList.at(y);


        for (int x = 0; x < width; x++) {
            QPen pen;
            pen.setColor(QColor::fromRgb(0, 0, 0));
            pen.setWidth(1);

            painter.setPen(pen);
            painter.drawLine(x, 0, x, height);
            painter.drawPoint(x, y);
        }
    }

    painter.end();
}

void WaterFall::addData(QList<quint8> data) {
    if (data.length() != WATERFALL_DATA_SIZE)
        return;

    dataList.append(data);
    cleanDataList();

    update();
}

void WaterFall::cleanDataList() {
    QPainter painter;
    painter.begin(this);

    while (dataList.length() > painter.window().height())
        dataList.removeFirst();

    painter.end();
}

void WaterFall::resample(const double *in, size_t in_ln, double *out, size_t out_ln) {
    double step = (double) in_ln / (double) out_ln;

    for (size_t i = 0; i < out_ln; i++) {
        double start = step * i;
        double end = step * (i + 1);

        int first = (int) start;
        int last = (int) end;

        double v = 0;

        for (int j = first; j <= last; j++) {
            double coef = 1;

            if (j == first)
                coef = 1 - (start - first);
            else if (j == last)
                coef = (end - last);

            v += in[j] * coef;
        }

        v /= step;
        *(out + i) = v;
    }
}
