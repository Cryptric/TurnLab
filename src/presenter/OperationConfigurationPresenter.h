//
// Created by gawain on 9/16/25.
//

#ifndef TURNLAB_OPERATIONCONFIGURATIONPRESENTER_H
#define TURNLAB_OPERATIONCONFIGURATIONPRESENTER_H

#include <QObject>

#include "../view/OperationConfigurationView.h"
#include "../view/GeometryView.h"
#include "../view/OperationConfigurationPlotHelper.h"
#include "../model/MachineConfig.h"
#include "../model/Tool.h"
#include "../model/operation/OperationConfiguration.h"

enum class OperationConfigTab {
    Tool = 0,
    Geometry = 1,
    Radii = 2,
    Passes = 3
};

class OperationConfigurationPresenter : public QObject {
    Q_OBJECT
private:
    OperationConfiguration operationConfig = OperationConfiguration();

    const OperationConfigVisibility visibilityConfig;
    const MachineConfig& machineConfig;
    const ToolTable& toolTable;
    GeometryView& geometryView;
    OperationConfigurationView& configView;
    OperationConfigurationPlotHelper plotHelper;

    bool geometrySelectionEnabled = false;

    void connectSignals();
    void populateToolSelector();
    void applyConfiguration();

signals:
    void configurationChanged();


private slots:
    void onTabChanged(OperationConfigTab tab);
    void onGeometrySelectionToggled(bool checked);
    void onSegmentSelected(size_t index);
    void onToolSelectionChanged(int toolNumber);
    void onRpmChanged(int rpm);
    void onFeedrateChanged(int feedrate);
    void onAxialStartOffsetChanged(int offset);
    void onAxialEndOffsetChanged(int offset);
    void onRetractDistanceChanged(int distance);
    void onClearanceDistanceChanged(int distance);
    void onFeedDistanceChanged(int distance);
    void onOuterDistanceChanged(int distance);
    void onInnerDistanceChanged(int distance);
    void onStepoverChanged(double stepover);
    void onCutDepthPerPassChanged(double depth);
    void onSpringPassesChanged(int passes);
    void onPeckDepthChanged(double depth);
    void onDwellTimeChanged(int time);

public:
    explicit OperationConfigurationPresenter(
        const OperationConfigVisibility& opVisibilityConfig,
        const MachineConfig& machineConfig,
        const ToolTable& toolTable,
        GeometryView& geometryView,
        OperationConfigurationView& operationConfigView,
        QObject* parent = nullptr
    );

    ~OperationConfigurationPresenter() = default;

    // Get current configuration values
    OperationConfigVisibility getVisibilityConfig() const;
    const OperationConfiguration& getOperationConfiguration() const;

};

#endif //TURNLAB_OPERATIONCONFIGURATIONPRESENTER_H