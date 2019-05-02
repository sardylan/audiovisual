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


#ifndef __AUDIOVISUAL__CUSTOMWIDGETS_VUMETER__H
#define __AUDIOVISUAL__CUSTOMWIDGETS_VUMETER__H

#include <QOpenGLWidget>

class VUMeter : public QOpenGLWidget {
Q_OBJECT

public:
    explicit VUMeter(QWidget *parent = nullptr);

    ~VUMeter() override;

    double getMin() const;

    double getMax() const;

    double getWarning() const;

    double getAlert() const;

    double getValue() const;

public slots:

    void setMin(double newValue);

    void setMax(double value);

    void setWarning(double value);

    void setAlert(double value);

    void setValue(double value);

protected:

    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

private:
    int width;
    int height;

    double min;
    double max;
    double warning;
    double alert;

    double value;

    double interval;

    float halfInterval;
    float valueThreshold;
    float warningThreshold;
    float alertThreshold;

    void updateInterval();

    void updateValueThreshold();

    void updateWarningThreshold();

    void updateAlertThreshold();
};

#endif
