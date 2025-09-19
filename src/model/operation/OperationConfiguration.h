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

    double axialStartPosition = 10;     // mm
    double axialEndPosition = 0;       // mm
    double axialStartOffset = 0;  // mm
    double axialEndOffset = 0;    // mm

    // Radii configuration
    double retractDistance = 5;   // mm
    double clearanceDistance = 2; // mm
    double feedDistance = 1;      // mm
    double outerDistance = 10;    // mm
    double innerDistance = 5;     // mm

    // Passes configuration
    double stepover = 5.0;          // mm
    double cutDepthPerPass = 2.0;   // mm
    int springPasses = 1;
    double peckDepth = 3.0;         // mm
    int dwellTime = 500;            // ms
};

struct OperationConfigVisibility {
    // Tool tab
    bool showToolSelector = false;
    bool showRpmInput = false;
    bool showFeedrateInput = false;

    // Geometry tab
    bool showGeometrySelection = false;
    bool singleSegmentSelection = false;
    bool showAxialStartOffset = false;
    bool showAxialEndOffset = false;

    // Radii tab
    bool showRetractDistance = false;
    bool showClearanceDistance = false;
    bool showFeedDistance = false;
    bool showOuterDistance = false;
    bool showInnerDistance = false;

    // Passes tab
    bool showStepover = false;
    bool showCutDepthPerPass = false;
    bool showSpringPasses = false;
    bool showPeckDepth = false;
    bool showDwellTime = false;

    // Tab visibility
    bool showToolTab = false;
    bool showGeometryTab = false;
    bool showRadiiTab = false;
    bool showPassesTab = false;
};

#endif //TURNLAB_OPERATIONCONFIGURATION_H