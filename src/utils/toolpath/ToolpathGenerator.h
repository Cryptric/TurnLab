//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TOOLPATHGENERATOR_H
#define TURNLAB_TOOLPATHGENERATOR_H


#include "../../model/MachineConfig.h"
#include "../../model/operation/OperationConfiguration.h"
#include "../../model/toolpath/Toolpath.h"

class ToolpathGenerator {

public:

    static TToolpathSequence generateToolpath(const OperationConfiguration& config, const MachineConfig& machine_config);

    static TToolpathSequence generateFacingToolPath(const OperationConfiguration& config);
    static TToolpathSequence generateTurningToolPath(const OperationConfiguration& config, const MachineConfig& machine_config);
    static TToolpathSequence generatePartingToolPath(const OperationConfiguration &opConfig, const MachineConfig &machineConfig);
};


#endif //TURNLAB_TOOLPATHGENERATOR_H