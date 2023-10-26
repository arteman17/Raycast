#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QRadialGradient>
#include <QGradientStops>
#include <QPainterPath>
#include <QMouseEvent>

#include "Ray.h"
#include "Polygon.h"
#include "Controller.h"

class MainWindow : public QMainWindow {
        Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Controller controller_;
    QComboBox comboBox_;
};