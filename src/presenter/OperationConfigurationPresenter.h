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
#include "../model/Project.h"
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

    const OperationConfigVisibility visibilityConfig;
    const ToolTable& toolTable;
    GeometryView& geometryView;

    bool geometrySelectionEnabled = false;

    void connectSignals();
    void populateToolSelector();
    void applyConfiguration();

protected:
    const MachineConfig& machineConfig;
    OperationConfiguration operationConfig = OperationConfiguration();
    const Project& project;

    OperationConfigurationView& configView;
    OperationConfigurationPlotHelper plotHelper;

signals:
    void configurationChanged();


protected slots:
    void onTabChanged(OperationConfigTab tab);
    void onGeometrySelectionToggled(bool checked);

    virtual void onSegmentSelected(size_t index);
    void onToolSelectionChanged(int toolNumber);
    void onRpmChanged(int rpm);
    void onFeedrateChanged(int feedrate);
    void onAxialStartOffsetChanged(double offset);
    void onAxialEndOffsetChanged(double offset);
    void onRetractDistanceChanged(double distance);
    void onClearanceDistanceChanged(double distance);
    void onFeedDistanceChanged(double distance);
    void onOuterDistanceChanged(double distance);
    void onInnerDistanceChanged(double distance);
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
        const Project& project,
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