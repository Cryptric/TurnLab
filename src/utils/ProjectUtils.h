//
// Created by gawain on 9/17/25.
//

#ifndef TURNLAB_PROJECTUTILS_H
#define TURNLAB_PROJECTUTILS_H

#include <filesystem>
#include <optional>
#include "../model/Project.h"

/**
 * Save a project to disk in TurnLab's native format.
 * @param project The project to save
 * @param filePath Path where to save the project file (should have .turnlab extension)
 * @return true if successful, false otherwise
 */
bool saveProject(const Project& project, const std::filesystem::path& filePath);

/**
 * Load a project from disk.
 * @param filePath Path to the project file to load
 * @return Project if successful, std::nullopt otherwise
 */
std::optional<Project> loadProject(const std::filesystem::path& filePath);

#endif //TURNLAB_PROJECTUTILS_H