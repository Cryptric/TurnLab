//
// Created by Claude on $(date +%m/%d/%y).
//

#ifndef TURNLAB_MACHINECONFIG_H
#define TURNLAB_MACHINECONFIG_H

#include <nlohmann/json.hpp>

enum class AxisDirection {
    Positive,
    Negative
};

struct MachineConfig {
    // Axis Direction Configuration
    AxisDirection zAxisDirection = AxisDirection::Positive;  // Default: moving towards tailstock is positive
    AxisDirection xAxisDirection = AxisDirection::Positive;  // Default: moving away from centerline is positive
    
    // Machine Limits
    double maxSpindleSpeed = 3000.0;      // Maximum RPM
    double maxFeedRateMMMin = 2000.0;     // Maximum feed rate in mm/min
    double maxFeedRateMMRev = 0.5;        // Maximum feed rate in mm/rev
    double maxZTravel = 300.0;            // Maximum Z travel distance in mm
    double maxXRadius = 150.0;            // Maximum X radius in mm
    double minClearanceDistance = 5.0;    // Minimum clearance distance for safety in mm
    
    // Display Settings
    int displayPrecision = 3;             // Number of decimal places for coordinates
    
    // Chuck Position (fixed on left side - no configuration needed)
    
    // JSON serialization
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MachineConfig,
        zAxisDirection,
        xAxisDirection,
        maxSpindleSpeed,
        maxFeedRateMMMin,
        maxFeedRateMMRev,
        maxZTravel,
        maxXRadius,
        minClearanceDistance,
        displayPrecision
    )
};

// JSON serialization for AxisDirection enum
NLOHMANN_JSON_SERIALIZE_ENUM(AxisDirection, {
    {AxisDirection::Positive, "positive"},
    {AxisDirection::Negative, "negative"}
})

#endif //TURNLAB_MACHINECONFIG_H