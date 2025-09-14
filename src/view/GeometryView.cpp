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
#include <qwt_scale_engine.h>
#include <qwt_scale_draw.h>

#include "DXFImportDialog.h"
#include "Line.h"
#include "Vector.h"
#include "../model/geometry/Geometry.h"
#include "../model/geometry/Segment.h"



GeometryView::GeometryView(const MachineConfig& config, QWidget *parent) : QwtPlot(parent), magnifier(canvas()), panner(canvas()), rescaler(canvas()), hoverPicker(canvas()), clickPicker(canvas()), machineConfig(config) {
    rescaler.setAspectRatio(yLeft, 1.0);
    rescaler.setExpandingDirection(QwtPlotRescaler::ExpandBoth);
    rescaler.setRescalePolicy(QwtPlotRescaler::Expanding);

    setAxesOrientation();
    setupGrid();

    // Setup hover picker for mouse tracking
    hoverPicker.setTrackerMode(QwtPlotPicker::AlwaysOn);
    hoverPicker.setStateMachine(new QwtPickerTrackerMachine());
    connect(&hoverPicker, &QwtPlotPicker::moved, this, &GeometryView::onHovered);

    // Setup click picker for mouse clicks
    clickPicker.setStateMachine(new QwtPickerClickPointMachine());
    connect(&clickPicker, QOverload<const QPointF&>::of(&QwtPlotPicker::selected), this, &GeometryView::onClicked);

    plotStock({-1, 10, 3});

    replot();
}

void GeometryView::setAxesOrientation() {
    // Set directions
    if (machineConfig.zAxisDirection == AxisDirection::Negative) {
        setAxisScaleEngine(xBottom, new QwtLinearScaleEngine());
        axisScaleEngine(xBottom)->setAttribute(QwtScaleEngine::Inverted, true);
    }
    if (machineConfig.xAxisDirection == AxisDirection::Positive) {
        setAxisScaleEngine(yLeft, new QwtLinearScaleEngine());
        axisScaleEngine(yLeft)->setAttribute(QwtScaleEngine::Inverted, true);
    }
    replot();
}

void GeometryView::setupGrid() {
    // Setup grid with subtle color
    grid.setMajorPen(QPen(Qt::lightGray, 0.5, Qt::DashLine));
    grid.setMinorPen(QPen(Qt::lightGray, 0.25, Qt::DotLine));

    grid.enableX(true);
    grid.enableY(true);
    grid.enableXMin(true);
    grid.enableYMin(true);

    grid.attach(this);

    // Setup X-axis (horizontal line at Y=0) with red color
    xAxisMarker.setLineStyle(QwtPlotMarker::HLine);
    xAxisMarker.setLinePen(xAxisPen);
    xAxisMarker.setYValue(0.0);
    xAxisMarker.setZ(-10.0);  // Lower Z-order to render underneath geometry
    xAxisMarker.attach(this);

    // Setup Y-axis (vertical line at X=0) with green color
    yAxisMarker.setLineStyle(QwtPlotMarker::VLine);
    yAxisMarker.setLinePen(yAxisPen);
    yAxisMarker.setXValue(0.0);
    yAxisMarker.setZ(-10.0);  // Lower Z-order to render underneath geometry
    yAxisMarker.attach(this);
}

void GeometryView::setGeometry(const Geometry &geom) {
    geometry = geom;
    spdlog::info("Geometry loaded with {} segments", geom.segments.size());

    segmentPlots.clear();
    pointPlots.clear();

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

void GeometryView::plotStock(const StockMaterial& stock) {
    stockPlot = std::make_unique<QwtPlotShapeItem>("Stock");
    QRectF rect(stock.startPosition, -stock.radius, stock.endPosition - stock.startPosition, stock.radius * 2);
    stockPlot->setRect(rect);
    stockPlot->setPen(QPen(STOCK_COLOR, 1.0, Qt::SolidLine));
    stockPlot->setBrush(QBrush(STOCK_COLOR, Qt::SolidPattern));
    stockPlot->attach(this);
    replot();
}

void GeometryView::enablePointPicking() {
    pointPlots.clear();
    points.clear();

    for (size_t i = 0; i < geometry.segments.size(); i++) {
        if (auto line = dynamic_cast<Line*>(geometry.segments[i].get())) {
            if (std::find(points.begin(), points.end(), line->p1) == points.end()) {
                points.push_back(line->p1);
            }
            if (std::find(points.begin(), points.end(), line->p2) == points.end()) {
                points.push_back(line->p2);
            }
        }
    }

    spdlog::debug("Picking from: {} points", points.size());

    for (auto p : points) {
        std::shared_ptr<QwtPlotCurve> pointPlot = std::make_shared<QwtPlotCurve>("Point");
        const double px[] = {p.x};
        const double py[] = {p.y};
        pointPlot->setSamples(px, py, 1);
        pointPlot->setRenderHint(QwtPlotItem::RenderAntialiased);
        pointPlot->setSymbol(POINT_SYMBOL);
        pointPlot->attach(this);
        pointPlots.push_back(pointPlot);
    }
    pointPicking = true;
    replot();
}

void GeometryView::disablePointPicking() {
    pointPlots.clear();
    points.clear();
    pointPicking = false;
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

long long GeometryView::getPointAtPoint(const QPointF& point) const {
    const QwtScaleMap xMap = canvasMap(xBottom);
    const double ppu = xMap.pDist() / xMap.sDist();

    for (size_t i = 0; i < points.size(); ++i) {
        const double distance = Vector({point.x(), point.y()}, points[i]).norm();
        const double distancePx = distance * ppu;
        if (distancePx <= HOVER_TOLERANCE_PX) {
            return static_cast<long long>(i);
        }
    }
    return -1;
}

void GeometryView::onHovered(const QPointF& point) {
    if (pointPicking) {
        long long hovering = getPointAtPoint(point);
        if (hovering != -1) {
            spdlog::trace("Hovering over point {}", hovering);
        }
        if (hoveredPointIndex != -1 && hoveredPointIndex != hovering) {
            pointPlots[hoveredPointIndex]->setSymbol(POINT_SYMBOL);
            replot();
        }
        if (hovering != -1 && hoveredPointIndex != hovering) {
            pointPlots[hovering]->setSymbol(POINT_HOVER_SYMBOL);
            replot();
        }
        hoveredPointIndex = hovering;
    } else {
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
}

void GeometryView::onClicked(const QPointF& point) {
    if (pointPicking) {
        const long long pointIndex = getPointAtPoint(point);
        if (pointIndex != -1) {
            spdlog::info("Clicked on point {}", pointIndex);
            emit pointSelected(points[pointIndex]);
        }
    } else {
        const long long segmentIndex = getSegmentAtPoint(point);
        if (segmentIndex != -1) {
            spdlog::info("Clicked on segment {}", segmentIndex);
            emit segmentSelected(static_cast<int>(segmentIndex));
        }
    }
}