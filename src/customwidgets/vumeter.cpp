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

#include "vumeter.hpp"

#include <QPainter>
#include <QPaintEvent>

VUMeter::VUMeter(QWidget *parent) : QOpenGLWidget(parent) {
    setAutoFillBackground(false);

    min = 0;
    max = 0;
    interval = 0;
    value = 0;
}

VUMeter::~VUMeter() {
}

double VUMeter::getMin() const {
    return min;
}

double VUMeter::getMax() const {
    return max;
}

double VUMeter::getValue() const {
    return value;
}

void VUMeter::setMin(double min) {
    VUMeter::min = min;
    updateInterval();
}

void VUMeter::setMax(double max) {
    VUMeter::max = max;
    updateInterval();
}

void VUMeter::setValue(double value) {
    VUMeter::value = value;

    update();
}

void VUMeter::updateInterval() {
    interval = max - min;
}

void VUMeter::paintEvent(QPaintEvent *event) {
    QBrush whiteBrush = QBrush(Qt::white);
    QBrush blackBrush = QBrush(Qt::black);

    QPen whitePen = QPen(Qt::white);
    whitePen.setWidth(0);

    QPen blackPen = QPen(Qt::black);
    blackPen.setWidth(0);

    QPen greyPen = QPen(Qt::lightGray);
    greyPen.setWidth(0);

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), whiteBrush);
    painter.translate(0, 0);

    int width = painter.window().width();

    double pixelWidth = interval / width;

    for (int x = 0; x < width; x++) {
        painter.save();

        painter.setPen((pixelWidth * x) <= value ? blackPen : whitePen);
        painter.drawPoint(QPoint(x, 12));
        painter.drawPoint(QPoint(x, 13));
        painter.drawPoint(QPoint(x, 14));
        painter.drawPoint(QPoint(x, 15));
        painter.drawPoint(QPoint(x, 16));

        painter.setPen(greyPen);

        painter.restore();
    }

    painter.end();
}
