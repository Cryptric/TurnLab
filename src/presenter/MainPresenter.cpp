//
// Created by gawain on 9/11/25.
//

#include "postprocessor/PythonPostProcessor.h"
#include "MainPresenter.h"

#include <QFileDialog>
#include <spdlog/spdlog.h>
#include <fstream>

#include "MachineConfigPresenter.h"
#include "ProjectUtils.h"
#include "../utils/ConfigurationManager.h"
#include "operation/FacingOperationPresenter.h"
#include "operation/PartingOperationPresenter.h"
#include "operation/TurningOperationPresenter.h"
#include "toolpath/ToolpathGenerator.h"

MainPresenter::MainPresenter() : machineConfig(ConfigurationManager::loadMachineConfig()), toolTable(ConfigurationManager::loadToolTable()), window(machineConfig, toolTable), toolpathPlotter(window.getGeometryView()) {
    window.show();
    connectSignals();
}

MainPresenter::MainPresenter(const std::string &inputDXF) : MainPresenter() {
    showDXFImportDialog(inputDXF);
}

MainPresenter::MainPresenter(Project project) : MainPresenter() {
    setProject(project);
}

void MainPresenter::connectSignals() {
    connect(&window, &MainWindow::onMachineConfigPressed, this, &MainPresenter::showMachineConfigDialog);
    connect(&window, &MainWindow::onLoadDXFPressed, this, [this]() { showDXFImportDialog(); });

    connect(&window, &MainWindow::onFacingPressed, this, &MainPresenter::onFacingPressed);
    connect(&window, &MainWindow::onTurningPressed, this, &MainPresenter::onTurningPressed);
    connect(&window, &MainWindow::onPartingPressed, this, &MainPresenter::onPartingPressed);

    connect(&window, &MainWindow::onGenerateGCodePressed, this, &MainPresenter::onGenerateGCodePressed);
}

void MainPresenter::showDXFImportDialog(std::string inputDXF) {
    if (inputDXF.empty()) {
        spdlog::info("Showing DXF import dialog");
        inputDXF = QFileDialog::getOpenFileName(nullptr, "Open DXF File", "", "DXF Files (*.dxf)").toStdString();
        if (inputDXF.empty()) {
            return;
        }
    }
    const DXFImportPresenter presenter(inputDXF, machineConfig, &window);
    if (const std::optional<Project> p = presenter.showDialog(); p.has_value()) {
        setProject(p.value());
    }
}

void MainPresenter::showCurrentOperation() {
    // Connect OK and Cancel button signals
    connect(currentOpConfigView.get(), &OperationConfigurationView::okPressed, this, &MainPresenter::onOperationConfigOkPressed);
    connect(currentOpConfigView.get(), &OperationConfigurationView::cancelPressed, this, &MainPresenter::onOperationConfigCancelPressed);

    window.replaceLeftPanel(currentOpConfigView.get());
    window.disableOperationButtons();
}

void MainPresenter::onFacingPressed() {
    spdlog::info("Facing pressed");
    currentOpConfigView = std::make_unique<OperationConfigurationView>(FacingOperationPresenter::visibility);
    currentOpConfigPresenter = std::make_unique<FacingOperationPresenter>(machineConfig, toolTable, *project, window.getGeometryView(), *currentOpConfigView);

    showCurrentOperation();
}

void MainPresenter::onTurningPressed() {
    spdlog::info("Turning pressed");
    currentOpConfigView = std::make_unique<OperationConfigurationView>(TurningOperationPresenter::visibility);
    currentOpConfigPresenter = std::make_unique<TurningOperationPresenter>(machineConfig, toolTable, *project, window.getGeometryView(), *currentOpConfigView);

    showCurrentOperation();
}

void MainPresenter::onPartingPressed() {
    spdlog::info("Parting pressed");
    currentOpConfigView = std::make_unique<OperationConfigurationView>(PartingOperationPresenter::visibility);
    currentOpConfigPresenter = std::make_unique<PartingOperationPresenter>(machineConfig, toolTable, *project, window.getGeometryView(), *currentOpConfigView);

    showCurrentOperation();
}

void MainPresenter::setProject(Project p) {
    project = std::make_unique<Project>(p);
    window.setProject(*project);
    if (!p.savePath.empty()) {
        saveProject(*project, p.savePath);
    }
    toolpaths.clear();
    for (const auto& op : p.operations) {
        toolpaths.push_back(ToolpathGenerator::generateToolpath(op, machineConfig));
    }
    toolpathPlotter.plotToolpaths(toolpaths);
}

void MainPresenter::showMachineConfigDialog() {
    spdlog::info("Showing machine config dialog");
    MachineConfigPresenter presenter(&window);
    MachineConfig updatedConfig = presenter.showDialog(machineConfig);

    // Save the updated configuration
    machineConfig = updatedConfig;
    ConfigurationManager::saveMachineConfig(machineConfig);
    spdlog::info("Machine configuration updated and saved");
}

void MainPresenter::onOperationConfigOkPressed() {
    spdlog::info("Operation configuration OK pressed");

    project->operations.push_back(currentOpConfigPresenter->getOperationConfiguration());
    saveProject(*project, project->savePath);
    window.setProject(*project);

    toolpaths.push_back(ToolpathGenerator::generateToolpath(currentOpConfigPresenter->getOperationConfiguration(), machineConfig));

    window.restoreLeftPanel();
    window.enableOperationButtons();
    toolpathPlotter.plotToolpaths(toolpaths);

    // Clear the current operation configuration view and presenter
    currentOpConfigView.reset();
    currentOpConfigPresenter.reset();
}

void MainPresenter::onGenerateGCodePressed() {
    spdlog::info("Generate GCode button pressed");

    if (!project || toolpaths.empty()) {
        spdlog::warn("No project or toolpaths available for GCode generation");
        return;
    }

    try {
        PythonPostProcessor postProcessor(machineConfig, toolTable);
        std::string gcode = postProcessor.generateGCode(toolpaths);

        if (!gcode.empty()) {
            // Show save dialog to user
            QString fileName = QFileDialog::getSaveFileName(
                &window,
                "Save NC File",
                QString(),
                "NC Files (*.nc)"
            );

            if (!fileName.isEmpty()) {
                std::ofstream file(fileName.toStdString());
                if (file.is_open()) {
                    file << gcode;
                    file.close();
                    spdlog::info("GCode saved to: {}", fileName.toStdString());
                } else {
                    spdlog::error("Failed to save GCode file: {}", fileName.toStdString());
                }
            }
        } else {
            spdlog::warn("Generated GCode is empty");
        }

    } catch (const std::exception& e) {
        spdlog::error("Error generating GCode: {}", e.what());
    }
}

void MainPresenter::onOperationConfigCancelPressed() {
    spdlog::info("Operation configuration Cancel pressed");

    // Restore the left panel and enable operation buttons without saving changes
    window.restoreLeftPanel();
    window.enableOperationButtons();

    // Clear the current operation configuration view and presenter
    currentOpConfigView.reset();
    currentOpConfigPresenter.reset();
}



