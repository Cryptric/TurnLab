//
// Created by gawain on 9/6/25.
//

#include <QApplication>
#include <QPushButton>

#include "view/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication qtApp(argc, argv);

    MainWindow window;
    window.show();

    return qtApp.exec();
}
