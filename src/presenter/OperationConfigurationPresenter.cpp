//
// Created by gawain on 9/16/25.
//

#include "OperationConfigurationPresenter.h"
#include <spdlog/spdlog.h>

OperationConfigurationPresenter::OperationConfigurationPresenter(
    const OperationConfigVisibility& opVisibilityConfig,
    const MachineConfig& machineConfig,
    const ToolTable& toolTable,
    const Project& project,
    GeometryView& geometryView,
    OperationConfigurationView& operationConfigView,
    QObject* parent)
    : QObject(parent)
    , visibilityConfig(opVisibilityConfig)
    , machineConfig(machineConfig)
    , toolTable(toolTable)
    , project(project)
    , geometryView(geometryView)
    , configView(operationConfigView)
    , plotHelper(visibilityConfig, operationConfig, geometryView)
{
    spdlog::debug("Creating OperationConfigurationPresenter");
    populateToolSelector();
    connectSignals();
    applyConfiguration();
    operationConfigView.setOperationConfiguration(operationConfig);
}

void OperationConfigurationPresenter::connectSignals() {
    // Connect tab change signal
    connect(&configView, QOverload<int>::of(&OperationConfigurationView::currentChanged), this, [this](int index) { onTabChanged(static_cast<OperationConfigTab>(index)); });

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
    connect(&geometryView, &GeometryView::segmentSelected, this, &OperationConfigurationPresenter::onSegmentSelected);
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
    connect(&configView, &OperationConfigurationView::backoffDistanceChanged,
            this, &OperationConfigurationPresenter::onBackoffDistanceChanged);

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
    geometrySelectionEnabled = checked;

}

void OperationConfigurationPresenter::onSegmentSelected(size_t segmentIndex) {
    if (geometrySelectionEnabled) {
        spdlog::debug("Segment {} selected", segmentIndex);
        if (visibilityConfig.singleSegmentSelection) {
            operationConfig.geometrySelection = {segmentIndex};
        } else {
            operationConfig.geometrySelection.push_back(segmentIndex);
        }
        geometryView.setSelectedSegments(operationConfig.geometrySelection);
    }
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

void OperationConfigurationPresenter::onAxialStartOffsetChanged(double offset) {
    spdlog::debug("Axial start offset changed to: {}", offset);
    operationConfig.axialStartOffset = offset;
    plotHelper.update();
    emit configurationChanged();
}

void OperationConfigurationPresenter::onAxialEndOffsetChanged(double offset) {
    spdlog::debug("Axial end offset changed to: {}", offset);
    operationConfig.axialEndOffset = offset;
    plotHelper.update();
    emit configurationChanged();
}

void OperationConfigurationPresenter::onRetractDistanceChanged(double distance) {
    spdlog::debug("Retract distance changed to: {}", distance);
    operationConfig.retractDistance = distance;
    plotHelper.update();
    emit configurationChanged();
}

void OperationConfigurationPresenter::onClearanceDistanceChanged(double distance) {
    spdlog::debug("Clearance distance changed to: {}", distance);
    operationConfig.clearanceDistance = distance;
    plotHelper.update();
    emit configurationChanged();
}

void OperationConfigurationPresenter::onFeedDistanceChanged(double distance) {
    spdlog::debug("Feed distance changed to: {}", distance);
    operationConfig.feedDistance = distance;
    plotHelper.update();
    emit configurationChanged();
}

void OperationConfigurationPresenter::onOuterDistanceChanged(double distance) {
    spdlog::debug("Outer distance changed to: {}", distance);
    operationConfig.outerDistance = distance;
    plotHelper.update();
    emit configurationChanged();
}

void OperationConfigurationPresenter::onInnerDistanceChanged(double distance) {
    spdlog::debug("Inner distance changed to: {}", distance);
    operationConfig.innerDistance = distance;
    plotHelper.update();
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

void OperationConfigurationPresenter::onBackoffDistanceChanged(double distance) {
    spdlog::debug("Backoff distance changed to: {}", distance);
    operationConfig.backoffDistance = distance;
    emit configurationChanged();
}

void OperationConfigurationPresenter::onTabChanged(OperationConfigTab tab) {
    switch (tab) {
        case OperationConfigTab::Tool:
            spdlog::debug("Tab changed to Tool");
            plotHelper.hideDistanceMarkers();
            plotHelper.hideAxialOffsetMarkers();
            break;
        case OperationConfigTab::Geometry:
            spdlog::debug("Tab changed to Geometry");
            plotHelper.hideDistanceMarkers();
            plotHelper.update();
            plotHelper.showAxialOffsetMarkers();
            break;
        case OperationConfigTab::Radii:
            spdlog::debug("Tab changed to Radii");
            plotHelper.hideAxialOffsetMarkers();
            plotHelper.update();
            plotHelper.showDistanceMarkers();
            break;
        case OperationConfigTab::Passes:
            spdlog::debug("Tab changed to Passes");
            plotHelper.hideDistanceMarkers();
            plotHelper.hideAxialOffsetMarkers();
            break;
    }
}