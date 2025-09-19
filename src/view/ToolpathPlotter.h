//
// Created by gawain on 9/19/25.
//

#ifndef TURNLAB_TOOLPATHPLOTTER_H
#define TURNLAB_TOOLPATHPLOTTER_H

#include <vector>
#include <memory>
#include <QPen>
#include <qwt_plot_curve.h>

#include "GeometryView.h"
#include "../model/toolpath/Toolpath.h"

class ToolpathPlotter {
private:
    GeometryView& geometryView;

    // Store curves for different toolpath sequences
    std::vector<std::unique_ptr<QwtPlotCurve>> toolpathCurves;

    // Pen styles following CAM industry conventions
    const QPen rapidMovePen = QPen(QColor(255, 165, 0), 0.5, Qt::DashLine);      // Orange - rapid moves
    const QPen feedMovePen = QPen(QColor(0, 255, 0), 0.5, Qt::SolidLine);        // Green - feed moves
    const QPen plungeMovePen = QPen(QColor(255, 0, 0), 0.5, Qt::SolidLine);      // Red - plunge moves
    const QPen retractMovePen = QPen(QColor(255, 255, 0), 0.5, Qt::DotLine);     // Yellow - retract moves

    // Helper methods
    void plotToolpathLine(const TLine& line, const QPen& pen, const QString& title);
    QPen getPenForToolpath(const TToolpath& toolpath);
    QString getTitleForToolpath(const TToolpath& toolpath, size_t sequenceIndex, size_t toolpathIndex);

public:
    explicit ToolpathPlotter(GeometryView& geomView);
    ~ToolpathPlotter();

    // Main plotting methods
    void plotToolpaths(const std::vector<TToolpathSequence>& sequences);
    void plotToolpathSequence(const TToolpathSequence& sequence, size_t sequenceIndex = 0);
    void clearToolpaths();
    void showToolpaths();
    void hideToolpaths();

    // Individual toolpath plotting
    void plotLine(const TLine& line, size_t sequenceIndex = 0, size_t toolpathIndex = 0);
};

#endif //TURNLAB_TOOLPATHPLOTTER_H