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

#define WATERFALL_DATA_SIZE 2048

class Waterfall : public QOpenGLWidget {
Q_OBJECT

public:
    explicit Waterfall(QWidget *parent = nullptr);

    ~Waterfall() override;

    static void resample(const double *in, size_t in_ln, double *out, size_t out_ln);

public slots:

    void setDataMax(double value);

    void setDataSize(size_t value);

    void addData(const QList<double>& value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double dataMax;
    size_t dataSize;
    QList<QList<double>> dataList;

    void cleanDataList();
};

#endif
