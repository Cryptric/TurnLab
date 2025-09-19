//
// Created by gawain on 9/19/25.
//

#include "ToolpathPlotter.h"
#include <spdlog/spdlog.h>
#include <qwt_text.h>

ToolpathPlotter::ToolpathPlotter(GeometryView& geomView) : geometryView(geomView) {
    spdlog::debug("Creating ToolpathPlotter");
}

ToolpathPlotter::~ToolpathPlotter() {
    clearToolpaths();
}

void ToolpathPlotter::plotToolpaths(const std::vector<TToolpathSequence>& sequences) {
    spdlog::debug("Plotting {} toolpath sequences", sequences.size());

    clearToolpaths();

    for (size_t i = 0; i < sequences.size(); ++i) {
        plotToolpathSequence(sequences[i], i);
    }

    geometryView.replot();
}

void ToolpathPlotter::plotToolpathSequence(const TToolpathSequence& sequence, size_t sequenceIndex) {
    spdlog::debug("Plotting toolpath sequence {} with {} toolpaths", sequenceIndex, sequence.size());

    for (size_t i = 0; i < sequence.toolpaths.size(); ++i) {
        const auto& toolpath = sequence.toolpaths[i];

        // Check toolpath type and plot accordingly
        if (auto line = dynamic_cast<const TLine*>(toolpath.get())) {
            plotLine(*line, sequenceIndex, i);
        }
        // Add more toolpath types here as they are implemented
    }
}

void ToolpathPlotter::plotLine(const TLine& line, size_t sequenceIndex, size_t toolpathIndex) {
    QPen pen = getPenForToolpath(line);
    QString title = getTitleForToolpath(line, sequenceIndex, toolpathIndex);

    plotToolpathLine(line, pen, title);
}

void ToolpathPlotter::plotToolpathLine(const TLine& line, const QPen& pen, const QString& title) {
    auto curve = std::make_unique<QwtPlotCurve>(title);

    // Set up the curve
    curve->setPen(pen);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);

    // Create data points for the line
    QVector<double> xData = { line.start.z, line.end.z };
    QVector<double> yData = { line.start.x, line.end.x };

    curve->setSamples(xData, yData);
    curve->attach(&geometryView);

    toolpathCurves.push_back(std::move(curve));
}

QPen ToolpathPlotter::getPenForToolpath(const TToolpath& toolpath) {
    // Determine pen style based on toolpath characteristics
    // This is a simple heuristic - you can make it more sophisticated

    if (toolpath.feedRate > 1000) {
        // High feed rate suggests rapid move
        return rapidMovePen;
    } else if (toolpath.feedRate < 50) {
        // Very low feed rate suggests plunge move
        return plungeMovePen;
    } else {
        // Normal feed rate
        return feedMovePen;
    }
}

QString ToolpathPlotter::getTitleForToolpath(const TToolpath& toolpath, size_t sequenceIndex, size_t toolpathIndex) {
    return QString("Seq%1_T%2_%3_%4")
           .arg(sequenceIndex)
           .arg(toolpath.toolNumber)
           .arg(toString(TToolpathType::Line).c_str())
           .arg(toolpathIndex);
}

void ToolpathPlotter::clearToolpaths() {
    spdlog::debug("Clearing all toolpath curves");

    for (auto& curve : toolpathCurves) {
        curve->detach();
    }
    toolpathCurves.clear();

    geometryView.replot();
}

void ToolpathPlotter::showToolpaths() {
    spdlog::debug("Showing toolpath curves");

    for (auto& curve : toolpathCurves) {
        curve->setVisible(true);
    }

    geometryView.replot();
}

void ToolpathPlotter::hideToolpaths() {
    spdlog::debug("Hiding toolpath curves");

    for (auto& curve : toolpathCurves) {
        curve->setVisible(false);
    }

    geometryView.replot();
}