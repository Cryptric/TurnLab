//
// Created by gawain on 9/16/25.
//

#ifndef TURNLAB_OPERATIONCONFIGURATION_H
#define TURNLAB_OPERATIONCONFIGURATION_H

#include <nlohmann/json.hpp>

enum class OperationType {
    Facing,
    Turning,
    Contouring,
    Threading,
    Parting,
    Drilling
};

NLOHMANN_JSON_SERIALIZE_ENUM(OperationType, {
    {OperationType::Facing, "Facing"},
    {OperationType::Turning, "Turning"},
    {OperationType::Contouring, "Contouring"},
    {OperationType::Threading, "Threading"},
    {OperationType::Parting, "Parting"},
    {OperationType::Drilling, "Drilling"}
})

inline std::string toString(OperationType type) {
    switch (type) {
        case OperationType::Facing: return "Facing";
        case OperationType::Turning: return "Turning";
        case OperationType::Contouring: return "Contouring";
        case OperationType::Threading: return "Threading";
        case OperationType::Parting: return "Parting";
        case OperationType::Drilling: return "Drilling";
        default: return "Unknown";
    }
}

struct OperationConfiguration {
    OperationType operationType;

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

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OperationConfiguration,
        operationType,
        toolNumber, rpm, feedrate,
        geometrySelection,
        axialStartPosition, axialEndPosition, axialStartOffset, axialEndOffset,
        retractDistance, clearanceDistance, feedDistance, outerDistance, innerDistance,
        stepover, cutDepthPerPass, springPasses, peckDepth, dwellTime)
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

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OperationConfigVisibility,
        showToolSelector, showRpmInput, showFeedrateInput,
        showGeometrySelection, singleSegmentSelection, showAxialStartOffset, showAxialEndOffset,
        showRetractDistance, showClearanceDistance, showFeedDistance, showOuterDistance, showInnerDistance,
        showStepover, showCutDepthPerPass, showSpringPasses, showPeckDepth, showDwellTime,
        showToolTab, showGeometryTab, showRadiiTab, showPassesTab)
};

#endif //TURNLAB_OPERATIONCONFIGURATION_H