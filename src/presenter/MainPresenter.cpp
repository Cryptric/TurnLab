//
// Created by gawain on 9/11/25.
//

#include "MainPresenter.h"

#include <spdlog/spdlog.h>

#include "MachineConfigPresenter.h"
#include "../utils/ConfigurationManager.h"

MainPresenter::MainPresenter() : machineConfig(ConfigurationManager::loadMachineConfig()), toolTable(ConfigurationManager::loadToolTable()), window(machineConfig, toolTable) {
    window.show();
    connectSignals();
}

MainPresenter::MainPresenter(const std::string &inputDXF) : MainPresenter() {
    showDXFImportDialog(inputDXF);
}

void MainPresenter::connectSignals() {
    connect(&window, &MainWindow::onMachineConfigPressed, this, &MainPresenter::showMachineConfigDialog);
}

void MainPresenter::showDXFImportDialog(const std::string& inputDXF) {
    const DXFImportPresenter presenter(inputDXF, machineConfig, &window);
    if (const std::optional<Project> p = presenter.showDialog(); p.has_value()) {
        setProject(p.value());
    }
}

void MainPresenter::setProject(Project p) {
    project = std::make_unique<Project>(p);
    window.setProject(*project.get());
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



