//
// Created by gawain on 9/19/25.
//

#include "FacingOperationPresenter.h"

#include <spdlog/spdlog.h>

#include "GeometryUtils.h"
#include "../model/geometry/Line.h"

FacingOperationPresenter::FacingOperationPresenter(const MachineConfig &machineConfig, const ToolTable &toolTable, const Project &project, GeometryView &geometryView, OperationConfigurationView &operationConfigView, QObject *parent) : OperationConfigurationPresenter(visibility, machineConfig, toolTable, project, geometryView, operationConfigView, parent) {
    operationConfig.operationType = OperationType::Facing;
}


void FacingOperationPresenter::onSegmentSelected(size_t segmentIndex) {
    // first check if it is a line, then check if it is vertical, if not return
    if (auto line = dynamic_cast<Line*>(project.geometry.segments[segmentIndex].get())) {
        if (!line->isVertical()) {
            spdlog::warn("Facing operation requires a vertical line segment. Selected segment is not vertical.");
            return;
        }
        OperationConfigurationPresenter::onSegmentSelected(segmentIndex);
        operationConfig.axialStartPosition = line->p1.x;
        operationConfig.axialEndPosition = line->p1.x;

        auto [innerP, outerP] = GeometryUtils::getInnerAndOuterPoint(*line, machineConfig);

        operationConfig.innerDistance = innerP.y;
        operationConfig.outerDistance = outerP.y;

        configView.setOperationConfiguration(operationConfig);
        plotHelper.update();
        configView.update();

    } else {
        spdlog::warn("Facing operation requires a line segment. Selected segment is not a line.");
        return;
    }
}
