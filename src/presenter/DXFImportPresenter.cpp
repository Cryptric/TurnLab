//
// Created by gawain on 9/11/25.
//

#include "DXFImportPresenter.h"
#include "../utils/DXFUtils.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <spdlog/spdlog.h>


#include "Line.h"
#include "../model/MachineConfig.h"

DXFImportPresenter::DXFImportPresenter(const std::string& dxfFilePath, const MachineConfig& config, QWidget* parent) : machineConfig(config), QObject(parent) {
    dialog = std::make_unique<DXFImportDialog>(machineConfig, parent);
    connectSignals();
    
    // Load the DXF file immediately
    if (!loadDXFFile(QString::fromStdString(dxfFilePath))) {
        QMessageBox::warning(parent, "DXF Import Error",
                           QString("Failed to load DXF file: %1").arg(QString::fromStdString(dxfFilePath)));
    }
}

void DXFImportPresenter::connectSignals() {
    connect(dialog.get(), &DXFImportDialog::activateCenterLineSelection, this, &DXFImportPresenter::activateCenterLineSelection);
    connect(dialog.get(), &DXFImportDialog::activateZeroPointSelection, this, &DXFImportPresenter::activateZeroPointSelection);
    connect(dialog.get(), &DXFImportDialog::segmentSelected, this, &DXFImportPresenter::segmentSelected);
    connect(dialog.get(), &DXFImportDialog::pointSelected, this, &DXFImportPresenter::pointSelected);

    connect(dialog.get(), &DXFImportDialog::onRotateCW, this, &DXFImportPresenter::onRotateCW);
    connect(dialog.get(), &DXFImportDialog::onRotateCCW, this, &DXFImportPresenter::onRotateCCW);
    connect(dialog.get(), &DXFImportDialog::onMirrorX, this, &DXFImportPresenter::onMirrorX);
    connect(dialog.get(), &DXFImportDialog::onMirrorZ, this, &DXFImportPresenter::onMirrorZ);

    connect(dialog.get(), &DXFImportDialog::onAxialOffsetChanged, this, &DXFImportPresenter::onAxialOffsetChanged);
    connect(dialog.get(), &DXFImportDialog::onRadialOffsetChanged, this, &DXFImportPresenter::onRadialOffsetChanged);
    connect(dialog.get(), &DXFImportDialog::onUnitsChanged, this, &DXFImportPresenter::onUnitsChanged);

    connect(dialog.get(), &DXFImportDialog::onStockStartOffsetChanged, this, &DXFImportPresenter::onStockStartOffsetChanged);
    connect(dialog.get(), &DXFImportDialog::onStockEndOffsetChanged, this, &DXFImportPresenter::onStockEndOffsetChanged);
    connect(dialog.get(), &DXFImportDialog::onStockRadiusChanged, this, &DXFImportPresenter::onStockRadiusChanged);

    connect(dialog.get(), &DXFImportDialog::nextStageRequested, this, &DXFImportPresenter::onNextStageRequested);
    connect(dialog.get(), &DXFImportDialog::previousStageRequested, this, &DXFImportPresenter::onPreviousStageRequested);

    connect(dialog.get(), &DXFImportDialog::importAccepted, this, &DXFImportPresenter::onImportAccepted);
    connect(dialog.get(), &DXFImportDialog::importCancelled, this, &DXFImportPresenter::onImportCancelled);
}

std::optional<Project> DXFImportPresenter::showDialog() const {
    int result = dialog->exec();

    // get path where to save project
    QString projectFilePath = QFileDialog::getSaveFileName(nullptr, "Save Project", "", "TurnLab project file (*.turnlab)");

    if (result == QDialog::Accepted && !projectFilePath.isEmpty()) {
        // Import button was pressed
        Project project = {
            .savePath = projectFilePath.toStdString(),
            .geometry = geometry->transform(transformations),
            .stockMaterial = stockMaterial
        };
        return project;
    } else {
        // Cancel or close was pressed
        return {}; // Return empty project
    }
}

void DXFImportPresenter::setGeometry(const Geometry& geom) {
    this->geometry = std::make_unique<Geometry>(geom);
    updateGeometryPreview();
}

bool DXFImportPresenter::loadDXFFile(const QString& filePath) {
    try {
        // Check if file exists
        const QFileInfo fileInfo(filePath);
        if (!fileInfo.exists() || !fileInfo.isReadable()) {
            return false;
        }
        
        // Load DXF file using DXFUtils
        Geometry loadedGeometry = loadDXF(filePath.toStdString());
        setGeometry(loadedGeometry);
        
        // Set dialog title to include filename
        dialog->setWindowTitle(QString("DXF Import Configuration - %1").arg(fileInfo.fileName()));
        
        return true;
    }
    catch (const std::exception& e) {
        // Handle DXF loading errors
        return false;
    }
}

void DXFImportPresenter::updateGeometryPreview() {
    if (geometry && dialog) {
        // Apply current configuration transformations here if needed
        // For now, just pass the geometry to the dialog
        dialog->setGeometry(geometry->transform(transformations));
    }
}

void DXFImportPresenter::activateCenterLineSelection(bool active) {
    if (state == ZERO_POINT_SELECTION) {
        dialog->deactivateZeroPointSelection();
    }
    state = active ? CENTER_LINE_SELECTION : IDLE;
}

void DXFImportPresenter::activateZeroPointSelection(bool active) {
    if (state == CENTER_LINE_SELECTION) {
        dialog->deactivateCenterLineSelection();
    }
    if (active) {
        state = ZERO_POINT_SELECTION;
    } else {
        state = IDLE;
        dialog->deactivateZeroPointSelection();
    }
}

void DXFImportPresenter::segmentSelected(size_t index) {
    if (state == CENTER_LINE_SELECTION) {
        if (Line* line = dynamic_cast<Line*>(geometry->segments[index].get())) {
            spdlog::trace("Center line selected");
            Line lineTransformed = *dynamic_cast<Line*>(line->transform(transformations).get());
            double phiInv = std::atan2(lineTransformed.p2.y - lineTransformed.p1.y, lineTransformed.p2.x - lineTransformed.p1.x);
            Transform t = Transform::rotate(-phiInv);
            Point p1Transformed = lineTransformed.p1.apply(t);
            t.translation[1] = -p1Transformed.y;
            transformations.push_back(t);
            updateGeometryPreview();
            dialog->deactivateCenterLineSelection();
            state = IDLE;
        }
        // if it is not a line, do nothing
    }
}

void DXFImportPresenter::pointSelected(const Point& point) {
    if (state == ZERO_POINT_SELECTION) {
        const Transform t = Transform::translate(-point.x, -point.y);
        transformations.push_back(t);
        updateGeometryPreview();
        dialog->deactivateZeroPointSelection();
        state = IDLE;
    }
}

void DXFImportPresenter::onRotateCW() {
    spdlog::trace("Rotating geometry 90 degrees clockwise");
    transformations.push_back(Transform::rotate(-M_PI_2));
    updateGeometryPreview();
}

void DXFImportPresenter::onRotateCCW() {
    spdlog::trace("Rotating geometry 90 degrees counter-clockwise");
    transformations.push_back(Transform::rotate(M_PI_2));
    updateGeometryPreview();
}

void DXFImportPresenter::onMirrorX() {
    spdlog::trace("Mirroring geometry along X-axis");
    transformations.push_back(Transform::mirrorY());
    updateGeometryPreview();
}

void DXFImportPresenter::onMirrorZ() {
    spdlog::trace("Mirroring geometry along Z-axis");
    transformations.push_back(Transform::mirrorX());
    updateGeometryPreview();
}

void DXFImportPresenter::onAxialOffsetChanged(double offset) {
    spdlog::trace("Applying axial offset of {} mm", offset);
    const double delta = offset - previousAxialOffset;
    const Transform t = Transform::translate(0.0, delta);
    transformations.push_back(t);
    previousAxialOffset = offset;
    updateGeometryPreview();
}

void DXFImportPresenter::onRadialOffsetChanged(double offset) {
    spdlog::trace("Applying radial offset of {} mm", offset);
    const double delta = offset - previousRadialOffest;
    const Transform t = Transform::translate(delta, 0.0);
    transformations.push_back(t);
    previousRadialOffest = offset;
    updateGeometryPreview();
}

void DXFImportPresenter::onUnitsChanged(const QString& units) {
    spdlog::trace("Units changed from {} to {}", previousUnits.toStdString(), units.toStdString());

    if (previousUnits == units) {
        return; // No change needed
    }

    double scaleFactor = 1.0;

    // Calculate scale factor for conversion
    // Convert to mm as base unit, then to target unit
    double fromMmToBase = 1.0; // Factor from mm to current unit
    double toMmFromTarget = 1.0; // Factor from target unit to mm

    // Convert previous unit to mm factor
    if (previousUnits == "mm") {
        fromMmToBase = 1.0;
    } else if (previousUnits == "m") {
        fromMmToBase = 1000.0; // 1 m = 1000 mm
    } else if (previousUnits == "inch") {
        fromMmToBase = 25.4; // 1 inch = 25.4 mm
    }

    // Convert target unit from mm factor
    if (units == "mm") {
        toMmFromTarget = 1.0;
    } else if (units == "m") {
        toMmFromTarget = 1000.0; // 1 m = 1000 mm
    } else if (units == "inch") {
        toMmFromTarget = 25.4; // 1 inch = 25.4 mm
    }

    scaleFactor = fromMmToBase / toMmFromTarget;

    if (scaleFactor != 1.0) {
        spdlog::trace("Applying scale factor {} for units conversion", scaleFactor);
        transformations.push_back(Transform::scale(scaleFactor, scaleFactor));
        updateGeometryPreview();
    }

    previousUnits = units;
}

void DXFImportPresenter::onStockStartOffsetChanged(double offset) {
    spdlog::trace("Stock start position changed to {} mm", offset);
    stockMaterial.startPosition = offset;
    updateStockPreview();
}

void DXFImportPresenter::onStockEndOffsetChanged(double offset) {
    spdlog::trace("Stock end position changed to {} mm", offset);
    stockMaterial.endPosition = offset;
    updateStockPreview();
}

void DXFImportPresenter::onStockRadiusChanged(double radius) {
    spdlog::trace("Stock radius changed to {} mm", radius);
    stockMaterial.radius = radius;
    updateStockPreview();
}

void DXFImportPresenter::updateStockPreview() {
    if (dialog) {
        dialog->setStock(stockMaterial);
    }
}

void DXFImportPresenter::onNextStageRequested() {
    spdlog::trace("Transitioning to stock setup stage");
    if (dialog) {
        dialog->switchToStage(STOCK_SETUP);
        double minPos = INFINITY;
        double maxPos = -INFINITY;
        double radius = 0.0;
        Geometry transformedGeometry = geometry->transform(transformations);
        for (const auto& segment : transformedGeometry.segments) {
            if (Line* line = dynamic_cast<Line*>(segment.get())) {
                minPos = std::min(minPos, line->p1.x);
                minPos = std::min(minPos, line->p2.x);
                maxPos = std::max(maxPos, line->p1.x);
                maxPos = std::max(maxPos, line->p2.x);
                radius = std::max(radius, line->p1.y);
                radius = std::max(radius, line->p2.y);
            }
        }
        stockMaterial.startPosition = minPos;
        stockMaterial.endPosition = maxPos;
        stockMaterial.radius = radius;
        updateStockPreview();
    }
}

void DXFImportPresenter::onPreviousStageRequested() {
    spdlog::trace("Transitioning back to geometry setup stage");
    if (dialog) {
        dialog->hideStock();
        dialog->switchToStage(GEOMETRY_SETUP);
    }
}

void DXFImportPresenter::onImportAccepted() {
    emit importConfigured();
}

void DXFImportPresenter::onImportCancelled() {
    emit importCancelled();
}