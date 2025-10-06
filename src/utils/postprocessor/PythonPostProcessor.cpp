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

void PythonPostProcessor::loadModules() {
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
        pyPostProcessor = postprocessorClass(machineConfig, toolTable);
        spdlog::info("PostProcessor instance created successfully");

    } catch (const std::exception& e) {
        spdlog::error("Error loading modules: {}", e.what());
        throw;
    }
}

std::string PythonPostProcessor::processToolpath(const std::unique_ptr<TToolpath>& toolpath, PostProcessorState& state) {
    std::string gcode;

    gcode += setupTool(toolpath, state);

    // Handle spindle speed changes
    if (state.currentRpm != toolpath->rpm) {
        gcode += callPostProcessor("spindle_on", toolpath->rpm);
        state.currentRpm = toolpath->rpm;
        state.spindleOn = true;
    }

    // Process specific toolpath type
    if (auto line = dynamic_cast<TLine*>(toolpath.get())) {
        gcode += callPostProcessor("linear_move", line->end, line->feedRate);
    } else {
        spdlog::warn("Unsupported toolpath type");
    }

    return gcode;
}

std::string PythonPostProcessor::setupTool(const std::unique_ptr<TToolpath>& toolpath, PostProcessorState& state) {
    std::string gcode = "";
    if (state.currentTool != toolpath->toolNumber) {
        gcode += callPostProcessor("tool_change", toolpath->toolNumber);
        state.currentTool = toolpath->toolNumber;
    }
    return gcode;
}

std::string PythonPostProcessor::generateGCode(const std::vector<TToolpathSequence>& toolpaths) {
    spdlog::info("PythonPostProcessor::generateGCode() called with {} toolpath sequences", toolpaths.size());

    spdlog::info("Creating Python interpreter");
    py::scoped_interpreter guard{};

    try {
        loadModules();

        std::string gcode;
        PostProcessorState state;

        // Process each toolpath sequence
        for (const auto& sequence : toolpaths) {
            gcode += setupTool(sequence.toolpaths[0], state);
            gcode += callPostProcessor("rapid_move", sequence.toolpaths[0]->getStartPosition(), 100.0);
            for (const auto& toolpath : sequence.toolpaths) {
                gcode += processToolpath(toolpath, state);
            }
        }

        // Turn off spindle at end if it was on
        if (state.spindleOn) {
            gcode += callPostProcessor("spindle_off");
        }

        spdlog::info("Generated {} characters of GCode", gcode.length());

        // Release Python objects before interpreter destruction
        pyPostProcessor = py::none();

        return gcode;

    } catch (const std::exception& e) {
        spdlog::error("Error generating GCode: {}", e.what());

        // Release Python objects before interpreter destruction
        pyPostProcessor = py::none();

        return "";
    }
}

