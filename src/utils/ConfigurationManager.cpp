//
// Created by gawain on 9/16/25.
//

#include "ConfigurationManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <QStandardPaths>
#include <spdlog/spdlog.h>

std::filesystem::path ConfigurationManager::getConfigDirectory() {
    // Get platform-appropriate config directory
    const QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    const std::filesystem::path configPath = std::filesystem::path(configDir.toStdString()) / "turnlab";

    // Create directory if it doesn't exist
    std::filesystem::create_directories(configPath);

    return configPath;
}

std::filesystem::path ConfigurationManager::getMachineConfigPath() {
    return getConfigDirectory() / "machine_config.json";
}

std::filesystem::path ConfigurationManager::getToolTablePath() {
    return getConfigDirectory() / "tool_table.json";
}

MachineConfig ConfigurationManager::loadMachineConfig() {
    try {
        std::filesystem::path configFile = getMachineConfigPath();

        if (!std::filesystem::exists(configFile)) {
            spdlog::info("Machine config file not found, using defaults");
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
        return MachineConfig();
    }
}

ToolTable ConfigurationManager::loadToolTable() {
    try {
        std::filesystem::path toolTableFile = getToolTablePath();

        if (!std::filesystem::exists(toolTableFile)) {
            spdlog::info("Tool table file not found, using defaults");
            return createDefaultToolTable();
        }

        std::ifstream file(toolTableFile);
        if (!file.is_open()) {
            spdlog::error("Failed to open tool table file: {}", toolTableFile.string());
            return ToolTable();
        }

        nlohmann::json j;
        file >> j;
        ToolTable table = j.get<ToolTable>();

        spdlog::info("Tool table loaded from: {}", toolTableFile.string());
        return table;
    } catch (const std::exception& e) {
        spdlog::error("Error loading tool table: {}", e.what());
        return ToolTable();
    }
}

void ConfigurationManager::saveMachineConfig(const MachineConfig& config) {
    try {
        std::filesystem::path configFile = getMachineConfigPath();

        std::ofstream file(configFile);
        if (!file.is_open()) {
            spdlog::error("Failed to create machine config file: {}", configFile.string());
            return;
        }

        nlohmann::json j = config;
        file << j.dump(2);  // Pretty print with 2-space indentation

        spdlog::info("Machine configuration saved to: {}", configFile.string());
    } catch (const std::exception& e) {
        spdlog::error("Error saving machine config: {}", e.what());
    }
}

void ConfigurationManager::saveToolTable(const ToolTable& toolTable) {
    try {
        std::filesystem::path toolTableFile = getToolTablePath();

        std::ofstream file(toolTableFile);
        if (!file.is_open()) {
            spdlog::error("Failed to create tool table file: {}", toolTableFile.string());
            return;
        }

        nlohmann::json j = toolTable;
        file << j.dump(2);  // Pretty print with 2-space indentation

        spdlog::info("Tool table saved to: {}", toolTableFile.string());
    } catch (const std::exception& e) {
        spdlog::error("Error saving tool table: {}", e.what());
    }
}

ToolTable ConfigurationManager::createDefaultToolTable() {
    ToolTable defaultTable;
    defaultTable.tools = {
        {1, "General Purpose Turning Tool", "CNMG120408"},
        {2, "Finishing Turning Tool", "VNMG160404"},
        {3, "Grooving Tool", "MGMN200-G"},
        {4, "Threading Tool", "16ER-AG60"},
        {5, "Parting Tool", "GTN-2"}
    };
    return defaultTable;
}