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


#ifndef __AUDIOVISUAL__AUDIOVISUAL__H
#define __AUDIOVISUAL__AUDIOVISUAL__H

#include "version.hpp"

#include <QApplication>

#include <windows/mainwindow.hpp>
#include <windows/configwindow.hpp>
#include <worker.hpp>


class AudioVisual : public QApplication {
Q_OBJECT

public:

    explicit AudioVisual(int &argc, char **argv);

    ~AudioVisual() override;

    void prepare();

    int run();

private:
    Status *status;
    Config *config;

    AudioWorker *audioWorker;

    ConfigWindow *configWindow;
    MainWindow *mainWindow;

    double audioMaxValue;


private slots:

    void showConfiguration();

    void toggleRun(bool value);

    void newAudioData(const QByteArray& data);
};

#endif
