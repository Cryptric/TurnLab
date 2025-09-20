//
// Created by gawain on 9/20/25.
//

#ifndef TURNLAB_PYTHONPOSTPRECESSOR_H
#define TURNLAB_PYTHONPOSTPRECESSOR_H

#include <string>
#include <vector>

#include "../../model/MachineConfig.h"
#include "../../model/Tool.h"
#include "../../model/toolpath/TToolpathSequence.h"


class PythonPostProcessor {

    const MachineConfig& machineConfig;
    const ToolTable& toolTable;

public:
    PythonPostProcessor(const MachineConfig& config, const ToolTable& tools);


    std::string generateGCode(const std::vector<TToolpathSequence>& toolpaths);

};


#endif //TURNLAB_PYTHONPOSTPRECESSOR_H