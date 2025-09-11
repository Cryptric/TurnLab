//
// Created by gawain on 9/9/25.
//

#ifndef TURNLAB_GEOMETRYVIEW_H
#define TURNLAB_GEOMETRYVIEW_H

#include <QtCharts/QChartView>
#include <vector>

#include "../model/geometry/Segment.h"
#include "../model/geometry/Geometry.h"

class GeometryView : public QChartView {
    Q_OBJECT

    QChart* chart;


public:
    explicit GeometryView(QWidget *parent = nullptr);
    ~GeometryView() override = default;

public slots:
    void setGeometry(const Geometry& geometry);
    void setSelectedSegments(std::vector<size_t> selectedSegmentIndices);

signals:
    void segmentSelected(int segmentIndex);

};


#endif //TURNLAB_GEOMETRYVIEW_H