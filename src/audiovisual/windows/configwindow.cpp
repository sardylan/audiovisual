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
#include <QtWidgets/QPushButton>
#include <QtMultimedia/QAudioDeviceInfo>
#include <config/manager.hpp>

#include "configwindow.hpp"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);

    status = Status::getInstance();
    config = Config::getInstance();

    signalConnect();
    initUi();
    load();
    checkStatus();
}

ConfigWindow::~ConfigWindow() {
    delete ui;
}

void ConfigWindow::signalConnect() {
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ConfigWindow::handleOK);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &ConfigWindow::handleApply);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &ConfigWindow::handleClose);
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &ConfigWindow::handleReset);

    connect(ui->audioDeviceComboBox,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &ConfigWindow::updateDeviceOptions);

    connect(ui->audioOutputDeviceComboBox,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &ConfigWindow::updateOutputDeviceOptions);
}

void ConfigWindow::initUi() {
    ui->audioDeviceComboBox->clear();
    ui->audioDeviceComboBox->addItem("");

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        ui->audioDeviceComboBox->addItem(audioDeviceInfo.deviceName(), QVariant::fromValue(audioDeviceInfo));

    ui->audioOutputDeviceComboBox->clear();
    ui->audioOutputDeviceComboBox->addItem("");

    for (const QAudioDeviceInfo &audioDeviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        ui->audioOutputDeviceComboBox->addItem(audioDeviceInfo.deviceName(), QVariant::fromValue(audioDeviceInfo));
}

void ConfigWindow::load() {
    ConfigManager::load();

    int index = -1;
    for (int i = 0; i < ui->audioDeviceComboBox->count(); i++)
        if (ui->audioDeviceComboBox->itemData(i).value<QAudioDeviceInfo>().deviceName() == config->getAudioDevice()) {
            index = i;
            break;
        }

    if (index != -1) {
        ui->audioDeviceComboBox->setCurrentIndex(index);
    } else {
        ui->audioDeviceComboBox->setItemText(0, QString("%1 [NOT PRESENT]").arg(config->getAudioDevice()));
        ui->audioDeviceComboBox->setCurrentIndex(0);
    }

    updateDeviceOptions();

    index = -1;
    for (int i = 0; i < ui->audioOutputDeviceComboBox->count(); i++)
        if (ui->audioOutputDeviceComboBox->itemData(i).value<QAudioDeviceInfo>().deviceName() ==
            config->getAudioOutputDevice()) {
            index = i;
            break;
        }

    if (index != -1) {
        ui->audioOutputDeviceComboBox->setCurrentIndex(index);
    } else {
        ui->audioOutputDeviceComboBox->setItemText(0, QString("%1 [NOT PRESENT]").arg(config->getAudioOutputDevice()));
        ui->audioOutputDeviceComboBox->setCurrentIndex(0);
    }

    updateOutputDeviceOptions();
}

void ConfigWindow::save() {
    config->setAudioDevice(ui->audioDeviceComboBox->currentData().value<QAudioDeviceInfo>().deviceName());
    config->setAudioChannels(ui->audioChannelsComboBox->currentData().value<int>());
    config->setAudioSampleRate(ui->audioSampleRateComboBox->currentData().value<int>());
    config->setAudioSampleSize(ui->audioSampleSizeComboBox->currentData().value<int>());
    config->setAudioSampleType(ui->audioSampleTypeComboBox->currentData().value<QAudioFormat::SampleType>());

    config->setAudioOutputDevice(ui->audioOutputDeviceComboBox->currentData().value<QAudioDeviceInfo>().deviceName());
    config->setAudioOutputChannels(ui->audioOutputChannelsComboBox->currentData().value<int>());
    config->setAudioOutputSampleRate(ui->audioOutputSampleRateComboBox->currentData().value<int>());
    config->setAudioOutputSampleSize(ui->audioOutputSampleSizeComboBox->currentData().value<int>());
    config->setAudioOutputSampleType(
            ui->audioOutputSampleTypeComboBox->currentData().value<QAudioFormat::SampleType>());

    ConfigManager::save();
}

void ConfigWindow::checkStatus() {
    bool enabled = !status->isRunning();

    ui->audioDeviceComboBox->setEnabled(enabled);
    ui->audioChannelsComboBox->setEnabled(enabled);
    ui->audioSampleRateComboBox->setEnabled(enabled);
    ui->audioSampleSizeComboBox->setEnabled(enabled);
    ui->audioSampleTypeComboBox->setEnabled(enabled);

    ui->audioOutputDeviceComboBox->setEnabled(enabled);
    ui->audioOutputChannelsComboBox->setEnabled(enabled);
    ui->audioOutputSampleRateComboBox->setEnabled(enabled);
    ui->audioOutputSampleSizeComboBox->setEnabled(enabled);
    ui->audioOutputSampleTypeComboBox->setEnabled(enabled);
}

void ConfigWindow::handleOK() {
    save();
    close();
}

void ConfigWindow::handleApply() {
    save();
}

void ConfigWindow::handleClose() {
    close();
}

void ConfigWindow::handleReset() {
    load();
}

void ConfigWindow::updateDeviceOptions() {
    const QAudioDeviceInfo &audioDeviceInfo = ui->audioDeviceComboBox->currentData().value<QAudioDeviceInfo>();

    ui->audioChannelsComboBox->clear();
    for (int channelCount: audioDeviceInfo.supportedChannelCounts())
        ui->audioChannelsComboBox->addItem(QString("%1").arg(channelCount), QVariant::fromValue(channelCount));

    for (int i = 0; i < ui->audioChannelsComboBox->count(); i++)
        if (ui->audioChannelsComboBox->itemData(i).value<int>() == config->getAudioChannels()) {
            ui->audioChannelsComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioSampleRateComboBox->clear();
    for (int sampleRate: audioDeviceInfo.supportedSampleRates())
        ui->audioSampleRateComboBox->addItem(QString("%1 Hz").arg(sampleRate), QVariant::fromValue(sampleRate));

    for (int i = 0; i < ui->audioSampleRateComboBox->count(); i++)
        if (ui->audioSampleRateComboBox->itemData(i).value<int>() == config->getAudioSampleRate()) {
            ui->audioSampleRateComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioSampleSizeComboBox->clear();
    for (int sampleSize: audioDeviceInfo.supportedSampleSizes())
        ui->audioSampleSizeComboBox->addItem(QString("%1 bit").arg(sampleSize), QVariant::fromValue(sampleSize));

    for (int i = 0; i < ui->audioSampleSizeComboBox->count(); i++)
        if (ui->audioSampleSizeComboBox->itemData(i).value<int>() == config->getAudioSampleSize()) {
            ui->audioSampleSizeComboBox->setCurrentIndex(i);
            break;
        }

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

    for (int i = 0; i < ui->audioSampleTypeComboBox->count(); i++)
        if (ui->audioSampleTypeComboBox->itemData(i).value<QAudioFormat::SampleType>() ==
            config->getAudioSampleType()) {
            ui->audioSampleTypeComboBox->setCurrentIndex(i);
            break;
        }
}

void ConfigWindow::updateOutputDeviceOptions() {
    const QAudioDeviceInfo &audioDeviceInfo = ui->audioOutputDeviceComboBox->currentData().value<QAudioDeviceInfo>();

    ui->audioOutputChannelsComboBox->clear();
    for (int channelCount: audioDeviceInfo.supportedChannelCounts())
        ui->audioOutputChannelsComboBox->addItem(QString("%1").arg(channelCount), QVariant::fromValue(channelCount));

    for (int i = 0; i < ui->audioOutputChannelsComboBox->count(); i++)
        if (ui->audioOutputChannelsComboBox->itemData(i).value<int>() == config->getAudioChannels()) {
            ui->audioOutputChannelsComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioOutputSampleRateComboBox->clear();
    for (int sampleRate: audioDeviceInfo.supportedSampleRates())
        ui->audioOutputSampleRateComboBox->addItem(QString("%1 Hz").arg(sampleRate), QVariant::fromValue(sampleRate));

    for (int i = 0; i < ui->audioOutputSampleRateComboBox->count(); i++)
        if (ui->audioOutputSampleRateComboBox->itemData(i).value<int>() == config->getAudioSampleRate()) {
            ui->audioOutputSampleRateComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioOutputSampleSizeComboBox->clear();
    for (int sampleSize: audioDeviceInfo.supportedSampleSizes())
        ui->audioOutputSampleSizeComboBox->addItem(QString("%1 bit").arg(sampleSize), QVariant::fromValue(sampleSize));

    for (int i = 0; i < ui->audioOutputSampleSizeComboBox->count(); i++)
        if (ui->audioOutputSampleSizeComboBox->itemData(i).value<int>() == config->getAudioSampleSize()) {
            ui->audioOutputSampleSizeComboBox->setCurrentIndex(i);
            break;
        }

    ui->audioOutputSampleTypeComboBox->clear();
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

        ui->audioOutputSampleTypeComboBox->addItem(text, QVariant::fromValue(sampleType));
    }

    for (int i = 0; i < ui->audioOutputSampleTypeComboBox->count(); i++)
        if (ui->audioOutputSampleTypeComboBox->itemData(i).value<QAudioFormat::SampleType>() ==
            config->getAudioSampleType()) {
            ui->audioOutputSampleTypeComboBox->setCurrentIndex(i);
            break;
        }
}
