//
// Created by gawain on 9/11/25.
//

#ifndef TURNLAB_DXFIMPORTPRESENTER_H
#define TURNLAB_DXFIMPORTPRESENTER_H

#include <QString>
#include <memory>

#include "Transform.h"
#include "../model/MachineConfig.h"
#include "../view/DXFImportDialog.h"
#include "../model/geometry/Geometry.h"

enum State {
    CENTER_LINE_SELECTION,
    ZERO_POINT_SELECTION,
    IDLE
};

class DXFImportPresenter : public QObject {
    Q_OBJECT

private:
    std::unique_ptr<DXFImportDialog> dialog;
    std::unique_ptr<Geometry> geometry;
    const MachineConfig& machineConfig;

    State state = IDLE;

    std::vector<Transform> transformations;

    double previousAxialOffset = 0;
    double previousRadialOffest = 0;
    QString previousUnits = "mm";

    void connectSignals();
    void updateGeometryPreview();

public:
    explicit DXFImportPresenter(const std::string& dxfFilePath, const MachineConfig& config, QWidget* parent = nullptr);
    ~DXFImportPresenter() = default;
    
    // Show the dialog and return the configuration result
    void showDialog();
    
    // Set geometry to preview
    void setGeometry(const Geometry& geometry);
    
    // Load DXF file for preview
    bool loadDXFFile(const QString& filePath);

private slots:
    void activateCenterLineSelection(bool active);
    void activateZeroPointSelection(bool active);
    void segmentSelected(size_t index);
    void pointSelected(const Point& point);

    void onRotateCW();
    void onRotateCCW();
    void onMirrorX();
    void onMirrorZ();

    void onAxialOffsetChanged(double offset);
    void onRadialOffsetChanged(double offset);
    void onUnitsChanged(const QString& units);

    void onStockStartOffsetChanged(double offset);
    void onStockEndOffsetChanged(double offset);
    void onStockRadiusChanged(double radius);

    void onImportAccepted();
    void onImportCancelled();

signals:
    void importConfigured();
    void importCancelled();
};

#endif //TURNLAB_DXFIMPORTPRESENTER_H