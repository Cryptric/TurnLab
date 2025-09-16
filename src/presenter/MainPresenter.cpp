//
// Created by gawain on 9/11/25.
//

#include "MainPresenter.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <QStandardPaths>
#include <spdlog/spdlog.h>

#include "MachineConfigPresenter.h"

MainPresenter::MainPresenter() : machineConfig(loadMachineConfig()), window(machineConfig) {
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
    saveMachineConfig();
    spdlog::info("Machine configuration updated and saved");
}

std::filesystem::path MainPresenter::getConfigPath() {
    // Get platform-appropriate config directory
    const QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    const std::filesystem::path configPath = std::filesystem::path(configDir.toStdString()) / "turnlab";
    
    // Create directory if it doesn't exist
    std::filesystem::create_directories(configPath);
    
    return configPath / "machine_config.json";
}

MachineConfig MainPresenter::loadMachineConfig() {
    try {
        std::filesystem::path configFile = getConfigPath();
        
        if (!std::filesystem::exists(configFile)) {
            spdlog::info("Machine config file not found, using defaults");
            // machineConfig will use default values from constructor
            return MachineConfig();
        }
        
        std::ifstream file(configFile);
        if (!file.is_open()) {
            spdlog::error("Failed to open machine config file: {}", configFile.string());
            return MachineConfig();
        }
        
        nlohmann::json j;
        file >> j;
        MachineConfig config = j.get<MachineConfig>();
        
        spdlog::info("Machine configuration loaded from: {}", configFile.string());
        return config;
    } catch (const std::exception& e) {
        spdlog::error("Error loading machine config: {}", e.what());
        // Continue with default configuration
        return MachineConfig();
    }
}

void MainPresenter::saveMachineConfig() {
    try {
        std::filesystem::path configFile = getConfigPath();
        
        std::ofstream file(configFile);
        if (!file.is_open()) {
            spdlog::error("Failed to create machine config file: {}", configFile.string());
            return;
        }
        
        nlohmann::json j = machineConfig;
        file << j.dump(2);  // Pretty print with 2-space indentation
        
        spdlog::info("Machine configuration saved to: {}", configFile.string());
    } catch (const std::exception& e) {
        spdlog::error("Error saving machine config: {}", e.what());
    }
}


