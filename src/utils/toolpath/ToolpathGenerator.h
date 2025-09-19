//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TOOLPATHGENERATOR_H
#define TURNLAB_TOOLPATHGENERATOR_H


#include "../../model/operation/OperationConfiguration.h"
#include "../../model/toolpath/Toolpath.h"

class ToolpathGenerator {

public:

    static TToolpathSequence generateToolpath(const OperationConfiguration& config);

    static TToolpathSequence generateFacingToolPath(const OperationConfiguration& config);

};


#endif //TURNLAB_TOOLPATHGENERATOR_H