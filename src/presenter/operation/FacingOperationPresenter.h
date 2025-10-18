//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_FACINGOPERATIONPRESENTER_H
#define TURNLAB_FACINGOPERATIONPRESENTER_H
#include "../OperationConfigurationPresenter.h"


class FacingOperationPresenter : public OperationConfigurationPresenter {


    void onSegmentSelected(size_t segmentIndex) override;


public:
    explicit FacingOperationPresenter(
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
        .showAxialEndOffset = true,

        .showRetractDistance = true,
        .showClearanceDistance = true,
        .showFeedDistance = true,
        .showOuterDistance = true,
        .showInnerDistance = true,

        .showStepover = true,
        .showBackoffDistance = true,

        .showToolTab = true,
        .showGeometryTab = true,
        .showRadiiTab = true,
        .showPassesTab = true,
    };
};


#endif //TURNLAB_FACINGOPERATIONPRESENTER_H