//
// Created by gawain on 9/16/25.
//

#include "OperationConfigurationView.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QFormLayout>

OperationConfigurationView::OperationConfigurationView(QWidget* parent) : QWidget(parent) {
    setupUI();
    connectSignals();
}

OperationConfigurationView::OperationConfigurationView(const OperationConfigVisibility& visibilityConfig, QWidget* parent)
    : QWidget(parent), config(visibilityConfig) {
    setupUI();
    connectSignals();
}

void OperationConfigurationView::setupUI() {
    // Create main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Create tab widget
    tabWidget = new QTabWidget();

    // Create tabs
    toolTab = new QWidget();
    geometryTab = new QWidget();
    radiiTab = new QWidget();
    passesTab = new QWidget();

    // Create vertical layouts for each tab
    QFormLayout* passesLayout = new QFormLayout(passesTab);
    passesLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    // Setup tool tab
    QFormLayout* toolFormLayout = new QFormLayout(toolTab);
    toolFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    toolSelector = new QComboBox();
    toolSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    rpmInput = new QSpinBox();
    rpmInput->setRange(100, 10000);
    rpmInput->setSuffix(" RPM");
    rpmInput->setValue(1000);
    rpmInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    feedrateInput = new QSpinBox();
    feedrateInput->setRange(1, 1000);
    feedrateInput->setSuffix(" mm/min");
    feedrateInput->setValue(100);
    feedrateInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (config.showToolSelector) toolFormLayout->addRow("Tool:", toolSelector);
    if (config.showRpmInput) toolFormLayout->addRow("RPM:", rpmInput);
    if (config.showFeedrateInput) toolFormLayout->addRow("Feedrate:", feedrateInput);

    // Setup geometry tab
    QFormLayout* geometryLayout = new QFormLayout(geometryTab);
    geometryLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    geometrySelectionButton = new QPushButton("Select Geometry");
    geometrySelectionButton->setCheckable(true);
    geometrySelectionButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if (config.showGeometrySelection) geometryLayout->addRow("Geometry Selection:", geometrySelectionButton);

    axialStartOffset = new QDoubleSpinBox();
    axialStartOffset->setRange(-1000.0, 1000.0);
    axialStartOffset->setSuffix(" mm");
    axialStartOffset->setDecimals(2);
    axialStartOffset->setValue(0.0);
    axialStartOffset->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if (config.showAxialStartOffset) geometryLayout->addRow("Axial Start Offset:", axialStartOffset);

    axialEndOffset = new QDoubleSpinBox();
    axialEndOffset->setRange(-1000.0, 1000.0);
    axialEndOffset->setSuffix(" mm");
    axialEndOffset->setDecimals(2);
    axialEndOffset->setValue(0.0);
    axialEndOffset->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if (config.showAxialEndOffset) geometryLayout->addRow("Axial End Offset:", axialEndOffset);

    // Setup radii tab
    QFormLayout* radiiFormLayout = new QFormLayout(radiiTab);
    radiiFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    retractDistanceInput = new QDoubleSpinBox();
    retractDistanceInput->setRange(0.0, 100.0);
    retractDistanceInput->setSuffix(" mm");
    retractDistanceInput->setDecimals(2);
    retractDistanceInput->setValue(5.0);
    retractDistanceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    clearanceDistanceInput = new QDoubleSpinBox();
    clearanceDistanceInput->setRange(0.0, 100.0);
    clearanceDistanceInput->setSuffix(" mm");
    clearanceDistanceInput->setDecimals(2);
    clearanceDistanceInput->setValue(2.0);
    clearanceDistanceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    feedDistanceInput = new QDoubleSpinBox();
    feedDistanceInput->setRange(0.0, 100.0);
    feedDistanceInput->setSuffix(" mm");
    feedDistanceInput->setDecimals(2);
    feedDistanceInput->setValue(1.0);
    feedDistanceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    outerDistanceInput = new QDoubleSpinBox();
    outerDistanceInput->setRange(0.0, 100.0);
    outerDistanceInput->setSuffix(" mm");
    outerDistanceInput->setDecimals(2);
    outerDistanceInput->setValue(10.0);
    outerDistanceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    innerDistanceInput = new QDoubleSpinBox();
    innerDistanceInput->setRange(0.0, 100.0);
    innerDistanceInput->setSuffix(" mm");
    innerDistanceInput->setDecimals(2);
    innerDistanceInput->setValue(5.0);
    innerDistanceInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (config.showRetractDistance) radiiFormLayout->addRow("Retract Distance:", retractDistanceInput);
    if (config.showClearanceDistance) radiiFormLayout->addRow("Clearance Distance:", clearanceDistanceInput);
    if (config.showFeedDistance) radiiFormLayout->addRow("Feed Distance:", feedDistanceInput);
    if (config.showOuterDistance) radiiFormLayout->addRow("Outer Distance:", outerDistanceInput);
    if (config.showInnerDistance) radiiFormLayout->addRow("Inner Distance:", innerDistanceInput);

    // Setup passes tab
    stepoverInput = new QDoubleSpinBox();
    stepoverInput->setRange(0.1, 100.0);
    stepoverInput->setSuffix(" mm");
    stepoverInput->setDecimals(2);
    stepoverInput->setValue(5.0);
    stepoverInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    cutDepthPerPassInput = new QDoubleSpinBox();
    cutDepthPerPassInput->setRange(0.1, 50.0);
    cutDepthPerPassInput->setSuffix(" mm");
    cutDepthPerPassInput->setDecimals(2);
    cutDepthPerPassInput->setValue(2.0);
    cutDepthPerPassInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    springPassesInput = new QSpinBox();
    springPassesInput->setRange(0, 10);
    springPassesInput->setValue(1);
    springPassesInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    peckDepthInput = new QDoubleSpinBox();
    peckDepthInput->setRange(0.1, 20.0);
    peckDepthInput->setSuffix(" mm");
    peckDepthInput->setDecimals(2);
    peckDepthInput->setValue(3.0);
    peckDepthInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    dwellTimeInput = new QSpinBox();
    dwellTimeInput->setRange(0, 10000);
    dwellTimeInput->setSuffix(" ms");
    dwellTimeInput->setValue(500);
    dwellTimeInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    if (config.showStepover) passesLayout->addRow("Stepover:", stepoverInput);
    if (config.showCutDepthPerPass) passesLayout->addRow("Cut Depth per Pass:", cutDepthPerPassInput);
    if (config.showSpringPasses) passesLayout->addRow("Spring Passes:", springPassesInput);
    if (config.showPeckDepth) passesLayout->addRow("Peck Depth:", peckDepthInput);
    if (config.showDwellTime) passesLayout->addRow("Dwell Time:", dwellTimeInput);

    // Add tabs to the tab widget based on visibility configuration
    if (config.showToolTab) tabWidget->addTab(toolTab, "Tool");
    if (config.showGeometryTab) tabWidget->addTab(geometryTab, "Geometry");
    if (config.showRadiiTab) tabWidget->addTab(radiiTab, "Radii");
    if (config.showPassesTab) tabWidget->addTab(passesTab, "Passes");

    // Create OK and Cancel buttons
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cancelButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Create button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    // Add tab widget and buttons to main layout
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);
}

void OperationConfigurationView::connectSignals() {
    // Tool tab connections
    connect(toolSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                int toolNumber = toolSelector->itemData(index).toInt();
                emit toolSelectionChanged(toolNumber);
            });

    connect(rpmInput, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &OperationConfigurationView::rpmChanged);

    connect(feedrateInput, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &OperationConfigurationView::feedrateChanged);

    // Geometry tab connections
    connect(geometrySelectionButton, &QPushButton::toggled,
            this, &OperationConfigurationView::geometrySelectionToggled);

    connect(axialStartOffset, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::axialStartOffsetChanged);

    connect(axialEndOffset, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::axialEndOffsetChanged);

    // Radii tab connections
    connect(retractDistanceInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::retractDistanceChanged);

    connect(clearanceDistanceInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::clearanceDistanceChanged);

    connect(feedDistanceInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::feedDistanceChanged);

    connect(outerDistanceInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::outerDistanceChanged);

    connect(innerDistanceInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::innerDistanceChanged);

    // Passes tab connections
    connect(stepoverInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::stepoverChanged);

    connect(cutDepthPerPassInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::cutDepthPerPassChanged);

    connect(springPassesInput, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &OperationConfigurationView::springPassesChanged);

    connect(peckDepthInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &OperationConfigurationView::peckDepthChanged);

    connect(dwellTimeInput, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &OperationConfigurationView::dwellTimeChanged);

    // Button connections
    connect(okButton, &QPushButton::clicked, this, &OperationConfigurationView::okPressed);
    connect(cancelButton, &QPushButton::clicked, this, &OperationConfigurationView::cancelPressed);

    // Tab widget connections
    connect(tabWidget, &QTabWidget::currentChanged, this, &OperationConfigurationView::currentChanged);
}

void OperationConfigurationView::setToolTable(const ToolTable& toolTable) {
    toolSelector->clear();

    for (const auto& tool : toolTable.tools) {
        toolSelector->addItem(
            QString("T%1 - %2 (%3)")
                .arg(tool.number)
                .arg(QString::fromStdString(tool.description))
                .arg(QString::fromStdString(tool.isoCode)),
            tool.number
        );
    }
}

void OperationConfigurationView::setOperationConfiguration(const OperationConfiguration& config) {
    // Set tool configuration values
    int i = 0;
    for (; i < toolSelector->count(); ++i) {
        if (toolSelector->itemData(i).toInt() == config.toolNumber) {
            toolSelector->setCurrentIndex(i);
            break;
        }
    }
    if (i == toolSelector->count()) {
        toolSelector->setCurrentIndex(0);
        const int toolNumber = toolSelector->itemData(0).toInt();
        emit toolSelectionChanged(toolNumber);
    }
    rpmInput->setValue(config.rpm);
    feedrateInput->setValue(config.feedrate);

    // Set geometry configuration values
    axialStartOffset->setValue(config.axialStartOffset);
    axialEndOffset->setValue(config.axialEndOffset);

    // Set radii configuration values
    retractDistanceInput->setValue(config.retractDistance);
    clearanceDistanceInput->setValue(config.clearanceDistance);
    feedDistanceInput->setValue(config.feedDistance);
    outerDistanceInput->setValue(config.outerDistance);
    innerDistanceInput->setValue(config.innerDistance);

    // Set passes configuration values
    stepoverInput->setValue(config.stepover);
    cutDepthPerPassInput->setValue(config.cutDepthPerPass);
    springPassesInput->setValue(config.springPasses);
    peckDepthInput->setValue(config.peckDepth);
    dwellTimeInput->setValue(config.dwellTime);
}