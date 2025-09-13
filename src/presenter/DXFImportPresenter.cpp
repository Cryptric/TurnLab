//
// Created by gawain on 9/11/25.
//

#include "DXFImportPresenter.h"
#include "../utils/DXFUtils.h"

#include <QMessageBox>
#include <QFileInfo>

DXFImportPresenter::DXFImportPresenter(QWidget* parent) : QObject(parent) {
    dialog = std::make_unique<DXFImportDialog>(parent);
    connectSignals();
}

DXFImportPresenter::DXFImportPresenter(const std::string& dxfFilePath, QWidget* parent) : QObject(parent) {
    dialog = std::make_unique<DXFImportDialog>(parent);
    connectSignals();
    
    // Load the DXF file immediately
    if (!loadDXFFile(QString::fromStdString(dxfFilePath))) {
        QMessageBox::warning(parent, "DXF Import Error", 
                           QString("Failed to load DXF file: %1").arg(dxfFilePath));
    }
}

void DXFImportPresenter::connectSignals() {
    connect(dialog.get(), &DXFImportDialog::configurationChanged, this, &DXFImportPresenter::onConfigurationChanged);
    connect(dialog.get(), &DXFImportDialog::importAccepted, this, &DXFImportPresenter::onImportAccepted);
    connect(dialog.get(), &DXFImportDialog::importCancelled, this, &DXFImportPresenter::onImportCancelled);
}

DXFImportConfiguration DXFImportPresenter::showDialog() {
    // Show the dialog modally
    dialog->exec();
    
    // Return the final configuration
    return config;
}

void DXFImportPresenter::setGeometry(const Geometry& geometry) {
    this->geometry = std::make_unique<Geometry>(geometry);
    updateGeometryPreview();
}

bool DXFImportPresenter::loadDXFFile(const QString& filePath) {
    try {
        // Check if file exists
        QFileInfo fileInfo(filePath);
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
        dialog->setGeometry(*geometry);
    }
}

void DXFImportPresenter::onConfigurationChanged() {
    // Update configuration from dialog
    config.axialOffset = dialog->getAxialOffset();
    config.radialOffset = dialog->getRadialOffset();
    config.chuckSide = dialog->getChuckSide();
    config.units = dialog->getUnits();
    
    // Update the geometry preview with new configuration
    updateGeometryPreview();
}

void DXFImportPresenter::onImportAccepted() {
    // Get final configuration
    config.axialOffset = dialog->getAxialOffset();
    config.radialOffset = dialog->getRadialOffset();
    config.chuckSide = dialog->getChuckSide();
    config.units = dialog->getUnits();
    config.accepted = true;
    
    // Emit signal with configuration
    emit importConfigured(config);
}

void DXFImportPresenter::onImportCancelled() {
    config.accepted = false;
    emit importCancelled();
}