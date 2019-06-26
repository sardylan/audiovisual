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
    setMouseTracking(true);

    width = 0;
    height = 0;

    showMousePos = false;
    mousePosX = 0;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);

    if (width == 0 || height == 0)
        return;

    const unsigned int interval = width / 96;
    const unsigned int intervalBig = interval * 3;

    for (int y = 0; y < height && y < dataList.size(); y++) {
        QList<double> lineList = dataList.at((dataList.size() - 1) - y);

        glBegin(GL_LINE_STRIP);

        glVertex2i(-1, y);

        for (int x = 0; x < lineList.size(); x++) {
            double value = lineList.at(x);
            QColor qColor = computeRgbValue(value);
            glColor3f(qColor.redF(), qColor.greenF(), qColor.blueF());
            glVertex2i(x, y);
        }

        glEnd();
    }

    for (int x = 0; x < width; x += interval) {
        glBegin(GL_LINES);

        if (x % intervalBig == 0)
            glColor4ub(64, 64, 64, 128);
        else
            glColor4ub(32, 32, 32, 128);

        glVertex2f(x, 0);
        glVertex2f(x, height);
        glEnd();
    }

    if (showMousePos) {
        glBegin(GL_LINES);
        glColor3f(128, 128, 128);
        glVertex2f(mousePosX, 0);
        glVertex2f(mousePosX, height);
        glEnd();
    }

    glBegin(GL_POINTS);
    glColor3f(255, 255, 255);
    glVertex2i(10, 10);
    glEnd();
}

void Waterfall::mouseMoveEvent(QMouseEvent *event) {
    mousePosX = event->pos().x();
    update();
}

void Waterfall::enterEvent(QEvent *event) {
    if (event->type() == QEvent::Enter) {
        showMousePos = true;
        update();
    }
}

void Waterfall::leaveEvent(QEvent *event) {
    if (event->type() == QEvent::Leave) {
        showMousePos = false;
        update();
    }
}

unsigned int Waterfall::getMaxFrequency() const {
    return maxFrequency;
}

void Waterfall::setMaxFrequency(unsigned int value) {
    Waterfall::maxFrequency = value;
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
