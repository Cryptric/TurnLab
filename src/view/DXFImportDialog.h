//
// Created by gawain on 9/11/25.
//

#ifndef TURNLAB_DXFIMPORTDIALOG_H
#define TURNLAB_DXFIMPORTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QSplitter>
#include <QDialogButtonBox>

#include "GeometryView.h"
#include "../model/MachineConfig.h"
#include "../model/geometry/Geometry.h"

enum ImportStage {
    GEOMETRY_SETUP,
    STOCK_SETUP
};

class DXFImportDialog : public QDialog {
    Q_OBJECT

private:
    GeometryView* geometryView;
    ImportStage currentStage = GEOMETRY_SETUP;
    QGroupBox* configGroup;

    // Container widgets for each stage
    QWidget* geometryWidget;
    QWidget* stockWidget;

    // Geometry setup controls
    QPushButton* centerLineSelection;
    QPushButton* zeroPointSelection;
    QPushButton* rotateCCW;
    QPushButton* rotateCW;
    QPushButton* mirrorX;
    QPushButton* mirrorZ;
    QDoubleSpinBox* axialOffsetSpinBox;
    QDoubleSpinBox* radialOffsetSpinBox;
    QComboBox* unitsCombo;

    // Stock setup controls
    QDoubleSpinBox* stockStartSpinBox;
    QDoubleSpinBox* stockEndSpinBox;
    QDoubleSpinBox* stockRadiusSpinBox;

    // Dialog buttons
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* nextButton;
    QPushButton* backButton;

    MachineConfig machineConfig;

    void setupLayout();
    void setupConfigurationPanel();
    void setupGeometryPanel();
    void setupStockPanel();
    void connectSignals();

public:
    explicit DXFImportDialog(const MachineConfig& config, QWidget *parent = nullptr);
    ~DXFImportDialog() override = default;
    

    // Geometry display
    void setGeometry(const Geometry& geometry);

    void setStock(const StockMaterial &stock);
    void hideStock();

    // Stage control
    void switchToStage(ImportStage stage);

public slots:
    void deactivateCenterLineSelection();
    void deactivateZeroPointSelection();

    void onOkClicked();
    void onCancelClicked();

signals:
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

    void nextStageRequested();
    void previousStageRequested();

    void importAccepted();
    void importCancelled();
};

#endif //TURNLAB_DXFIMPORTDIALOG_H