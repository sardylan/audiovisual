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

void VUMeter::setMin(double min) {
    VUMeter::min = min;
    updateInterval();
    update();
}

double VUMeter::getMax() const {
    return max;
}

void VUMeter::setMax(double max) {
    VUMeter::max = max;
    updateInterval();
    update();
}

double VUMeter::getWarning() const {
    return warning;
}

void VUMeter::setWarning(double warning) {
    VUMeter::warning = warning;
    updateInterval();
    update();
}

double VUMeter::getAlert() const {
    return alert;
}

void VUMeter::setAlert(double alert) {
    VUMeter::alert = alert;
    updateInterval();
    update();
}

double VUMeter::getValue() const {
    return value;
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

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), whiteBrush);
    painter.translate(0, 0);

    int width = painter.window().width();
    int height = painter.window().height();

    double pixelWidth = interval / width;

    double valueThreshold = value / pixelWidth;
    double warningThreshold = warning / pixelWidth;
    double alertThreshold = alert / pixelWidth;

    for (int x = 0; x < width; x++) {
        QPen pen;

        if (x > valueThreshold)
            pen = QPen(Qt::white);
        else if (x < warningThreshold)
            pen = QPen(Qt::green);
        else if (x < alertThreshold)
            pen = QPen(Qt::yellow);
        else
            pen = QPen(Qt::red);

        pen.setWidth(0);

        painter.setPen(pen);
        painter.drawLine(x, 0, x, height);
    }

    painter.end();
}
