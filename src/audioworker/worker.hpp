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


#ifndef __AUDIOWORKER__WORKER__H
#define __AUDIOWORKER__WORKER__H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>


class AudioWorker : public QObject {
Q_OBJECT

public:

    explicit AudioWorker(QObject *parent = nullptr);

    ~AudioWorker() override;

    const QAudioDeviceInfo &getDeviceInfo() const;

    void setDeviceInfo(const QAudioDeviceInfo &value);

    const QAudioFormat &getFormat() const;

    void setFormat(const QAudioFormat &value);

public slots:

    void start();

    void stop();

private:
    QAudioDeviceInfo deviceInfo;
    QAudioFormat format;

    QAudioInput *audioInput;
    QIODevice *ioDevice;

    QThread *audioThread;

private slots:

    void readAvailableData();

signals:

    void newStatus(bool value);

    void newAudioData(const QByteArray &data);

};

#endif