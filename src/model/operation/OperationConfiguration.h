//
// Created by gawain on 9/16/25.
//

#ifndef TURNLAB_OPERATIONCONFIGURATION_H
#define TURNLAB_OPERATIONCONFIGURATION_H

struct OperationConfiguration {
    // Tool configuration
    int toolNumber = 0;
    int rpm = 1000;
    int feedrate = 100;  // mm/min

    // Geometry configuration
    std::vector<size_t> geometrySelection = {};
    int axialStartOffset = 0;  // mm
    int axialEndOffset = 0;    // mm

    // Radii configuration
    int retractDistance = 5;   // mm
    int clearanceDistance = 2; // mm
    int feedDistance = 1;      // mm
    int outerDistance = 10;    // mm
    int innerDistance = 5;     // mm

    // Passes configuration
    double stepover = 5.0;          // mm
    double cutDepthPerPass = 2.0;   // mm
    int springPasses = 1;
    double peckDepth = 3.0;         // mm
    int dwellTime = 500;            // ms
};

struct OperationConfigVisibility {
    // Tool tab
    bool showToolSelector = true;
    bool showRpmInput = true;
    bool showFeedrateInput = true;

    // Geometry tab
    bool showGeometrySelection = true;
    bool singleSegmentSelection = true;
    bool showAxialStartOffset = true;
    bool showAxialEndOffset = true;

    // Radii tab
    bool showRetractDistance = true;
    bool showClearanceDistance = true;
    bool showFeedDistance = true;
    bool showOuterDistance = true;
    bool showInnerDistance = true;

    // Passes tab
    bool showStepover = true;
    bool showCutDepthPerPass = true;
    bool showSpringPasses = true;
    bool showPeckDepth = true;
    bool showDwellTime = true;

    // Tab visibility
    bool showToolTab = true;
    bool showGeometryTab = true;
    bool showRadiiTab = true;
    bool showPassesTab = true;
};

#endif //TURNLAB_OPERATIONCONFIGURATION_H