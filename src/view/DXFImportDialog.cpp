//
// Created by gawain on 9/11/25.
//

#include "DXFImportDialog.h"

DXFImportDialog::DXFImportDialog(const MachineConfig& config, QWidget *parent) : machineConfig(config), QDialog(parent) {
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
    geometryView = new GeometryView(machineConfig, this);
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

    // Select middle line
    centerLineSelection = new QPushButton("Select Middle Line", this);
    centerLineSelection->setCheckable(true);
    connect(centerLineSelection, &QPushButton::toggled, [this](const bool checked) { emit activateCenterLineSelection(checked); });
    formLayout->addRow("Middle Line:", centerLineSelection);

    // Zero point selection
    zeroPointSelection = new QPushButton("Select Zero Point", this);
    zeroPointSelection->setCheckable(true);
    connect(zeroPointSelection, &QPushButton::toggled, [this](const bool checked) {
        geometryView->enablePointPicking();
        emit activateZeroPointSelection(checked);
    });
    formLayout->addRow("Zero Point:", zeroPointSelection);

    // Rotation
    rotateCCW = new QPushButton(QIcon(":/res/icons/rotate-ccw"), "", this);
    rotateCW = new QPushButton(QIcon(":/res/icons/rotate-cw"), "", this);
    connect(rotateCCW, &QPushButton::clicked, [this]() { emit onRotateCCW(); });
    connect(rotateCW, &QPushButton::clicked, [this]() { emit onRotateCW(); });
    QHBoxLayout* rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(rotateCCW);
    rotationLayout->addWidget(rotateCW);
    formLayout->addRow("Rotation:", rotationLayout);

    // Mirror
    mirrorX = new QPushButton(QIcon(":/res/icons/mirror-x.png"), "", this);
    mirrorZ = new QPushButton(QIcon(":/res/icons/mirror-z.png"), "", this);
    connect(mirrorX, &QPushButton::clicked, [this]() { emit onMirrorX(); });
    connect(mirrorZ, &QPushButton::clicked, [this]() { emit onMirrorZ(); });
    QHBoxLayout* mirrorLayout = new QHBoxLayout();
    mirrorLayout->addWidget(mirrorX);
    mirrorLayout->addWidget(mirrorZ);
    formLayout->addRow("Mirror:", mirrorLayout);
    
    // Axial offset control
    axialOffsetSpinBox = new QDoubleSpinBox(this);
    axialOffsetSpinBox->setRange(-1000.0, 1000.0);
    axialOffsetSpinBox->setDecimals(3);
    axialOffsetSpinBox->setSuffix(" mm");
    axialOffsetSpinBox->setValue(0.0);
    connect(axialOffsetSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) { emit onAxialOffsetChanged(value); });
    formLayout->addRow("Axial Offset:", axialOffsetSpinBox);
    
    // Radial offset control
    radialOffsetSpinBox = new QDoubleSpinBox(this);
    radialOffsetSpinBox->setRange(-1000.0, 1000.0);
    radialOffsetSpinBox->setDecimals(3);
    radialOffsetSpinBox->setSuffix(" mm");
    radialOffsetSpinBox->setValue(0.0);
    connect(radialOffsetSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) { emit onRadialOffsetChanged(value); });
    formLayout->addRow("Radial Offset:", radialOffsetSpinBox);

    // Units selection
    unitsCombo = new QComboBox(this);
    unitsCombo->addItem("mm");
    unitsCombo->addItem("m");
    unitsCombo->addItem("inch");
    unitsCombo->setCurrentText("mm");
    connect(unitsCombo, QOverload<const QString&>::of(&QComboBox::currentTextChanged), [this](const QString& text) { emit onUnitsChanged(text); });
    formLayout->addRow("Units:", unitsCombo);
    
    configGroup->setLayout(formLayout);
}

void DXFImportDialog::connectSignals() {
    connect(geometryView, &GeometryView::segmentSelected, this, &DXFImportDialog::segmentSelected);
    connect(geometryView, &GeometryView::pointSelected, this, &DXFImportDialog::pointSelected);

    connect(okButton, &QPushButton::clicked, this, &DXFImportDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &DXFImportDialog::onCancelClicked);
}

void DXFImportDialog::setGeometry(const Geometry& geometry) {
    geometryView->setGeometry(geometry);
}

void DXFImportDialog::setStock(const StockMaterial& stock) {
    // TODO
}

void DXFImportDialog::onOkClicked() {
    emit importAccepted();
    accept();
}

void DXFImportDialog::onCancelClicked() {
    emit importCancelled();
    reject();
}

void DXFImportDialog::deactivateCenterLineSelection() {
    centerLineSelection->setChecked(false);
}

void DXFImportDialog::deactivateZeroPointSelection() {
    zeroPointSelection->setChecked(false);
    geometryView->disablePointPicking();
}