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
    // facing toolpath consists of several straight moves:
    // 1. move from clearance to feed distance
    // 3. move from feed to outer distance
    // 4. move to inner distance
    // 2. back off
    // 3. move out
    double zPos = opConfig.axialStartPosition + opConfig.axialStartOffset;
    double retractDistance  = opConfig.outerDistance + opConfig.feedDistance + opConfig.retractDistance;

    TPoint clearancePoint(opConfig.outerDistance + opConfig.feedDistance + opConfig.retractDistance + opConfig.clearanceDistance, zPos);
    TPoint feedPoint(opConfig.outerDistance + opConfig.feedDistance, zPos);
    TPoint outerPoint(opConfig.outerDistance, zPos);
    TPoint innerPoint(opConfig.innerDistance, zPos);

    // TODO make +1 to a op config parameter lead out distance
    TPoint backOffPoint(opConfig.innerDistance, zPos + 1);
    TPoint retractPoint(retractDistance, zPos + 1);

    TLine c2f(clearancePoint, feedPoint, opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm);
    TLine f2o(feedPoint, outerPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine o2i(outerPoint, innerPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine i2b(innerPoint, backOffPoint, opConfig.toolNumber, opConfig.feedrate, opConfig.rpm);
    TLine b2r(backOffPoint, retractPoint, opConfig.toolNumber, machineConfig.retractFeedRate, opConfig.rpm);
    TLine r2c(retractPoint, {clearancePoint.x, zPos + 1}, opConfig.toolNumber, machineConfig.rapidFeedRate, opConfig.rpm);

    TToolpathSequence toolpath;
    toolpath.addToolpath(std::make_unique<TLine>(c2f));
    toolpath.addToolpath(std::make_unique<TLine>(f2o));
    toolpath.addToolpath(std::make_unique<TLine>(o2i));
    toolpath.addToolpath(std::make_unique<TLine>(i2b));
    toolpath.addToolpath(std::make_unique<TLine>(b2r));
    toolpath.addToolpath(std::make_unique<TLine>(r2c));

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