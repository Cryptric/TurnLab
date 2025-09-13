//
// Created by gawain on 9/6/25.
//

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "presenter/MainPresenter.h"

int main(int argc, char *argv[]) {
    QApplication qtApp(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("TurnLab - A simple CAM system for CNC lathes. Import DXF profiles, configure machining operations (turning, facing, threading, drilling, parting off, and contouring), and generate G-code with customizable Python post-processors.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption inputDXFOption("i", "Input DXF file", "filename", "");
    parser.addOption(inputDXFOption);

    parser.process(qtApp);

    QString inputDXF = parser.value(inputDXFOption);

    std::unique_ptr<MainPresenter> mainPresenter;
    if (!inputDXF.isEmpty()) {
        mainPresenter = std::make_unique<MainPresenter>(inputDXF.toStdString());
    } else {
        mainPresenter = std::make_unique<MainPresenter>();
    }



    return qtApp.exec();
}
