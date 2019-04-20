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


#ifndef __AUDIOVISUAL__WINDOWS_MAIN__H
#define __AUDIOVISUAL__WINDOWS_MAIN__H

#include <QMainWindow>
#include <QTimer>
#include <vumeter.hpp>
#include <waterfall.hpp>
#include <status.hpp>
#include <config/config.hpp>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    Status *status;
    Config *config;

    VUMeter *vuMeter;
    Waterfall *waterfall;
    QTimer *timer;

    void signalConnect();

    void initUi();

    void startTimer();

signals:

    void showConfiguration();
};

#endif
