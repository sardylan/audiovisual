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

#include <GL/gl.h>
#include <GL/glu.h>

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QRgb>

Waterfall::Waterfall(QWidget *parent) : QOpenGLWidget(parent) {
    width = 0;
    height = 0;
}

Waterfall::~Waterfall() = default;

void Waterfall::initializeGL() {
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_FLAT);
}

void Waterfall::resizeGL(int w, int h) {
    width = w;
    height = h;
}

void Waterfall::paintGL() {
    if (width == 0 || height == 0)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_POINTS);

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<double> lineList = dataList.at((dataList.size() - 1) - y);

        for (int x = 0; x < lineList.size(); x++) {
            float posX = (((float) x + 1) / ((float) lineList.size() / 2)) - 1;
            float posY = 1 - (((float) y + 1) / ((float) height / 2));

            double value = lineList.at(x);
            QColor qColor = computeRgbValue(value);
            glColor3f(qColor.redF(), qColor.greenF(), qColor.blueF());
            glVertex3f(posX, posY, 0);
        }
    }

    glEnd();
}

void Waterfall::addData(const QList<double> &value) {
    dataList.append(value);
    cleanDataList();
    update();
}

void Waterfall::cleanDataList() {
    while (dataList.length() > height)
        dataList.removeFirst();
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