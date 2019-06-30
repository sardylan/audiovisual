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


#ifndef __AUDIOVISUAL__CUSTOMWIDGETS_WATERFALL__H
#define __AUDIOVISUAL__CUSTOMWIDGETS_WATERFALL__H

#include <QOpenGLWidget>
#include <QList>
#include <QRgb>

#define CUSTOMWIDGETS_WATERFALL_DATA_MAX 1024

class Waterfall : public QOpenGLWidget {
Q_OBJECT

public:
    explicit Waterfall(QWidget *parent = nullptr);

    ~Waterfall() override;

    unsigned int getMaxFrequency() const;

    bool isBfoEnabled() const;

    unsigned int getBfoFrequency() const;

public slots:

    void addData(QList<double> value);

    void setMaxFrequency(unsigned int value);

    void setBfoEnabled(bool value);

    void setBfoFrequency(unsigned int value);

protected:

    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void enterEvent(QEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    int width;
    int height;

    int mousePosX;
    bool showMousePos;

    QList<QList<double>> dataList;

    unsigned int maxFrequency;

    bool bfoEnabled;
    unsigned int bfoFrequency;

    void cleanDataList();

    static QColor computeRgbValue(int value);

    static QColor computeRgbValue(double value);

signals:

    void newClickFrequency(unsigned int frequency);
};

#endif
