//
// Created by gawain on 9/20/25.
//

#include "PythonPostProcessor.h"

#include "python_bindings.h"

#include <pybind11/embed.h>
#include <spdlog/spdlog.h>
#include <filesystem>

namespace py = pybind11;

PythonPostProcessor::PythonPostProcessor(const MachineConfig& config, const ToolTable& tools) : machineConfig(config), toolTable(tools) {
    spdlog::debug("Creating PythonPostProcessor");
}

std::string PythonPostProcessor::generateGCode(const std::vector<TToolpathSequence>& toolpaths) {
    spdlog::info("PythonPostProcessor::generateGCode() called");

    spdlog::info("Creating Python interpreter");
    py::scoped_interpreter guard{};

    try {
        spdlog::info("Creating extension module 'turnlab'");
        auto m = py::module::create_extension_module("turnlab", nullptr, new py::module::module_def);

        spdlog::info("Calling init_py_module to register bindings");
        init_py_module(m);

        spdlog::info("Adding module to sys.modules");
        py::module::import("sys").attr("modules")["turnlab"] = m;

        // Add script directory to Python path and import module
        spdlog::info("Importing script module from: {}", machineConfig.postprocessorScriptPath);
        std::filesystem::path scriptPath(machineConfig.postprocessorScriptPath);
        std::string scriptDir = scriptPath.parent_path().string();
        std::string scriptName = scriptPath.stem().string(); // filename without extension

        py::module sys = py::module::import("sys");
        py::list path = sys.attr("path");
        path.insert(0, scriptDir); // Add script directory to Python path

        spdlog::info("Importing module: {} from directory: {}", scriptName, scriptDir);
        py::module script = py::module::import(scriptName.c_str());
        py::object postprocessorClass = script.attr(machineConfig.postprocessorClassName.c_str());
        py::object postprocessor = postprocessorClass(machineConfig, toolTable);


    } catch (const std::exception& e) {
        spdlog::error("Error: {}", e.what());
    }

    return "";
}

