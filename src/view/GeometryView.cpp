//
// Created by gawain on 9/9/25.
//

#include "GeometryView.h"

#include <complex>

#include <spdlog/spdlog.h>
#include <QPen>
#include <QEvent>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_map.h>

#include "Line.h"
#include "../model/geometry/Geometry.h"
#include "../model/geometry/Segment.h"



GeometryView::GeometryView(QWidget *parent) : QwtPlot(parent), magnifier(canvas()), panner(canvas()), rescaler(canvas()), hoverPicker(canvas()), clickPicker(canvas()) {
    rescaler.setAspectRatio(yLeft, 1.0);
    rescaler.setExpandingDirection(QwtPlotRescaler::ExpandBoth);
    rescaler.setRescalePolicy(QwtPlotRescaler::Expanding);

    // Setup hover picker for mouse tracking
    hoverPicker.setTrackerMode(QwtPlotPicker::AlwaysOn);
    hoverPicker.setStateMachine(new QwtPickerTrackerMachine());
    connect(&hoverPicker, &QwtPlotPicker::moved, this, &GeometryView::onHovered);
    
    // Setup click picker for mouse clicks
    clickPicker.setStateMachine(new QwtPickerClickPointMachine());
    connect(&clickPicker, QOverload<const QPointF&>::of(&QwtPlotPicker::selected), this, &GeometryView::onClicked);

    replot();
}

void GeometryView::setGeometry(const Geometry &geom) {
    geometry = geom;
    spdlog::info("Geometry loaded with {} segments", geom.segments.size());

    for (size_t i = 0; i < geom.segments.size(); i++) {
        if (auto line = dynamic_cast<Line*>(geom.segments[i].get())) {
            double xs[] = {line->p1.x, line->p2.x};
            double ys[] = {line->p1.y, line->p2.y};
            std::shared_ptr<QwtPlotCurve> curve = std::make_shared<QwtPlotCurve>("Geometry");
            curve->setSamples(xs, ys, 2);
            curve->setRenderHint(QwtPlotItem::RenderAntialiased);
            curve->setPen(normalPen);
            curve->attach(this);
            segmentPlots.push_back(curve);
        }
    }

    replot();
}

void GeometryView::setSelectedSegments(const std::vector<size_t> &selectedSegmentIndices) {
    for (const auto selectedIndex : selectedSegments) {
        segmentPlots[selectedIndex]->setPen(normalPen);
    }
    for (const auto selectedIndex : selectedSegmentIndices) {
        segmentPlots[selectedIndex]->setPen(selectedPen);
    }
    selectedSegments = selectedSegmentIndices;
}

long long GeometryView::getSegmentAtPoint(const QPointF& point) const {
    const QwtScaleMap xMap = canvasMap(xBottom);
    const double ppu = xMap.pDist() / xMap.sDist();

    for (size_t i = 0; i < geometry.segments.size(); ++i) {
        const double distance = geometry.segments[i]->distance(Point(point.x(), point.y()));
        const double distancePx = distance * ppu;
        if (distancePx <= HOVER_TOLERANCE_PX) {
            return static_cast<long long>(i);
        }
    }
    return -1;
}

void GeometryView::onHovered(const QPointF& point) {
    long long hovering = getSegmentAtPoint(point);
    
    if (hovering != -1) {
        spdlog::trace("Hovering over segment {}", hovering);
    }
    
    if (hoveredSegmentIndex != -1 && hoveredSegmentIndex != hovering) {
        segmentPlots[hoveredSegmentIndex]->setPen(hoverPenCopy);
        replot();
    } else if (hovering != -1 && hoveredSegmentIndex != hovering) {
        hoverPenCopy = segmentPlots[hovering]->pen();
        segmentPlots[hovering]->setPen(hoverPen);
        replot();
    }
    hoveredSegmentIndex = hovering;
}

void GeometryView::onClicked(const QPointF& point) {
    const long long segmentIndex = getSegmentAtPoint(point);
    if (segmentIndex != -1) {
        spdlog::info("Clicked on segment {}", segmentIndex);
        emit segmentSelected(static_cast<int>(segmentIndex));
    }
}