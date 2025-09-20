//
// Created by Claude on $(date +%m/%d/%y).
//

#include "MachineConfigDialog.h"

MachineConfigDialog::MachineConfigDialog(QWidget *parent)
    : QDialog(parent), currentConfig()
{
    setWindowTitle("Machine Configuration");
    setModal(true);
    resize(500, 600);
    
    setupUI();
    connectSignals();
    updateUIFromConfig(currentConfig); // Load default configuration
}

void MachineConfigDialog::setupUI() {
    mainLayout = new QVBoxLayout(this);

    setupAxisDirectionGroup();
    setupMachineLimitsGroup();
    setupDisplaySettingsGroup();
    setupPostProcessorGroup();

    // Restore Defaults button
    restoreDefaultsButton = new QPushButton("Restore Defaults", this);

    // Dialog buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    // Add to main layout
    mainLayout->addWidget(axisDirectionGroup);
    mainLayout->addWidget(machineLimitsGroup);
    mainLayout->addWidget(displaySettingsGroup);
    mainLayout->addWidget(postProcessorGroup);
    mainLayout->addWidget(restoreDefaultsButton);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void MachineConfigDialog::setupAxisDirectionGroup() {
    axisDirectionGroup = new QGroupBox("Axis Direction Configuration", this);
    axisDirectionLayout = new QFormLayout(axisDirectionGroup);
    
    // Z-Axis Direction
    zAxisLabel = new QLabel("Z-Axis Direction:", this);
    
    zAxisButtonGroup = new QButtonGroup(this);
    zPositiveTowardsTailstock = new QRadioButton("Moving towards tailstock is positive (+Z)", this);
    zPositiveTowardsChuck = new QRadioButton("Moving towards chuck is positive (+Z)", this);
    
    zAxisButtonGroup->addButton(zPositiveTowardsTailstock, static_cast<int>(AxisDirection::Positive));
    zAxisButtonGroup->addButton(zPositiveTowardsChuck, static_cast<int>(AxisDirection::Negative));
    
    QVBoxLayout* zAxisLayout = new QVBoxLayout();
    zAxisLayout->addWidget(zPositiveTowardsTailstock);
    zAxisLayout->addWidget(zPositiveTowardsChuck);
    
    // X-Axis Direction
    xAxisLabel = new QLabel("X-Axis Direction:", this);
    
    xAxisButtonGroup = new QButtonGroup(this);
    xPositiveAwayFromCenter = new QRadioButton("Moving away from centerline is positive (+X)", this);
    xPositiveTowardsCenter = new QRadioButton("Moving towards centerline is positive (+X)", this);
    
    xAxisButtonGroup->addButton(xPositiveAwayFromCenter, static_cast<int>(AxisDirection::Positive));
    xAxisButtonGroup->addButton(xPositiveTowardsCenter, static_cast<int>(AxisDirection::Negative));
    
    QVBoxLayout* xAxisLayout = new QVBoxLayout();
    xAxisLayout->addWidget(xPositiveAwayFromCenter);
    xAxisLayout->addWidget(xPositiveTowardsCenter);
    
    axisDirectionLayout->addRow(zAxisLabel, zAxisLayout);
    axisDirectionLayout->addRow(xAxisLabel, xAxisLayout);
}

void MachineConfigDialog::setupMachineLimitsGroup() {
    machineLimitsGroup = new QGroupBox("Machine Limits", this);
    machineLimitsLayout = new QFormLayout(machineLimitsGroup);
    
    // Maximum Spindle Speed
    maxSpindleSpeedSpinBox = new QDoubleSpinBox(this);
    maxSpindleSpeedSpinBox->setRange(100.0, 10000.0);
    maxSpindleSpeedSpinBox->setSuffix(" RPM");
    maxSpindleSpeedSpinBox->setDecimals(0);
    
    // Maximum Feed Rate (mm/min)
    maxFeedRateMMMinSpinBox = new QDoubleSpinBox(this);
    maxFeedRateMMMinSpinBox->setRange(10.0, 10000.0);
    maxFeedRateMMMinSpinBox->setSuffix(" mm/min");
    maxFeedRateMMMinSpinBox->setDecimals(0);
    
    // Maximum Feed Rate (mm/rev)
    maxFeedRateMMRevSpinBox = new QDoubleSpinBox(this);
    maxFeedRateMMRevSpinBox->setRange(0.01, 10.0);
    maxFeedRateMMRevSpinBox->setSuffix(" mm/rev");
    maxFeedRateMMRevSpinBox->setDecimals(3);
    
    // Maximum Z Travel
    maxZTravelSpinBox = new QDoubleSpinBox(this);
    maxZTravelSpinBox->setRange(50.0, 2000.0);
    maxZTravelSpinBox->setSuffix(" mm");
    maxZTravelSpinBox->setDecimals(1);
    
    // Maximum X Radius
    maxXRadiusSpinBox = new QDoubleSpinBox(this);
    maxXRadiusSpinBox->setRange(10.0, 500.0);
    maxXRadiusSpinBox->setSuffix(" mm");
    maxXRadiusSpinBox->setDecimals(1);
    
    // Minimum Clearance Distance
    minClearanceDistanceSpinBox = new QDoubleSpinBox(this);
    minClearanceDistanceSpinBox->setRange(0.1, 50.0);
    minClearanceDistanceSpinBox->setSuffix(" mm");
    minClearanceDistanceSpinBox->setDecimals(1);
    
    machineLimitsLayout->addRow("Maximum Spindle Speed:", maxSpindleSpeedSpinBox);
    machineLimitsLayout->addRow("Maximum Feed Rate (mm/min):", maxFeedRateMMMinSpinBox);
    machineLimitsLayout->addRow("Maximum Feed Rate (mm/rev):", maxFeedRateMMRevSpinBox);
    machineLimitsLayout->addRow("Maximum Z Travel Distance:", maxZTravelSpinBox);
    machineLimitsLayout->addRow("Maximum X Radius:", maxXRadiusSpinBox);
    machineLimitsLayout->addRow("Minimum Clearance Distance:", minClearanceDistanceSpinBox);
}

void MachineConfigDialog::setupDisplaySettingsGroup() {
    displaySettingsGroup = new QGroupBox("Display Settings", this);
    displaySettingsLayout = new QFormLayout(displaySettingsGroup);

    // Display Precision
    displayPrecisionSpinBox = new QSpinBox(this);
    displayPrecisionSpinBox->setRange(0, 6);
    displayPrecisionSpinBox->setSuffix(" decimal places");

    displaySettingsLayout->addRow("Coordinate Display Precision:", displayPrecisionSpinBox);
}

void MachineConfigDialog::setupPostProcessorGroup() {
    postProcessorGroup = new QGroupBox("PostProcessor Settings", this);
    postProcessorLayout = new QFormLayout(postProcessorGroup);

    // Script Path with Browse button
    QHBoxLayout* scriptPathLayout = new QHBoxLayout();
    postprocessorScriptPathLineEdit = new QLineEdit(this);
    browseScriptButton = new QPushButton("Browse...", this);
    browseScriptButton->setMaximumWidth(80);

    scriptPathLayout->addWidget(postprocessorScriptPathLineEdit);
    scriptPathLayout->addWidget(browseScriptButton);

    // Class Name
    postprocessorClassNameLineEdit = new QLineEdit(this);

    postProcessorLayout->addRow("Script Path:", scriptPathLayout);
    postProcessorLayout->addRow("Class Name:", postprocessorClassNameLineEdit);
}

void MachineConfigDialog::connectSignals() {
    connect(buttonBox, &QDialogButtonBox::accepted, this, &MachineConfigDialog::onOkClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &MachineConfigDialog::onCancelClicked);
    connect(restoreDefaultsButton, &QPushButton::clicked, this, &MachineConfigDialog::onRestoreDefaultsClicked);
    connect(browseScriptButton, &QPushButton::clicked, this, &MachineConfigDialog::onBrowseScriptClicked);
}

void MachineConfigDialog::setMachineConfig(const MachineConfig& config) {
    currentConfig = config;
    updateUIFromConfig(config);
}

MachineConfig MachineConfigDialog::getMachineConfig() const {
    return getConfigFromUI();
}

void MachineConfigDialog::updateUIFromConfig(const MachineConfig& config) {
    // Axis directions
    if (config.zAxisDirection == AxisDirection::Positive) {
        zPositiveTowardsTailstock->setChecked(true);
    } else {
        zPositiveTowardsChuck->setChecked(true);
    }
    
    if (config.xAxisDirection == AxisDirection::Positive) {
        xPositiveAwayFromCenter->setChecked(true);
    } else {
        xPositiveTowardsCenter->setChecked(true);
    }
    
    // Machine limits
    maxSpindleSpeedSpinBox->setValue(config.maxSpindleSpeed);
    maxFeedRateMMMinSpinBox->setValue(config.maxFeedRateMMMin);
    maxFeedRateMMRevSpinBox->setValue(config.maxFeedRateMMRev);
    maxZTravelSpinBox->setValue(config.maxZTravel);
    maxXRadiusSpinBox->setValue(config.maxXRadius);
    minClearanceDistanceSpinBox->setValue(config.minClearanceDistance);
    
    // Display settings
    displayPrecisionSpinBox->setValue(config.displayPrecision);

    // PostProcessor settings
    postprocessorScriptPathLineEdit->setText(QString::fromStdString(config.postprocessorScriptPath));
    postprocessorClassNameLineEdit->setText(QString::fromStdString(config.postprocessorClassName));
}

MachineConfig MachineConfigDialog::getConfigFromUI() const {
    MachineConfig config;
    
    // Axis directions
    config.zAxisDirection = zPositiveTowardsTailstock->isChecked() ? 
        AxisDirection::Positive : AxisDirection::Negative;
    config.xAxisDirection = xPositiveAwayFromCenter->isChecked() ? 
        AxisDirection::Positive : AxisDirection::Negative;
    
    // Machine limits
    config.maxSpindleSpeed = maxSpindleSpeedSpinBox->value();
    config.maxFeedRateMMMin = maxFeedRateMMMinSpinBox->value();
    config.maxFeedRateMMRev = maxFeedRateMMRevSpinBox->value();
    config.maxZTravel = maxZTravelSpinBox->value();
    config.maxXRadius = maxXRadiusSpinBox->value();
    config.minClearanceDistance = minClearanceDistanceSpinBox->value();
    
    // Display settings
    config.displayPrecision = displayPrecisionSpinBox->value();

    // PostProcessor settings
    config.postprocessorScriptPath = postprocessorScriptPathLineEdit->text().toStdString();
    config.postprocessorClassName = postprocessorClassNameLineEdit->text().toStdString();

    return config;
}

void MachineConfigDialog::onOkClicked() {
    currentConfig = getConfigFromUI();
    accept();
}

void MachineConfigDialog::onCancelClicked() {
    reject();
}

void MachineConfigDialog::onRestoreDefaultsClicked() {
    MachineConfig defaultConfig; // Create with default values
    updateUIFromConfig(defaultConfig);
}

void MachineConfigDialog::onBrowseScriptClicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
        "Select PostProcessor Script",
        postprocessorScriptPathLineEdit->text(),
        "Python Files (*.py)");

    if (!fileName.isEmpty()) {
        postprocessorScriptPathLineEdit->setText(fileName);
    }
}