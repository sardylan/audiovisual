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

    status = Status::getInstance();
    config = Config::getInstance();

    signalConnect();
    initUi();

    startTimer();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::signalConnect() {
    connect(ui->actionFileExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfig, &QAction::triggered, this, &MainWindow::showConfiguration);
}

void MainWindow::initUi() {
    vuMeter = new VUMeter(this);
    ui->vuMeterStackedWidget->addWidget(vuMeter);

    waterfall = new Waterfall(this);
    ui->waterfallWidget->addWidget(waterfall);

    vuMeter->setMin(0);
    vuMeter->setMax(1024);
    vuMeter->setWarning(512);
    vuMeter->setAlert(900);

}

void MainWindow::startTimer() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        vuMeter->setValue(qrand() % 1024);

        QList<double> data;
        for (int i = 0; i < 1024; i++)
            data.append(qrand() % 1024);
        waterfall->addData(data);
    });
    timer->setSingleShot(false);
    timer->start(1);
}
