//
// Created by gawain on 9/9/25.
//

#include "GeometryView.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>

#include <cmath>

#include "../model/geometry/Geometry.h"
#include "../model/geometry/Segment.h"



GeometryView::GeometryView(QWidget *parent) : QChartView(parent) {
    QLineSeries *series = new QLineSeries();
    for (double x = 0.0; x <= 5.0; x += 0.1)
        series->append(x, std::sin(x) * 5.0 + 5.0);


    chart = new QChart();
    chart->addSeries(series);
    chart->setTheme(QChart::ChartThemeDark);
    setChart(chart);
}

void GeometryView::setGeometry(const Geometry &geometry) {
    // TODO: Implement geometry loading and display
}

void GeometryView::setSelectedSegments(std::vector<size_t> selectedSegmentIndices) {
    // TODO: Implement segment selection visualization
}