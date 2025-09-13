//
// Created by gawain on 9/11/25.
//

#include "DXFImportDialog.h"

DXFImportDialog::DXFImportDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("DXF Import Configuration");
    setModal(true);
    resize(800, 600);
    
    setupLayout();
    setupConfigurationPanel();
    connectSignals();
}

void DXFImportDialog::setupLayout() {
    // Create main vertical layout
    auto* mainLayout = new QVBoxLayout(this);
    
    // Create horizontal splitter for center content (geometry view + config panel)
    auto* splitter = new QSplitter(Qt::Horizontal, this);
    
    // Create geometry view for the left/center
    geometryView = new GeometryView(this);
    splitter->addWidget(geometryView);
    
    // Create configuration panel for the right side
    auto* configGroup = new QGroupBox("Import Configuration", this);
    configGroup->setFixedWidth(300); // Fixed width for configuration panel
    splitter->addWidget(configGroup);
    
    // Set splitter proportions (geometry view gets more space)
    splitter->setStretchFactor(0, 1); // GeometryView stretches
    splitter->setStretchFactor(1, 0); // ConfigPanel fixed
    
    mainLayout->addWidget(splitter);
    
    // Create button panel for the bottom
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    okButton = buttonBox->button(QDialogButtonBox::Ok);
    cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox);
    
    setLayout(mainLayout);
}

void DXFImportDialog::setupConfigurationPanel() {
    // Get the config group that was created in setupLayout()
    auto* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    auto* splitter = qobject_cast<QSplitter*>(mainLayout->itemAt(0)->widget());
    auto* configGroup = qobject_cast<QGroupBox*>(splitter->widget(1));
    
    auto* formLayout = new QFormLayout(configGroup);
    
    // Axial offset control
    axialOffsetSpinBox = new QDoubleSpinBox(this);
    axialOffsetSpinBox->setRange(-1000.0, 1000.0);
    axialOffsetSpinBox->setDecimals(3);
    axialOffsetSpinBox->setSuffix(" mm");
    axialOffsetSpinBox->setValue(0.0);
    formLayout->addRow("Axial Offset:", axialOffsetSpinBox);
    
    // Radial offset control
    radialOffsetSpinBox = new QDoubleSpinBox(this);
    radialOffsetSpinBox->setRange(-1000.0, 1000.0);
    radialOffsetSpinBox->setDecimals(3);
    radialOffsetSpinBox->setSuffix(" mm");
    radialOffsetSpinBox->setValue(0.0);
    formLayout->addRow("Radial Offset:", radialOffsetSpinBox);
    
    // Chuck side selection
    chuckSideCombo = new QComboBox(this);
    chuckSideCombo->addItem("Left");
    chuckSideCombo->addItem("Right");
    chuckSideCombo->setCurrentText("Left");
    formLayout->addRow("Chuck Side:", chuckSideCombo);
    
    // Units selection
    unitsCombo = new QComboBox(this);
    unitsCombo->addItem("mm");
    unitsCombo->addItem("m");
    unitsCombo->addItem("inch");
    unitsCombo->setCurrentText("mm");
    formLayout->addRow("Units:", unitsCombo);
    
    configGroup->setLayout(formLayout);
}

void DXFImportDialog::connectSignals() {
    // Connect configuration change signals
    connect(axialOffsetSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DXFImportDialog::onConfigurationChanged);
    connect(radialOffsetSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &DXFImportDialog::onConfigurationChanged);
    connect(chuckSideCombo, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &DXFImportDialog::onConfigurationChanged);
    connect(unitsCombo, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &DXFImportDialog::onConfigurationChanged);
    
    // Connect button signals
    connect(okButton, &QPushButton::clicked, this, &DXFImportDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &DXFImportDialog::onCancelClicked);
}

double DXFImportDialog::getAxialOffset() const {
    return axialOffsetSpinBox->value();
}

double DXFImportDialog::getRadialOffset() const {
    return radialOffsetSpinBox->value();
}

QString DXFImportDialog::getChuckSide() const {
    return chuckSideCombo->currentText();
}

QString DXFImportDialog::getUnits() const {
    return unitsCombo->currentText();
}

void DXFImportDialog::setGeometry(const Geometry& geometry) {
    geometryView->setGeometry(geometry);
}

void DXFImportDialog::onConfigurationChanged() {
    emit configurationChanged();
}

void DXFImportDialog::onOkClicked() {
    emit importAccepted();
    accept();
}

void DXFImportDialog::onCancelClicked() {
    emit importCancelled();
    reject();
}