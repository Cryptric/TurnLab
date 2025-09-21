//
// Created by gawain on 9/20/25.
//

#include "TurningOperationPresenter.h"

#include <spdlog/spdlog.h>
#include "GeometryUtils.h"
#include "../model/geometry/Line.h"

TurningOperationPresenter::TurningOperationPresenter(const MachineConfig &machineConfig, const ToolTable &toolTable, const Project &project, GeometryView &geometryView, OperationConfigurationView &operationConfigView, QWidget *parent)
    : OperationConfigurationPresenter(visibility, machineConfig, toolTable, project, geometryView, operationConfigView, parent) {
    operationConfig.operationType = OperationType::Turning;
}

void TurningOperationPresenter::onSegmentSelected(size_t segmentIndex) {
    if (auto line = dynamic_cast<Line*>(project.geometry.segments[segmentIndex].get())) {
        if (!line->isHorizontal()) {
            spdlog::warn("Facing operation requires a vertical line segment. Selected segment is not vertical.");
            return;
        }
        OperationConfigurationPresenter::onSegmentSelected(segmentIndex);

        auto [chuckP, tailStockP] = GeometryUtils::getChuckAndTailstockPoint(*line, machineConfig);

        operationConfig.axialStartPosition = tailStockP.x;
        operationConfig.axialEndPosition = chuckP.x;

        operationConfig.innerDistance = chuckP.y;
        operationConfig.outerDistance = machineConfig.xAxisDirection == AxisDirection::Positive ? project.stockMaterial.radius : -project.stockMaterial.radius;

        configView.setOperationConfiguration(operationConfig);
        plotHelper.update();
        configView.update();

    } else {
        spdlog::warn("Facing operation requires a line segment. Selected segment is not a line.");
        return;
    }
}
