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

#include <QtCore/QDebug>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    vuMeter = new VUMeter(this);
    initVUMeter();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        int val = qrand() % 1024;
        vuMeter->setValue(val);
    });
    timer->setSingleShot(false);
    timer->start(10);

    vuMeter->setMin(0);
    vuMeter->setMax(1024);
    vuMeter->setWarning(512);
    vuMeter->setAlert(900);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initVUMeter() {
    ui->vuMeterStackedWidget->addWidget(vuMeter);
}