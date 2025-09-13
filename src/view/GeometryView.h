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

#include "../model/geometry/Geometry.h"

#define HOVER_TOLERANCE_PX 5.0

class GeometryView : public QwtPlot {
    Q_OBJECT

private:
    const QPen normalPen = QPen(Qt::blue, 2.0, Qt::SolidLine);
    const QPen selectedPen = QPen(Qt::red, 2.0, Qt::SolidLine);
    const QPen hoverPen = QPen(Qt::green, 2.0, Qt::SolidLine);

    QwtPlotMagnifier magnifier;
    QwtPlotPanner panner;
    QwtPlotRescaler rescaler;
    QwtPlotPicker hoverPicker;
    QwtPlotPicker clickPicker;

    Geometry geometry;
    std::vector<std::shared_ptr<QwtPlotCurve>> segmentPlots;
    std::vector<size_t> selectedSegments;

    long long hoveredSegmentIndex = -1;
    QPen hoverPenCopy = normalPen;

public:
    explicit GeometryView(QWidget *parent = nullptr);
    ~GeometryView() override = default;

public slots:
    void setGeometry(const Geometry &geom);
    void setSelectedSegments(const std::vector<size_t> &selectedSegmentIndices);

    long long getSegmentAtPoint(const QPointF &point) const;

private slots:
    void onHovered(const QPointF& point);
    void onClicked(const QPointF& point);

signals:
    void segmentSelected(int segmentIndex);

};


#endif //TURNLAB_GEOMETRYVIEW_H