//
// Created by gawain on 9/11/25.
//

#include "MainPresenter.h"

#include <QFileDialog>
#include <spdlog/spdlog.h>

#include "MachineConfigPresenter.h"
#include "ProjectUtils.h"
#include "../utils/ConfigurationManager.h"
#include "operation/FacingOperationPresenter.h"

MainPresenter::MainPresenter() : machineConfig(ConfigurationManager::loadMachineConfig()), toolTable(ConfigurationManager::loadToolTable()), window(machineConfig, toolTable) {
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

void MainPresenter::onFacingPressed() {
    spdlog::info("Facing pressed");
    currentOpConfigView = std::make_unique<OperationConfigurationView>(FacingOperationPresenter::visibility);
    currentOpConfigPresenter = std::make_unique<FacingOperationPresenter>(machineConfig, toolTable, project->geometry, window.getGeometryView(), *currentOpConfigView);

    // Connect OK and Cancel button signals
    connect(currentOpConfigView.get(), &OperationConfigurationView::okPressed, this, &MainPresenter::onOperationConfigOkPressed);
    connect(currentOpConfigView.get(), &OperationConfigurationView::cancelPressed, this, &MainPresenter::onOperationConfigCancelPressed);

    window.replaceLeftPanel(currentOpConfigView.get());
    window.disableOperationButtons();
}

void MainPresenter::setProject(Project p) {
    project = std::make_unique<Project>(p);
    window.setProject(*project);
    if (!p.savePath.empty()) {
        saveProject(*project, p.savePath);
    }
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

    // TODO: Save the operation configuration or apply it to the project
    // For now, just restore the left panel and enable operation buttons

    window.restoreLeftPanel();
    window.enableOperationButtons();

    // Clear the current operation configuration view and presenter
    currentOpConfigView.reset();
    currentOpConfigPresenter.reset();
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



