//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_GEOMETRYVIEW_H
#define TURNLAB_GEOMETRYVIEW_H

#include <vector>
#include <memory>

#include <QPen>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_rescaler.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

#include "../model/geometry/Geometry.h"
#include "../model/MachineConfig.h"

#define HOVER_TOLERANCE_PX 5.0

#define POINT_SYMBOL new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::black), QPen(Qt::black), QSize(10, 10))
#define POINT_HOVER_SYMBOL new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::green), QPen(Qt::green), QSize(10, 10))

class GeometryView : public QwtPlot {
    Q_OBJECT

private:
    const QPen normalPen = QPen(Qt::blue, 2.0, Qt::SolidLine);
    const QPen selectedPen = QPen(Qt::red, 2.0, Qt::SolidLine);
    const QPen hoverPen = QPen(Qt::green, 2.0, Qt::SolidLine);
    const QPen xAxisPen = QPen(Qt::red, 1.5, Qt::SolidLine);
    const QPen yAxisPen = QPen(Qt::green, 1.5, Qt::SolidLine);

    QwtPlotMagnifier magnifier;
    QwtPlotPanner panner;
    QwtPlotRescaler rescaler;
    QwtPlotPicker hoverPicker;
    QwtPlotPicker clickPicker;
    QwtPlotGrid grid;
    QwtPlotMarker xAxisMarker;
    QwtPlotMarker yAxisMarker;

    const MachineConfig& machineConfig;

    Geometry geometry;
    std::vector<std::shared_ptr<QwtPlotCurve>> segmentPlots;
    std::vector<size_t> selectedSegments;

    std::vector<std::shared_ptr<QwtPlotCurve>> pointPlots;
    std::vector<Point> points;

    long long hoveredSegmentIndex = -1;
    QPen hoverPenCopy = normalPen;

    bool pointPicking = false;
    long long hoveredPointIndex = -1;

public:
    explicit GeometryView(const MachineConfig& config, QWidget *parent = nullptr);

    void setAxesOrientation();
    void setupGrid();

    void enablePointPicking();
    void disablePointPicking();

    ~GeometryView() override = default;

public slots:
    void setGeometry(const Geometry &geom);
    void setSelectedSegments(const std::vector<size_t> &selectedSegmentIndices);

    long long getSegmentAtPoint(const QPointF &point) const;
    long long getPointAtPoint(const QPointF &point) const;

private slots:
    void onHovered(const QPointF& point);
    void onClicked(const QPointF& point);

signals:
    void segmentSelected(int segmentIndex);
    void pointSelected(const Point& point);

};


#endif //TURNLAB_GEOMETRYVIEW_H