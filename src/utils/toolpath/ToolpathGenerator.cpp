//
// Created by gawain on 9/19/25.
//

#include "ToolpathGenerator.h"

#include <spdlog/spdlog.h>

#include "../../model/MachineConfig.h"

TToolpathSequence ToolpathGenerator::generateToolpath(const OperationConfiguration& opConfig, const MachineConfig& machineConfig) {
    switch (opConfig.operationType) {
        case OperationType::Facing:
            return generateFacingToolPath(opConfig, machineConfig);
        case OperationType::Turning:
            return generateTurningToolPath(opConfig, machineConfig);
        case OperationType::Parting:
            return generatePartingToolPath(opConfig, machineConfig);
        // Future cases for other operation types
        default:
            spdlog::error("Unsupported operation type for toolpath generation");
            return {};
    }
}

TToolpathSequence ToolpathGenerator::generateFacingToolPath(const OperationConfiguration& opConfig, const MachineConfig &machineConfig) {
    spdlog::debug("Generating toolpath for operation: {}", toString(opConfig.operationType));

    double zStartPos = opConfig.axialStartPosition + opConfig.axialStartOffset;
    double zEndPos = opConfig.axialEndPosition + opConfig.axialEndOffset;


    double clearanceDistance = opConfig.outerDistance + opConfig.feedDistance + opConfig.retractDistance + opConfig.clearanceDistance;
    double retractDistance = opConfig.outerDistance + opConfig.feedDistance + opConfig.retractDistance;
    double feedDistance = opConfig.outerDistance + opConfig.feedDistance;
    double innerDistance = opConfig.innerDistance;

    double backoffZDistance = zStartPos - opConfig.backoffDistance * (machineConfig.zAxisDirection == AxisDirection::Positive ? -1 : 1);

    TPoint backoffPoint(innerDistance, backoffZDistance);

    size_t numPasses = std::ceil(std::abs(zEndPos - zStartPos) / static_cast<double>(opConfig.stepover));
    TToolpathSequence toolpath;

    toolpath.addToolpath(std::make_unique<TLine>(TPoint(clearanceDistance, backoffZDistance), TPoint(retractDistance, backoffZDistance), opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm));

    double currentZ = zStartPos;
    for (size_t i = 0; i < numPasses - 1; i++) {
        currentZ += opConfig.stepover * (machineConfig.zAxisDirection == AxisDirection::Positive ? -1 : 1);
        // Move to correct z distance
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(retractDistance, backoffZDistance), TPoint(retractDistance, currentZ), opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm));
        // Move to feed distance
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(retractDistance, currentZ), TPoint(feedDistance, currentZ), opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm));
        // Move to inner distance
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(feedDistance, currentZ), TPoint(innerDistance, currentZ), opConfig.toolNumber, opConfig.feedrate, opConfig.rpm));
        // Move to backoff distance
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(innerDistance, currentZ), backoffPoint, opConfig.toolNumber, machineConfig.retractFeedRate, opConfig.rpm));
        // Move to retract distance
        toolpath.addToolpath(std::make_unique<TLine>(backoffPoint, TPoint(retractDistance, backoffZDistance), opConfig.toolNumber, machineConfig.retractFeedRate, opConfig.rpm));
    }

    // Move to final z position
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(retractDistance, backoffZDistance), TPoint(retractDistance, zEndPos), opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm));
    // Move to feed distance
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(retractDistance, zEndPos), TPoint(feedDistance, zEndPos), opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm));
    // Move to inner distance
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(feedDistance, zEndPos), TPoint(innerDistance, zEndPos), opConfig.toolNumber, opConfig.feedrate, opConfig.rpm));
    // Move to backoff distance
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(innerDistance, zEndPos), backoffPoint, opConfig.toolNumber, machineConfig.retractFeedRate, opConfig.rpm));
    // Move to retract distance
    toolpath.addToolpath(std::make_unique<TLine>(backoffPoint, TPoint(retractDistance, backoffZDistance), opConfig.toolNumber, machineConfig.retractFeedRate, opConfig.rpm));
    // Move to clearance distance
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(retractDistance, backoffZDistance), TPoint(clearanceDistance, backoffZDistance), opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm));

    return toolpath;
}

TToolpathSequence ToolpathGenerator::generateTurningToolPath(const OperationConfiguration &config, const MachineConfig& machineConfig) {
    spdlog::debug("Generating toolpath for operation: {}", toString(config.operationType));
    // turning consists of several straight moves
    // 1. move from retract to clearance distance
    // 2. move from clearance distance to feed distance
    // 3. move from feed distance to current outer distance
    // 4. move to axial end position
    // 5. back off to feed distance
    // 6. back off to clearance distance
    // 7. move to axial start position
    // goto 2
    // if inner distance reached:
    // 8. move to retract

    int toolNumber = config.toolNumber;
    double feedrate = config.feedrate;
    double rpm = config.rpm;

    double zStart = config.axialStartPosition + config.axialStartOffset;
    double zEnd = config.axialEndPosition + config.axialEndOffset;

    double clearanceDistance = config.outerDistance + config.feedDistance + config.retractDistance + config.clearanceDistance;
    double retractDistance = config.outerDistance + config.feedDistance + config.retractDistance;
    double feedDistance = config.outerDistance + config.feedDistance;
    double outerDistance = config.outerDistance;
    double innerDistance = config.innerDistance;

    TPoint clearanceStartPoint = {clearanceDistance, zStart};
    TPoint retractStartPoint = {retractDistance, zStart};
    TPoint feedStartPoint = {feedDistance, zStart};

    TPoint retractEndPoint = {retractDistance, zEnd};
    TPoint clearanceEndPoint = {clearanceDistance, zEnd};

    TToolpathSequence toolpath;

    size_t numPasses = std::ceil(std::abs(outerDistance - innerDistance) / static_cast<double>(config.stepover));

    toolpath.addToolpath(std::make_unique<TLine>(clearanceStartPoint, retractStartPoint, toolNumber, machineConfig.rapidFeedRate, rpm));

    double currentDistance = outerDistance;
    for (size_t i = 0; i < numPasses - 1; i++) {
        // move to feed distance
        toolpath.addToolpath(std::make_unique<TLine>(retractStartPoint, feedStartPoint, toolNumber, machineConfig.rapidFeedRate, rpm));
        // move to current outer distance + step over
        currentDistance += config.stepover * (machineConfig.xAxisDirection == AxisDirection::Positive ? -1 : 1);
        toolpath.addToolpath(std::make_unique<TLine>(feedStartPoint, TPoint(currentDistance, zStart), toolNumber, feedrate, rpm));
        // move to z end
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(currentDistance, zStart), TPoint(currentDistance, zEnd), toolNumber, feedrate, rpm));
        // move out to retract distance
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(currentDistance, zEnd), retractEndPoint, toolNumber, machineConfig.retractFeedRate, rpm));
        // move to start position
        toolpath.addToolpath(std::make_unique<TLine>(retractEndPoint, retractStartPoint, toolNumber, machineConfig.rapidFeedRate, rpm));
    }

    // move to feed distance
    toolpath.addToolpath(std::make_unique<TLine>(retractStartPoint, feedStartPoint, toolNumber, machineConfig.rapidFeedRate, rpm));
    // move to final distance
    toolpath.addToolpath(std::make_unique<TLine>(feedStartPoint, TPoint(innerDistance, zStart), toolNumber, feedrate, rpm));
    // move to z end
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(innerDistance, zStart), TPoint(innerDistance, zEnd), toolNumber, feedrate, rpm));
    // move out to retract distance
    toolpath.addToolpath(std::make_unique<TLine>(TPoint(innerDistance, zEnd), retractEndPoint, toolNumber, machineConfig.retractFeedRate, rpm));
    // move out to clearance distance
    toolpath.addToolpath(std::make_unique<TLine>(retractEndPoint, clearanceEndPoint, toolNumber, machineConfig.rapidFeedRate, rpm));

    return toolpath;
}

TToolpathSequence ToolpathGenerator::generatePartingToolPath(const OperationConfiguration& opConfig, const MachineConfig& machineConfig) {
    spdlog::debug("Generating toolpath for operation: {}", toString(opConfig.operationType));

    double zPos = opConfig.axialStartPosition + opConfig.axialStartOffset;
    TPoint clearancePoint(opConfig.outerDistance + opConfig.feedDistance + opConfig.retractDistance + opConfig.clearanceDistance, zPos);
    TPoint retractPoint(opConfig.outerDistance + opConfig.feedDistance + opConfig.retractDistance, zPos);
    TPoint feedPoint(opConfig.outerDistance + opConfig.feedDistance, zPos);
    TPoint outerPoint(opConfig.outerDistance, zPos);
    TPoint innerPoint(opConfig.innerDistance, zPos);


    TLine c2r(clearancePoint, retractPoint, opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm);
    TLine r2f(retractPoint, feedPoint, opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm);
    TLine r2c(retractPoint, clearancePoint, opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm);

    TToolpathSequence toolpath;
    toolpath.addToolpath(std::make_unique<TLine>(c2r));

    size_t numPasses = std::ceil(std::abs(outerPoint.x - innerPoint.x) / opConfig.cutDepthPerPass);
    double currentX = outerPoint.x;
    for (size_t i = 0; i < numPasses - 1; i++) {
        toolpath.addToolpath(std::make_unique<TLine>(r2f));
        currentX += opConfig.cutDepthPerPass * (machineConfig.xAxisDirection == AxisDirection::Positive ? -1 : 1);
        toolpath.addToolpath(std::make_unique<TLine>(feedPoint, TPoint(currentX, zPos), opConfig.toolNumber, opConfig.feedrate, opConfig.rpm));
        toolpath.addToolpath(std::make_unique<TLine>(TPoint(currentX, zPos), retractPoint, opConfig.toolNumber, machineConfig.retractFeedRate, opConfig.rpm));
    }
    toolpath.addToolpath(std::make_unique<TLine>(r2f));
    toolpath.addToolpath(std::make_unique<TLine>(feedPoint, innerPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm));
    toolpath.addToolpath(std::make_unique<TLine>(innerPoint, retractPoint, opConfig.toolNumber, opConfig.retractDistance, opConfig.rpm));
    toolpath.addToolpath(std::make_unique<TLine>(r2c));

    return toolpath;
}