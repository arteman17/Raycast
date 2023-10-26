#include <QApplication>
#include "MainWindow.h"
#include "Ray.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
