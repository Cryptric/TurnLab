//
// Created by gawain on 9/18/25.
//

#include "OperationConfigurationPlotHelper.h"
#include <spdlog/spdlog.h>
#include <qwt_text.h>

OperationConfigurationPlotHelper::OperationConfigurationPlotHelper(const OperationConfigVisibility& configVisibility, const OperationConfiguration& opConfig, GeometryView &geomView) : visibility(configVisibility), operationConfig(opConfig), geometryView(geomView) {

    retractDistanceMarker.setLineStyle(QwtPlotMarker::HLine);
    retractDistanceMarker.setLinePen(retractDistancePen);
    retractDistanceMarker.setTitle("Retract Distance");
    retractDistanceMarker.setLabel(QwtText(retractDistanceMarker.title().text()));
    retractDistanceMarker.setLabelAlignment(Qt::AlignTop | Qt::AlignRight);

    clearanceDistanceMarker.setLineStyle(QwtPlotMarker::HLine);
    clearanceDistanceMarker.setLinePen(clearanceDistancePen);
    clearanceDistanceMarker.setTitle("Clearance Distance");
    clearanceDistanceMarker.setLabel(QwtText(clearanceDistanceMarker.title().text()));
    clearanceDistanceMarker.setLabelAlignment(Qt::AlignTop | Qt::AlignRight);

    feedDistanceMarker.setLineStyle(QwtPlotMarker::HLine);
    feedDistanceMarker.setLinePen(feedDistancePen);
    feedDistanceMarker.setTitle("Feed Distance");
    feedDistanceMarker.setLabel(QwtText(feedDistanceMarker.title().text()));
    feedDistanceMarker.setLabelAlignment(Qt::AlignTop | Qt::AlignRight);

    outerDistanceMarker.setLineStyle(QwtPlotMarker::HLine);
    outerDistanceMarker.setLinePen(outerDistancePen);
    outerDistanceMarker.setTitle("Outer Distance");
    outerDistanceMarker.setLabel(QwtText(outerDistanceMarker.title().text()));
    outerDistanceMarker.setLabelAlignment(Qt::AlignTop | Qt::AlignRight);

    innerDistanceMarker.setLineStyle(QwtPlotMarker::HLine);
    innerDistanceMarker.setLinePen(innerDistancePen);
    innerDistanceMarker.setTitle("Inner Distance");
    innerDistanceMarker.setLabel(QwtText(innerDistanceMarker.title().text()));
    innerDistanceMarker.setLabelAlignment(Qt::AlignTop | Qt::AlignRight);

    axialStartOffsetMarker.setLineStyle(QwtPlotMarker::VLine);
    axialStartOffsetMarker.setLinePen(axialStartOffsetPen);
    axialStartOffsetMarker.setTitle("Axial Start Offset");

    axialEndOffsetMarker.setLineStyle(QwtPlotMarker::VLine);
    axialEndOffsetMarker.setLinePen(axialEndOffsetPen);
    axialEndOffsetMarker.setTitle("Axial End Offset");

    update();
}

void OperationConfigurationPlotHelper::showDistanceMarkers() {
    if (visibility.showRetractDistance) {
        retractDistanceMarker.attach(&geometryView);
        spdlog::debug("Showing retract distance marker at Y={}", operationConfig.retractDistance);
    }
    if (visibility.showClearanceDistance) {
        clearanceDistanceMarker.attach(&geometryView);
        spdlog::debug("Showing clearance distance marker at Y={}", operationConfig.clearanceDistance);
    }
    if (visibility.showFeedDistance) {
        feedDistanceMarker.attach(&geometryView);
        spdlog::debug("Showing feed distance marker at Y={}", operationConfig.feedDistance);
    }
    if (visibility.showOuterDistance) {
        outerDistanceMarker.attach(&geometryView);
        spdlog::debug("Showing outer distance marker at Y={}", operationConfig.outerDistance);
    }
    if (visibility.showInnerDistance) {
        innerDistanceMarker.attach(&geometryView);
        spdlog::debug("Showing inner distance marker at Y={}", operationConfig.innerDistance);
    }
    geometryView.replot();
}

void OperationConfigurationPlotHelper::hideDistanceMarkers() {
    retractDistanceMarker.detach();
    clearanceDistanceMarker.detach();
    feedDistanceMarker.detach();
    outerDistanceMarker.detach();
    innerDistanceMarker.detach();
    geometryView.replot();
}

void OperationConfigurationPlotHelper::showAxialOffsetMarkers() {
    if (visibility.showAxialStartOffset) {
        axialStartOffsetMarker.attach(&geometryView);
        spdlog::debug("Showing axial start offset marker at X={}", operationConfig.axialStartOffset);
    }
    if (visibility.showAxialEndOffset) {
        axialEndOffsetMarker.attach(&geometryView);
        spdlog::debug("Showing axial end offset marker at X={}", operationConfig.axialEndOffset);
    }
    geometryView.replot();
}

void OperationConfigurationPlotHelper::hideAxialOffsetMarkers() {
    axialStartOffsetMarker.detach();
    axialEndOffsetMarker.detach();
    geometryView.replot();
}

void OperationConfigurationPlotHelper::update() {
    clearanceDistanceMarker.setYValue(operationConfig.clearanceDistance + operationConfig.retractDistance + operationConfig.feedDistance + operationConfig.outerDistance);
    retractDistanceMarker.setYValue(operationConfig.retractDistance + operationConfig.feedDistance + operationConfig.outerDistance);
    feedDistanceMarker.setYValue(operationConfig.feedDistance + operationConfig.outerDistance);
    outerDistanceMarker.setYValue(operationConfig.outerDistance);
    innerDistanceMarker.setYValue(operationConfig.innerDistance);

    axialStartOffsetMarker.setXValue(operationConfig.axialStartPosition + operationConfig.axialStartOffset);
    axialEndOffsetMarker.setXValue(operationConfig.axialEndPosition + operationConfig.axialEndOffset);

    geometryView.replot();
}
