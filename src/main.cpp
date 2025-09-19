//
// Created by gawain on 9/6/25.
//

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <spdlog/spdlog.h>

#include "ProjectUtils.h"
#include "presenter/MainPresenter.h"

int main(int argc, char *argv[]) {
    QApplication qtApp(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("TurnLab - A simple CAM system for CNC lathes. Import DXF profiles, configure machining operations (turning, facing, threading, drilling, parting off, and contouring), and generate G-code with customizable Python post-processors.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption inputDXFOption("i", "Input DXF file", "filename", "");
    parser.addOption(inputDXFOption);

    QCommandLineOption inputProjectOption("p", "Input TurnLab project file", "filename", "");
    parser.addOption(inputProjectOption);

    QCommandLineOption logLevelOption("log-level", "Set log level (trace, debug, info, warn, error, critical, off)", "level", "info");
    parser.addOption(logLevelOption);

    parser.process(qtApp);

    QString inputDXF = parser.value(inputDXFOption);
    QString inputProject = parser.value(inputProjectOption);
    QString logLevel = parser.value(logLevelOption);

    // Set log level
    if (logLevel == "trace") {
        spdlog::set_level(spdlog::level::trace);
    } else if (logLevel == "debug") {
        spdlog::set_level(spdlog::level::debug);
    } else if (logLevel == "info") {
        spdlog::set_level(spdlog::level::info);
    } else if (logLevel == "warn") {
        spdlog::set_level(spdlog::level::warn);
    } else if (logLevel == "error") {
        spdlog::set_level(spdlog::level::err);
    } else if (logLevel == "critical") {
        spdlog::set_level(spdlog::level::critical);
    } else if (logLevel == "off") {
        spdlog::set_level(spdlog::level::off);
    } else {
        spdlog::warn("Unknown log level '{}', using 'info'", logLevel.toStdString());
        spdlog::set_level(spdlog::level::info);
    }

    std::unique_ptr<MainPresenter> mainPresenter;
    if (!inputProject.isEmpty()) {
        mainPresenter = std::make_unique<MainPresenter>(loadProject(inputProject.toStdString()).value());
    } else if (!inputDXF.isEmpty()) {
        mainPresenter = std::make_unique<MainPresenter>(inputDXF.toStdString());
    } else {
        mainPresenter = std::make_unique<MainPresenter>();
    }



    return qtApp.exec();
}
