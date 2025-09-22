//
// Created by gawain on 9/22/25.
//

#ifndef TURNLAB_PARTINGOPERATIONPRESENTER_H
#define TURNLAB_PARTINGOPERATIONPRESENTER_H
#include <QObject>

#include "../OperationConfigurationPresenter.h"


class PartingOperationPresenter : public OperationConfigurationPresenter {

    void onSegmentSelected(size_t segmentIndex) override;

public:
    explicit PartingOperationPresenter(
    const MachineConfig& machineConfig,
    const ToolTable& toolTable,
    const Project& project,
    GeometryView& geometryView,
    OperationConfigurationView& operationConfigView,
    QObject* parent = nullptr
);


    static constexpr OperationConfigVisibility visibility = {
        .showToolSelector = true,
        .showRpmInput = true,
        .showFeedrateInput = true,

        .showGeometrySelection = true,
        .singleSegmentSelection = true,
        .showAxialStartOffset = true,

        .showRetractDistance = true,
        .showClearanceDistance = true,
        .showFeedDistance = true,
        .showOuterDistance = true,
        .showInnerDistance = true,

        .showCutDepthPerPass = true,

        .showToolTab = true,
        .showGeometryTab = true,
        .showRadiiTab = true,
        .showPassesTab = true,
    };

};


#endif //TURNLAB_PARTINGOPERATIONPRESENTER_H