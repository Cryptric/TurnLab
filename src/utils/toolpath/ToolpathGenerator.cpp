//
// Created by gawain on 9/19/25.
//

#include "ToolpathGenerator.h"

#include <spdlog/spdlog.h>

TToolpathSequence ToolpathGenerator::generateToolpath(const OperationConfiguration& opConfig) {
    switch (opConfig.operationType) {
        case OperationType::Facing:
            return generateFacingToolPath(opConfig);
        // Future cases for other operation types
        default:
            spdlog::error("Unsupported operation type for toolpath generation");
            return {};
    }
}

TToolpathSequence ToolpathGenerator::generateFacingToolPath(const OperationConfiguration& opConfig) {
    spdlog::debug("Generating toolpath for operation: {}", toString(opConfig.operationType));
    // facing toolpath consists of several straight moves:
    // 1. move from retract to clearance distance
    // 2. move from clearance distance to feed distance
    // 3. move move to outer distance
    // 4. move to inner distance
    // 2. back off
    // 3. move out
    double zPos = opConfig.axialStartPosition + opConfig.axialStartOffset;
    TPoint retractPoint(opConfig.outerDistance + opConfig.feedDistance + opConfig.clearanceDistance + opConfig.retractDistance, zPos);
    TPoint clearancePoint(opConfig.outerDistance + opConfig.feedDistance + opConfig.clearanceDistance, zPos);
    TPoint feedPoint(opConfig.outerDistance + opConfig.feedDistance, zPos);
    TPoint outerPoint(opConfig.outerDistance, zPos);
    TPoint innerPoint(opConfig.innerDistance, zPos);

    // TODO make +1 to a op config parameter lead out distance
    TPoint backOffPoint(opConfig.innerDistance, zPos + 1);
    TPoint outPoint(retractPoint.x, zPos + 1);

    TLine r2c(retractPoint, clearancePoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine c2f(clearancePoint, feedPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine f2o(feedPoint, outerPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine o2i(outerPoint, innerPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine i2b(innerPoint, backOffPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine b2o(backOffPoint, outPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);

    TToolpathSequence toolpath;
    toolpath.addToolpath(std::make_unique<TLine>(retractPoint, clearancePoint));
    toolpath.addToolpath(std::make_unique<TLine>(clearancePoint, feedPoint));
    toolpath.addToolpath(std::make_unique<TLine>(feedPoint, outerPoint));
    toolpath.addToolpath(std::make_unique<TLine>(outerPoint, innerPoint));
    toolpath.addToolpath(std::make_unique<TLine>(innerPoint, backOffPoint));
    toolpath.addToolpath(std::make_unique<TLine>(backOffPoint, outPoint));

    return toolpath;
}
