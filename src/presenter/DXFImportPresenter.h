//
// Created by gawain on 9/11/25.
//

#ifndef TURNLAB_DXFIMPORTPRESENTER_H
#define TURNLAB_DXFIMPORTPRESENTER_H

#include <QString>
#include <memory>

#include "../view/DXFImportDialog.h"
#include "../model/geometry/Geometry.h"

struct DXFImportConfiguration {
    double axialOffset = 0.0;
    double radialOffset = 0.0;
    QString chuckSide = "Left";
    QString units = "mm";
    bool accepted = false;
};

class DXFImportPresenter : public QObject {
    Q_OBJECT

private:
    std::unique_ptr<DXFImportDialog> dialog;
    std::unique_ptr<Geometry> geometry;
    DXFImportConfiguration config;
    
    void connectSignals();
    void updateGeometryPreview();

public:
    explicit DXFImportPresenter(QWidget* parent = nullptr);
    explicit DXFImportPresenter(const std::string& dxfFilePath, QWidget* parent = nullptr);
    ~DXFImportPresenter() = default;
    
    // Show the dialog and return the configuration result
    DXFImportConfiguration showDialog();
    
    // Set geometry to preview
    void setGeometry(const Geometry& geometry);
    
    // Load DXF file for preview
    bool loadDXFFile(const QString& filePath);

private slots:
    void onConfigurationChanged();
    void onImportAccepted();
    void onImportCancelled();

signals:
    void importConfigured(const DXFImportConfiguration& config);
    void importCancelled();
};

#endif //TURNLAB_DXFIMPORTPRESENTER_H