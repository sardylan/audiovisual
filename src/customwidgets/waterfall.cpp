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
#include "waterfall_color_table.hpp"

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

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<double> lineList = dataList.at((dataList.size() - 1) - y);

        float posY = 1 - (((float) y + 1) / ((float) height / 2));

        glBegin(GL_LINE_STRIP);

        glVertex3f(-1, posY, 0);

        for (int x = 0; x < lineList.size(); x++) {
            float posX = (((float) x + 1) / ((float) lineList.size() / 2)) - 1;

            double value = lineList.at(x);
            QColor qColor = computeRgbValue(value);
            glColor3f(qColor.redF(), qColor.greenF(), qColor.blueF());
            glVertex3f(posX, posY, 0);
        }

        glEnd();
    }
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

    int r = waterfall_color_table[v][0];
    int g = waterfall_color_table[v][1];
    int b = waterfall_color_table[v][2];

    return QColor::fromRgb(r, g, b);
}

QColor Waterfall::computeRgbValue(double value) {
    return computeRgbValue((int) value);
}