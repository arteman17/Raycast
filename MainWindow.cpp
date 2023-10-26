#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    comboBox_(new QComboBox(this))
{
    resize(1200, 800);
    controller_.AddPolygon(Polygon({{-10, -10}, {-10, 1010}, {1930, 1010}, {1930, -10}}));
    comboBox_.addItem("polygons");
    comboBox_.addItem("light");
    comboBox_.addItem("static lights");
    comboBox_.setFixedSize(100, 30);

    setFocus();
    setMouseTracking(true);
    connect(&comboBox_, &QComboBox::currentTextChanged, [this]() {
        setFocus();
    });
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (comboBox_.currentText() == "polygons") {
        if (event->button() == Qt::LeftButton) {
            if (controller_.getNewPol()) {
                controller_.setNewPol(false);
                std::vector<QPointF> vertices;
                vertices.emplace_back(event->pos());
                controller_.AddPolygon(Polygon(vertices));
            } else {
                controller_.AddVertexToLastPolygon(event->pos());
            }
        }
        if (event->button() == Qt::RightButton) {
            controller_.setNewPol(true);
        }
    }
    if (comboBox_.currentText() == "static lights") {
        if (event->button() == Qt::LeftButton) {
            controller_.addStaticLights(event->pos());
        }
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen;
    pen.setWidthF(3);
    painter.fillRect(0, 0, 1920, 1000, Qt::black);

    pen.setColor(Qt::white);
    painter.setPen(pen);
    controller_.drawPolygons(painter);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    controller_.drawStaticLights(painter);

    if (comboBox_.currentText() == "light") {
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawEllipse(controller_.getLightSource(), 1.5, 1.5);
        controller_.drawLightAreas(painter);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (comboBox_.currentText() == "light") {
        controller_.setLightSource(event->pos());
        controller_.setLightSources(event->pos());
        for (int i = 0; i < controller_.getLightSources().size(); ++i) {
            update();
        }
    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (comboBox_.currentText() == "polygons") {
        if ((event->key() == Qt::Key_Z) && (event->modifiers() & Qt::ControlModifier)) {
            controller_.EraseLastVertexInLastPolygon();
            update();
        }
    }
    if (comboBox_.currentText() == "static lights") {
        if ((event->key() == Qt::Key_Z) && (event->modifiers() & Qt::ControlModifier)) {
            controller_.EraseLastStaticLights();
            update();
        }
    }
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}
