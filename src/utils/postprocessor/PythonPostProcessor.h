//
// Created by gawain on 9/20/25.
//

#ifndef TURNLAB_PYTHONPOSTPRECESSOR_H
#define TURNLAB_PYTHONPOSTPRECESSOR_H

#include <string>
#include <vector>
#include <pybind11/pybind11.h>

#include "../../model/MachineConfig.h"
#include "../../model/Tool.h"
#include "../../model/toolpath/TToolpathSequence.h"


struct PostProcessorState {
    int currentTool = -1;
    double currentRpm = -1;
    bool spindleOn = false;
};

class PythonPostProcessor {

    const MachineConfig& machineConfig;
    const ToolTable& toolTable;

    pybind11::object pyPostProcessor; // Store the Python pyPostProcessor instance

    void loadModules();
    std::string processToolpath(const std::unique_ptr<TToolpath>& toolpath, PostProcessorState& state);

    std::string setupTool(const std::unique_ptr<TToolpath> &toolpath, PostProcessorState &state);

    template<typename... Args>
    std::string callPostProcessor(const std::string& method, Args&&... args) {
        return pyPostProcessor.attr(method.c_str())(std::forward<Args>(args)...).template cast<std::string>();
    }

public:
    PythonPostProcessor(const MachineConfig& config, const ToolTable& tools);


    std::string generateGCode(const std::vector<TToolpathSequence>& toolpaths);

};


#endif //TURNLAB_PYTHONPOSTPRECESSOR_H