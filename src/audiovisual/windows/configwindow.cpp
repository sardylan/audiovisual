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

#include <QComboBox>
#include <QtMultimedia/QAudioDeviceInfo>

#include "configwindow.hpp"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);

    initUi();
}

ConfigWindow::~ConfigWindow() {
    delete ui;
}

void ConfigWindow::initUi() const {
    initDeviceList();

    connect(ui->audioDeviceComboBox,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &ConfigWindow::updateSample);
}

void ConfigWindow::initDeviceList() const {
    ui->audioDeviceComboBox->clear();
    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        ui->audioDeviceComboBox->addItem(audioDeviceInfo.deviceName(), QVariant::fromValue(audioDeviceInfo));
}

void ConfigWindow::updateSample(const QString &deviceName) {
    const QAudioDeviceInfo &audioDeviceInfo = ui->audioDeviceComboBox->currentData().value<QAudioDeviceInfo>();

    ui->audioChannelsComboBox->clear();
    for (int channelCount: audioDeviceInfo.supportedChannelCounts())
        ui->audioChannelsComboBox->addItem(QString("%1").arg(channelCount), QVariant::fromValue(channelCount));

    ui->audioSampleRateComboBox->clear();
    for (int sampleRate: audioDeviceInfo.supportedSampleRates())
        ui->audioSampleRateComboBox->addItem(QString("%1 Hz").arg(sampleRate), QVariant::fromValue(sampleRate));

    ui->audioSampleSizeComboBox->clear();
    for (int sampleSize: audioDeviceInfo.supportedSampleSizes())
        ui->audioSampleSizeComboBox->addItem(QString("%1 bit").arg(sampleSize), QVariant::fromValue(sampleSize));

    ui->audioSampleTypeComboBox->clear();
    for (QAudioFormat::SampleType sampleType: audioDeviceInfo.supportedSampleTypes()) {
        QString text;

        switch (sampleType) {
            case QAudioFormat::Unknown:
                text = "Unknown";
                break;
            case QAudioFormat::SignedInt:
                text = "Signed";
                break;
            case QAudioFormat::UnSignedInt:
                text = "Unsigned";
                break;
            case QAudioFormat::Float:
                text = "Float";
                break;
        }

        ui->audioSampleTypeComboBox->addItem(text, QVariant::fromValue(sampleType));
    }
}
