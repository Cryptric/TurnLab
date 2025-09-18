//
// Created by gawain on 9/17/25.
//

#include "ProjectUtils.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

bool saveProject(const Project& project, const std::filesystem::path& filePath) {
    try {
        nlohmann::json projectJson = project;

        // Write to file
        std::ofstream file(filePath);
        if (!file.is_open()) {
            spdlog::error("Failed to open file for writing: {}", filePath.string());
            return false;
        }

        file << projectJson.dump(2); // Pretty print with 2-space indentation
        file.close();

        spdlog::info("Project saved successfully to: {}", filePath.string());
        return true;

    } catch (const std::exception& e) {
        spdlog::error("Error saving project: {}", e.what());
        return false;
    }
}

std::optional<Project> loadProject(const std::filesystem::path& filePath) {
    try {
        // Check if file exists
        if (!std::filesystem::exists(filePath)) {
            spdlog::error("Project file does not exist: {}", filePath.string());
            return std::nullopt;
        }

        // Read file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            spdlog::error("Failed to open file for reading: {}", filePath.string());
            return std::nullopt;
        }

        nlohmann::json projectJson;
        file >> projectJson;
        file.close();

        // Deserialize project using intrusive serialization
        Project project = projectJson.get<Project>();

        spdlog::info("Project loaded successfully from: {}", filePath.string());
        return project;

    } catch (const std::exception& e) {
        spdlog::error("Error loading project: {}", e.what());
        return std::nullopt;
    }
}