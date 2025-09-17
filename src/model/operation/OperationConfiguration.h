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
    bool geometrySelection = false;
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

#endif //TURNLAB_OPERATIONCONFIGURATION_H