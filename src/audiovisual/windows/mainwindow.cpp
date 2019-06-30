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
#include <QtCore/QtMath>
#include <QtGui/QOpenGLContext>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    vuMeter = new VUMeter(this);
    waterfall = new Waterfall(this);

    signalConnect();
    initUi();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initUi() {
    ui->vuMeterStackedWidget->addWidget(vuMeter);
    ui->waterfallWidget->addWidget(waterfall);

    vuMeter->setMin(0);
    vuMeter->setMax(1024);
    vuMeter->setWarning(512);
    vuMeter->setAlert(900);

    ui->gainSlider->setMinimum(0);
    ui->gainSlider->setMaximum(10000);
    ui->gainSlider->setValue(100);

    newGain();
}

void MainWindow::signalConnect() {
    connect(ui->actionFileExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfig, &QAction::triggered, this, &MainWindow::showConfiguration);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::toggleRunning);

    connect(ui->gainSlider, &QAbstractSlider::valueChanged, this, &MainWindow::newGain);

    connect(ui->bfoCheckBox, &QCheckBox::stateChanged, this, &MainWindow::handleNewBfoState);
    connect(waterfall, &Waterfall::newClickFrequency, this, &MainWindow::handleNewBfoFrequency);
}

void MainWindow::updateRunning(bool value) {
    ui->actionRun->setChecked(value);
}

void MainWindow::updateVuMeterMax(const double &value) {
    vuMeter->setMin(0);
    vuMeter->setWarning(value * .5);
    vuMeter->setAlert(value * .75);
    vuMeter->setMax(value);
}

void MainWindow::updateVuMeter(const double &value) {
    vuMeter->setValue(value);
}

void MainWindow::updateWaterfall(const QList<double> &data) {
    waterfall->addData(data);
}

void MainWindow::updateWaterfallMaxFrequency(const unsigned int &maxFrequency) {
    waterfall->setMaxFrequency(maxFrequency);
}

void MainWindow::newGain() {

    double logValue = ((double) ui->gainSlider->value()) / ((double) ui->gainSlider->maximum() / 100);

    ui->gainValue->setText(QString().setNum(logValue, 'f', 2));

    emit newGainValue(logValue);
}

void MainWindow::handleNewBfoState(int state) {
    bool newState = false;

    if (state == Qt::Checked)
        newState = true;

    emit newBfoState(newState);
}

void MainWindow::handleNewBfoFrequency(unsigned int frequency) {
    ui->bfoFrequenctValue->setText(QString("%1").arg(frequency));
    emit newBfoFrequency(frequency);
}

void MainWindow::updateBfoStatus(const bool &value) {
    waterfall->setBfoEnabled(value);
}

void MainWindow::updateBfoFrequency(const unsigned int &frequency) {
    waterfall->setBfoFrequency(frequency);
}
