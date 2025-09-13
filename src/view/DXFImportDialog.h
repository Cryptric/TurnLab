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
#include "../model/geometry/Geometry.h"

class DXFImportDialog : public QDialog {
    Q_OBJECT

private:
    GeometryView* geometryView;
    
    // Configuration controls
    QDoubleSpinBox* axialOffsetSpinBox;
    QDoubleSpinBox* radialOffsetSpinBox;
    QComboBox* chuckSideCombo;
    QComboBox* unitsCombo;
    
    // Dialog buttons
    QPushButton* okButton;
    QPushButton* cancelButton;
    
    void setupLayout();
    void setupConfigurationPanel();
    void connectSignals();

public:
    explicit DXFImportDialog(QWidget *parent = nullptr);
    ~DXFImportDialog() override = default;
    
    // Configuration accessors
    double getAxialOffset() const;
    double getRadialOffset() const;
    QString getChuckSide() const;
    QString getUnits() const;
    
    // Geometry display
    void setGeometry(const Geometry& geometry);

public slots:
    void onConfigurationChanged();
    void onOkClicked();
    void onCancelClicked();

signals:
    void configurationChanged();
    void importAccepted();
    void importCancelled();
};

#endif //TURNLAB_DXFIMPORTDIALOG_H