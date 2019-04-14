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
#include "utility.hpp"

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QRgb>

Waterfall::Waterfall(QWidget *parent) : QOpenGLWidget(parent) {
    setAutoFillBackground(false);
}

Waterfall::~Waterfall() = default;

void Waterfall::paintEvent(QPaintEvent *event) {
    QBrush whiteBrush = QBrush(Qt::white);
    QBrush blackBrush = QBrush(Qt::black);

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), whiteBrush);
    painter.translate(0, 0);

    int width = painter.window().width();
    int height = painter.window().height();

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<QColor> lineList = dataList.at((dataList.size() - 1) - y);

        for (int x = 0; x < width && x < lineList.size(); x++) {
            QPen pen;
            pen.setColor(lineList.at(x));
            pen.setWidth(1);

            painter.setPen(pen);
            painter.drawPoint(x, y);
        }
    }

    painter.end();
}

void Waterfall::addData(const QList<double> &value) {
    QPainter painter;
    painter.begin(this);
    int width = painter.window().width();
    painter.end();

    double rowData[width];
    CustomWidgetsUtility::resample(value.toVector().constData(), value.size(), rowData, width);

    QList<QColor> colorLine;

    for (const double &item : rowData) {
        QColor color = computeRgbValue(item);
        colorLine.append(color);
    }

    dataList.append(colorLine);
    cleanDataList();
    update();
}

void Waterfall::cleanDataList() {
    QPainter painter;
    painter.begin(this);
    int height = painter.window().height();
    painter.end();

    while (dataList.length() > height)
        dataList.removeFirst();
}

void Waterfall::resizeEvent(QResizeEvent *e) {
    dataList.clear();
    QOpenGLWidget::resizeEvent(e);
}

QColor Waterfall::computeRgbValue(int value) {
    int v = value;
    if (v < 0)
        v = 0;
    if (v > CUSTOMWIDGETS_WATERFALL_DATA_MAX)
        v = CUSTOMWIDGETS_WATERFALL_DATA_MAX;

    int r = 255;
    int g = 0;
    int b = 255;

    if (v >= 0 && v < 256) {
        r = 0;
        g = 0;
        b = v;
    } else if (v >= 256 && v < 512) {
        int k = v - 256;
        r = k;
        g = k;
        b = 255 - k;
    } else if (v >= 512 && v < 768) {
        int k = v - 512;
        r = 255;
        g = 255 - k;
        b = 0;
    } else if (v >= 768 && v < CUSTOMWIDGETS_WATERFALL_DATA_MAX) {
        int k = v - 768;
        r = 255;
        g = k;
        b = k;
    }

    return QColor::fromRgb(r, g, b);
}

QColor Waterfall::computeRgbValue(double value) {
    return computeRgbValue((int) value);
}