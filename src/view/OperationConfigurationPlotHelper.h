//
// Created by gawain on 9/18/25.
//

#ifndef TURNLAB_OPERATIONCONFIGURATIONPLOTHELPER_H
#define TURNLAB_OPERATIONCONFIGURATIONPLOTHELPER_H
#include "GeometryView.h"
#include <QPen>

#include "../model/operation/OperationConfiguration.h"

class OperationConfigurationPlotHelper {

private:
    const OperationConfigVisibility& visibility;
    const OperationConfiguration& operationConfig;

    GeometryView& geometryView;

    // Distance marker pen styles following CAM industry conventions
    const QPen retractDistancePen = QPen(QColor(255, 165, 0), 2.0, Qt::DashLine);     // Orange - rapid/retract moves
    const QPen clearanceDistancePen = QPen(QColor(255, 0, 0), 2.0, Qt::DotLine);      // Red - safety clearance
    const QPen feedDistancePen = QPen(QColor(0, 255, 0), 2.0, Qt::SolidLine);         // Green - feed engagement
    const QPen outerDistancePen = QPen(QColor(0, 100, 255), 2.0, Qt::SolidLine);      // Blue - outer boundary
    const QPen innerDistancePen = QPen(QColor(255, 0, 255), 2.0, Qt::SolidLine);      // Magenta - inner boundary

    // Axial offset marker pen styles
    const QPen axialStartOffsetPen = QPen(QColor(255, 255, 0), 2.0, Qt::SolidLine);   // Yellow - start offset
    const QPen axialEndOffsetPen = QPen(QColor(255, 140, 0), 2.0, Qt::SolidLine);     // Dark orange - end offset

    QwtPlotMarker retractDistanceMarker;
    QwtPlotMarker clearanceDistanceMarker;
    QwtPlotMarker feedDistanceMarker;
    QwtPlotMarker outerDistanceMarker;
    QwtPlotMarker innerDistanceMarker;

    QwtPlotMarker axialStartOffsetMarker;
    QwtPlotMarker axialEndOffsetMarker;


public:
    OperationConfigurationPlotHelper(const OperationConfigVisibility& configVisibility, const OperationConfiguration& opConfig, GeometryView& geomView);

    void showDistanceMarkers();
    void hideDistanceMarkers();
    void showAxialOffsetMarkers();
    void hideAxialOffsetMarkers();
    void update();
};


#endif //TURNLAB_OPERATIONCONFIGURATIONPLOTHELPER_H