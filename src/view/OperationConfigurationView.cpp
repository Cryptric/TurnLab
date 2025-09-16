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

OperationConfigurationView::OperationConfigurationView(QWidget* parent) : QTabWidget(parent) {
    setupUI();
}

OperationConfigurationView::OperationConfigurationView(const OperationConfigVisibility& visibilityConfig, QWidget* parent)
    : QTabWidget(parent), config(visibilityConfig) {
    setupUI();
}

void OperationConfigurationView::setupUI() {
    // Create tabs
    toolTab = new QWidget();
    geometryTab = new QWidget();
    radiiTab = new QWidget();
    passesTab = new QWidget();

    // Create vertical layouts for each tab
    QFormLayout* passesLayout = new QFormLayout(passesTab);

    // Setup tool tab
    QFormLayout* toolFormLayout = new QFormLayout(toolTab);

    toolSelector = new QComboBox();
    rpmInput = new QSpinBox();
    rpmInput->setRange(100, 10000);
    rpmInput->setSuffix(" RPM");
    rpmInput->setValue(1000);

    feedrateInput = new QSpinBox();
    feedrateInput->setRange(1, 1000);
    feedrateInput->setSuffix(" mm/min");
    feedrateInput->setValue(100);

    if (config.showToolSelector) toolFormLayout->addRow("Tool:", toolSelector);
    if (config.showRpmInput) toolFormLayout->addRow("RPM:", rpmInput);
    if (config.showFeedrateInput) toolFormLayout->addRow("Feedrate:", feedrateInput);

    // Setup geometry tab
    QFormLayout* geometryLayout = new QFormLayout(geometryTab);

    geometrySelectionButton = new QPushButton("Select Geometry");
    geometrySelectionButton->setCheckable(true);
    if (config.showGeometrySelection) geometryLayout->addRow("Geometry Selection:", geometrySelectionButton);

    axialStartOffset = new QSpinBox();
    axialStartOffset->setRange(-1000, 1000);
    axialStartOffset->setSuffix(" mm");
    axialStartOffset->setValue(0);
    if (config.showAxialStartOffset) geometryLayout->addRow("Axial Start Offset:", axialStartOffset);

    axialEndOffset = new QSpinBox();
    axialEndOffset->setRange(-1000, 1000);
    axialEndOffset->setSuffix(" mm");
    axialEndOffset->setValue(0);
    if (config.showAxialEndOffset) geometryLayout->addRow("Axial End Offset:", axialEndOffset);

    // Setup radii tab
    QFormLayout* radiiFormLayout = new QFormLayout(radiiTab);

    retractDistanceInput = new QSpinBox();
    retractDistanceInput->setRange(0, 100);
    retractDistanceInput->setSuffix(" mm");
    retractDistanceInput->setValue(5);

    clearanceDistanceInput = new QSpinBox();
    clearanceDistanceInput->setRange(0, 100);
    clearanceDistanceInput->setSuffix(" mm");
    clearanceDistanceInput->setValue(2);

    feedDistanceInput = new QSpinBox();
    feedDistanceInput->setRange(0, 100);
    feedDistanceInput->setSuffix(" mm");
    feedDistanceInput->setValue(1);

    outerDistanceInput = new QSpinBox();
    outerDistanceInput->setRange(0, 100);
    outerDistanceInput->setSuffix(" mm");
    outerDistanceInput->setValue(10);

    innerDistanceInput = new QSpinBox();
    innerDistanceInput->setRange(0, 100);
    innerDistanceInput->setSuffix(" mm");
    innerDistanceInput->setValue(5);

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

    cutDepthPerPassInput = new QDoubleSpinBox();
    cutDepthPerPassInput->setRange(0.1, 50.0);
    cutDepthPerPassInput->setSuffix(" mm");
    cutDepthPerPassInput->setDecimals(2);
    cutDepthPerPassInput->setValue(2.0);

    springPassesInput = new QSpinBox();
    springPassesInput->setRange(0, 10);
    springPassesInput->setValue(1);

    peckDepthInput = new QDoubleSpinBox();
    peckDepthInput->setRange(0.1, 20.0);
    peckDepthInput->setSuffix(" mm");
    peckDepthInput->setDecimals(2);
    peckDepthInput->setValue(3.0);

    dwellTimeInput = new QSpinBox();
    dwellTimeInput->setRange(0, 10000);
    dwellTimeInput->setSuffix(" ms");
    dwellTimeInput->setValue(500);

    if (config.showStepover) passesLayout->addRow("Stepover:", stepoverInput);
    if (config.showCutDepthPerPass) passesLayout->addRow("Cut Depth per Pass:", cutDepthPerPassInput);
    if (config.showSpringPasses) passesLayout->addRow("Spring Passes:", springPassesInput);
    if (config.showPeckDepth) passesLayout->addRow("Peck Depth:", peckDepthInput);
    if (config.showDwellTime) passesLayout->addRow("Dwell Time:", dwellTimeInput);

    // Add tabs to the tab widget based on visibility configuration
    if (config.showToolTab) addTab(toolTab, "Tool");
    if (config.showGeometryTab) addTab(geometryTab, "Geometry");
    if (config.showRadiiTab) addTab(radiiTab, "Radii");
    if (config.showPassesTab) addTab(passesTab, "Passes");
}