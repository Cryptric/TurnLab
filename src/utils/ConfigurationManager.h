//
// Created by gawain on 9/16/25.
//

#ifndef TURNLAB_CONFIGURATIONMANAGER_H
#define TURNLAB_CONFIGURATIONMANAGER_H

#include <filesystem>
#include "../model/MachineConfig.h"
#include "../model/Tool.h"

class ConfigurationManager {
public:
    static MachineConfig loadMachineConfig();
    static ToolTable loadToolTable();
    static void saveMachineConfig(const MachineConfig& config);
    static void saveToolTable(const ToolTable& toolTable);

private:
    static std::filesystem::path getConfigDirectory();
    static std::filesystem::path getMachineConfigPath();
    static std::filesystem::path getToolTablePath();
    static ToolTable createDefaultToolTable();
};

#endif //TURNLAB_CONFIGURATIONMANAGER_H