//
// Created by gawain on 9/20/25.
//

#ifndef TURNLAB_TURNINGOPERATIONPRESENTER_H
#define TURNLAB_TURNINGOPERATIONPRESENTER_H
#include "../OperationConfigurationPresenter.h"


class TurningOperationPresenter : public OperationConfigurationPresenter {

    void onSegmentSelected(size_t segmentIndex) override;

public:
    explicit TurningOperationPresenter(
        const MachineConfig& machineConfig,
        const ToolTable& toolTable,
        const Project& project,
        GeometryView& geometryView,
        OperationConfigurationView& operationConfigView,
        QWidget* parent = nullptr
    );

    static constexpr OperationConfigVisibility visibility = {
        .showToolSelector = true,
        .showRpmInput = true,
        .showFeedrateInput = true,

        .showGeometrySelection = true,
        .singleSegmentSelection = true,
        .showAxialStartOffset = true,
        .showAxialEndOffset = true,

        .showRetractDistance = true,
        .showClearanceDistance = true,
        .showFeedDistance = true,
        .showOuterDistance = true,
        .showInnerDistance = true,

        .showStepover = true,

        .showToolTab = true,
        .showGeometryTab = true,
        .showRadiiTab = true,
        .showPassesTab = true,
    };


};


#endif //TURNLAB_TURNINGOPERATIONPRESENTER_H