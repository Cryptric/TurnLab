//
// Created by gawain on 9/16/25.
//

#include "OperationConfigurationPresenter.h"
#include <spdlog/spdlog.h>

OperationConfigurationPresenter::OperationConfigurationPresenter(
    const OperationConfigVisibility& visibilityConfig,
    const MachineConfig& machineConfig,
    const ToolTable& toolTable,
    GeometryView& geometryView,
    OperationConfigurationView& operationConfigView,
    QObject* parent)
    : QObject(parent)
    , visibilityConfig(visibilityConfig)
    , machineConfig(machineConfig)
    , toolTable(toolTable)
    , geometryView(geometryView)
    , configView(operationConfigView)
{
    spdlog::debug("Creating OperationConfigurationPresenter");
    populateToolSelector();
    connectSignals();
    applyConfiguration();
}

void OperationConfigurationPresenter::connectSignals() {
    // Connect tab change signal
    connect(&configView, QOverload<int>::of(&QTabWidget::currentChanged),
            this, [this](int index) { onTabChanged(static_cast<OperationConfigTab>(index)); });

    // Connect tool tab signals
    connect(&configView, &OperationConfigurationView::toolSelectionChanged,
            this, &OperationConfigurationPresenter::onToolSelectionChanged);
    connect(&configView, &OperationConfigurationView::rpmChanged,
            this, &OperationConfigurationPresenter::onRpmChanged);
    connect(&configView, &OperationConfigurationView::feedrateChanged,
            this, &OperationConfigurationPresenter::onFeedrateChanged);

    // Connect geometry tab signals
    connect(&configView, &OperationConfigurationView::geometrySelectionToggled,
            this, &OperationConfigurationPresenter::onGeometrySelectionToggled);
    connect(&configView, &OperationConfigurationView::axialStartOffsetChanged,
            this, &OperationConfigurationPresenter::onAxialStartOffsetChanged);
    connect(&configView, &OperationConfigurationView::axialEndOffsetChanged,
            this, &OperationConfigurationPresenter::onAxialEndOffsetChanged);

    // Connect radii tab signals
    connect(&configView, &OperationConfigurationView::retractDistanceChanged,
            this, &OperationConfigurationPresenter::onRetractDistanceChanged);
    connect(&configView, &OperationConfigurationView::clearanceDistanceChanged,
            this, &OperationConfigurationPresenter::onClearanceDistanceChanged);
    connect(&configView, &OperationConfigurationView::feedDistanceChanged,
            this, &OperationConfigurationPresenter::onFeedDistanceChanged);
    connect(&configView, &OperationConfigurationView::outerDistanceChanged,
            this, &OperationConfigurationPresenter::onOuterDistanceChanged);
    connect(&configView, &OperationConfigurationView::innerDistanceChanged,
            this, &OperationConfigurationPresenter::onInnerDistanceChanged);

    // Connect passes tab signals
    connect(&configView, &OperationConfigurationView::stepoverChanged,
            this, &OperationConfigurationPresenter::onStepoverChanged);
    connect(&configView, &OperationConfigurationView::cutDepthPerPassChanged,
            this, &OperationConfigurationPresenter::onCutDepthPerPassChanged);
    connect(&configView, &OperationConfigurationView::springPassesChanged,
            this, &OperationConfigurationPresenter::onSpringPassesChanged);
    connect(&configView, &OperationConfigurationView::peckDepthChanged,
            this, &OperationConfigurationPresenter::onPeckDepthChanged);
    connect(&configView, &OperationConfigurationView::dwellTimeChanged,
            this, &OperationConfigurationPresenter::onDwellTimeChanged);

    // Connect geometry view signals
    connect(&geometryView, &GeometryView::segmentSelected,
            this, &OperationConfigurationPresenter::configurationChanged);
}

void OperationConfigurationPresenter::populateToolSelector() {
    if (!visibilityConfig.showToolSelector) {
        return;
    }

    configView.setToolTable(toolTable);
    spdlog::debug("Populated tool selector with {} tools", toolTable.tools.size());
}

void OperationConfigurationPresenter::applyConfiguration() {
    spdlog::debug("Applying operation configuration");
    emit configurationChanged();
}

OperationConfigVisibility OperationConfigurationPresenter::getVisibilityConfig() const {
    return visibilityConfig;
}

const OperationConfiguration& OperationConfigurationPresenter::getOperationConfiguration() const {
    return operationConfig;
}

void OperationConfigurationPresenter::onGeometrySelectionToggled(bool checked) {
    spdlog::debug("Geometry selection toggled: {}", checked);
    operationConfig.geometrySelection = checked;

    if (checked) {
        geometryView.enablePointPicking();
        spdlog::info("Geometry selection enabled - click on geometry to select");
    } else {
        geometryView.disablePointPicking();
        spdlog::info("Geometry selection disabled");
    }

    emit configurationChanged();
}

void OperationConfigurationPresenter::onToolSelectionChanged(int toolNumber) {
    spdlog::debug("Tool selection changed to: {}", toolNumber);
    operationConfig.toolNumber = toolNumber;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onRpmChanged(int rpm) {
    spdlog::debug("RPM changed to: {}", rpm);
    operationConfig.rpm = rpm;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onFeedrateChanged(int feedrate) {
    spdlog::debug("Feedrate changed to: {}", feedrate);
    operationConfig.feedrate = feedrate;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onAxialStartOffsetChanged(int offset) {
    spdlog::debug("Axial start offset changed to: {}", offset);
    operationConfig.axialStartOffset = offset;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onAxialEndOffsetChanged(int offset) {
    spdlog::debug("Axial end offset changed to: {}", offset);
    operationConfig.axialEndOffset = offset;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onRetractDistanceChanged(int distance) {
    spdlog::debug("Retract distance changed to: {}", distance);
    operationConfig.retractDistance = distance;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onClearanceDistanceChanged(int distance) {
    spdlog::debug("Clearance distance changed to: {}", distance);
    operationConfig.clearanceDistance = distance;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onFeedDistanceChanged(int distance) {
    spdlog::debug("Feed distance changed to: {}", distance);
    operationConfig.feedDistance = distance;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onOuterDistanceChanged(int distance) {
    spdlog::debug("Outer distance changed to: {}", distance);
    operationConfig.outerDistance = distance;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onInnerDistanceChanged(int distance) {
    spdlog::debug("Inner distance changed to: {}", distance);
    operationConfig.innerDistance = distance;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onStepoverChanged(double stepover) {
    spdlog::debug("Stepover changed to: {}", stepover);
    operationConfig.stepover = stepover;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onCutDepthPerPassChanged(double depth) {
    spdlog::debug("Cut depth per pass changed to: {}", depth);
    operationConfig.cutDepthPerPass = depth;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onSpringPassesChanged(int passes) {
    spdlog::debug("Spring passes changed to: {}", passes);
    operationConfig.springPasses = passes;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onPeckDepthChanged(double depth) {
    spdlog::debug("Peck depth changed to: {}", depth);
    operationConfig.peckDepth = depth;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onDwellTimeChanged(int time) {
    spdlog::debug("Dwell time changed to: {}", time);
    operationConfig.dwellTime = time;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onTabChanged(OperationConfigTab tab) {
    switch (tab) {
        case OperationConfigTab::Tool:
            spdlog::debug("Tab changed to Tool");
            break;
        case OperationConfigTab::Geometry:
            spdlog::debug("Tab changed to Geometry");
            break;
        case OperationConfigTab::Radii:
            spdlog::debug("Tab changed to Radii");
            break;
        case OperationConfigTab::Passes:
            spdlog::debug("Tab changed to Passes");
            break;
    }
}