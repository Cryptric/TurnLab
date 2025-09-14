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
    configGroup = new QGroupBox("Geometry Setup", this);
    configGroup->setFixedWidth(300); // Fixed width for configuration panel
    splitter->addWidget(configGroup);

    // Set splitter proportions (geometry view gets more space)
    splitter->setStretchFactor(0, 1); // GeometryView stretches
    splitter->setStretchFactor(1, 0); // ConfigPanel fixed

    mainLayout->addWidget(splitter);

    // Create custom button panel for the bottom
    auto* buttonLayout = new QHBoxLayout();

    backButton = new QPushButton("Back", this);
    backButton->setVisible(false); // Hidden initially
    nextButton = new QPushButton("Next", this);
    cancelButton = new QPushButton("Cancel", this);
    okButton = new QPushButton("Import", this);
    okButton->setVisible(false); // Hidden initially

    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(okButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void DXFImportDialog::setupConfigurationPanel() {
    // Create a main layout for the config group
    auto* mainLayout = new QVBoxLayout(configGroup);

    // Create geometry container widget
    geometryWidget = new QWidget(this);
    setupGeometryPanel();
    mainLayout->addWidget(geometryWidget);

    // Create stock container widget
    stockWidget = new QWidget(this);
    setupStockPanel();
    mainLayout->addWidget(stockWidget);

    // Initially show only geometry widget
    geometryWidget->show();
    stockWidget->hide();

    configGroup->setLayout(mainLayout);
}

void DXFImportDialog::setupGeometryPanel() {
    auto* formLayout = new QFormLayout(geometryWidget);

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

    geometryWidget->setLayout(formLayout);
}

void DXFImportDialog::setupStockPanel() {
    auto* formLayout = new QFormLayout(stockWidget);

    // Stock start position
    stockStartSpinBox = new QDoubleSpinBox(this);
    stockStartSpinBox->setRange(-1000.0, 1000.0);
    stockStartSpinBox->setDecimals(3);
    stockStartSpinBox->setSuffix(" mm");
    stockStartSpinBox->setValue(-10.0);
    connect(stockStartSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) { emit onStockStartOffsetChanged(value); });
    formLayout->addRow("Start Position:", stockStartSpinBox);

    // Stock end position
    stockEndSpinBox = new QDoubleSpinBox(this);
    stockEndSpinBox->setRange(-1000.0, 1000.0);
    stockEndSpinBox->setDecimals(3);
    stockEndSpinBox->setSuffix(" mm");
    stockEndSpinBox->setValue(100.0);
    connect(stockEndSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) { emit onStockEndOffsetChanged(value); });
    formLayout->addRow("End Position:", stockEndSpinBox);

    // Stock radius
    stockRadiusSpinBox = new QDoubleSpinBox(this);
    stockRadiusSpinBox->setRange(0.1, 500.0);
    stockRadiusSpinBox->setDecimals(3);
    stockRadiusSpinBox->setSuffix(" mm");
    stockRadiusSpinBox->setValue(25.0);
    connect(stockRadiusSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) { emit onStockRadiusChanged(value); });
    formLayout->addRow("Radius:", stockRadiusSpinBox);

    stockWidget->setLayout(formLayout);
}

void DXFImportDialog::switchToStage(ImportStage stage) {
    currentStage = stage;

    if (stage == GEOMETRY_SETUP) {
        configGroup->setTitle("Geometry Setup");
        geometryWidget->show();
        stockWidget->hide();
        backButton->setVisible(false);
        nextButton->setVisible(true);
        okButton->setVisible(false);
    } else if (stage == STOCK_SETUP) {
        configGroup->setTitle("Stock Material Setup");
        geometryWidget->hide();
        stockWidget->show();
        backButton->setVisible(true);
        nextButton->setVisible(false);
        okButton->setVisible(true);
    }
}

void DXFImportDialog::connectSignals() {
    connect(geometryView, &GeometryView::segmentSelected, this, &DXFImportDialog::segmentSelected);
    connect(geometryView, &GeometryView::pointSelected, this, &DXFImportDialog::pointSelected);

    connect(okButton, &QPushButton::clicked, this, &DXFImportDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &DXFImportDialog::onCancelClicked);
    connect(nextButton, &QPushButton::clicked, this, &DXFImportDialog::nextStageRequested);
    connect(backButton, &QPushButton::clicked, this, &DXFImportDialog::previousStageRequested);
}

void DXFImportDialog::setGeometry(const Geometry& geometry) {
    geometryView->setGeometry(geometry);
}

void DXFImportDialog::setStock(const StockMaterial& stock) {
    geometryView->plotStock(stock);
    if (stockStartSpinBox) stockStartSpinBox->setValue(stock.startPosition);
    if (stockEndSpinBox) stockEndSpinBox->setValue(stock.endPosition);
    if (stockRadiusSpinBox) stockRadiusSpinBox->setValue(stock.radius);
}

void DXFImportDialog::hideStock() {
    geometryView->hideStock();
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