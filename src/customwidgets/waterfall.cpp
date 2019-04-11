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

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), whiteBrush);
    painter.translate(0, 0);

    int width = painter.window().width();
    int height = painter.window().height();

    for (int x = 0; x < width; x++) {
        QPen pen;

//        if (x > valueThreshold)
//            pen = QPen(Qt::white);
//        else if (x < warningThreshold)
//            pen = QPen(Qt::green);
//        else if (x < alertThreshold)
//            pen = QPen(Qt::yellow);
//        else
//            pen = QPen(Qt::red);

        pen.setWidth(0);

        painter.setPen(pen);
        painter.drawLine(x, 0, x, height);
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

//int WaterFall::interpolate(int x, float a[], int n )
//{
//    if( x <= 0 )  return a[0];
//    if( x >= n - 1 )  return a[n-1];
//    int j = int(x);
//    return a[j] + (x - j) * (a[j+1] - a[j]);
//}
//
//// linear interpolate array a[] -> array b[]
//void inter1parray( float a[], int n, float b[], int m )
//{
//    float step = float( n - 1 ) / (m - 1);
//    for( int j = 0; j < m; j ++ ){
//        b[j] = interp1( j*step, a, n );
//    }
//}

void WaterFall::interpolate(const double *in, size_t in_ln, double *out, size_t out_ln) {
    size_t i;
    size_t j;
    size_t offset;
    double step;
    double v;
    double r;

    step = (double) in_ln / (double) out_ln;
    r = 0;

    for (i = 0; i < out_ln; i++) {
        v = 0;
        offset = ((int) step) * i;

        if (i > 0 && r > 0) {
            v += *(in + offset - 1) * (1 - r);
        }

        for (j = 0; j < (int) step; j++) {
            v += *(in + offset + j);
        }

        r = step - j;
        v += *(in + j) * r;

        v /= step;

        *(out + i) = v;
    }
}
