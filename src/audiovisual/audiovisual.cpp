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

#include "audiovisual.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    AudioVisual qFM1000(argc, argv);
    qFM1000.prepare();
    return qFM1000.run();
}

AudioVisual::AudioVisual(int &argc, char **argv) : QApplication(argc, argv) {
    mainWindow = new MainWindow();
    configWindow = new ConfigWindow();
}

AudioVisual::~AudioVisual() {
    delete configWindow;
    delete mainWindow;
}

void AudioVisual::prepare() {
    connect(mainWindow, &MainWindow::showConfiguration, this, &AudioVisual::showConfiguration);
}

int AudioVisual::run() {
    mainWindow->show();

    return QApplication::exec();
}

void AudioVisual::showConfiguration() {
    configWindow->exec();
}
