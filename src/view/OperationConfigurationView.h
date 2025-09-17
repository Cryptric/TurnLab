//
// Created by gawain on 9/16/25.
//

#ifndef TURNLAB_OPERATIONCONFIGURATIONVIEW_H
#define TURNLAB_OPERATIONCONFIGURATIONVIEW_H

#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include <QPushButton>

#include "../model/Tool.h"

struct OperationConfigVisibility {
    // Tool tab
    bool showToolSelector = true;
    bool showRpmInput = true;
    bool showFeedrateInput = true;

    // Geometry tab
    bool showGeometrySelection = true;
    bool showAxialStartOffset = true;
    bool showAxialEndOffset = true;

    // Radii tab
    bool showRetractDistance = true;
    bool showClearanceDistance = true;
    bool showFeedDistance = true;
    bool showOuterDistance = true;
    bool showInnerDistance = true;

    // Passes tab
    bool showStepover = true;
    bool showCutDepthPerPass = true;
    bool showSpringPasses = true;
    bool showPeckDepth = true;
    bool showDwellTime = true;

    // Tab visibility
    bool showToolTab = true;
    bool showGeometryTab = true;
    bool showRadiiTab = true;
    bool showPassesTab = true;
};


class OperationConfigurationView : public QTabWidget {
    Q_OBJECT

    QWidget* toolTab;
    QWidget* geometryTab;
    QWidget* radiiTab;
    QWidget* passesTab;

    QComboBox* toolSelector;
    QSpinBox* rpmInput;
    QSpinBox* feedrateInput;

    QPushButton* geometrySelectionButton;
    QSpinBox* axialStartOffset;
    QSpinBox* axialEndOffset;

    QSpinBox* retractDistanceInput;
    QSpinBox* clearanceDistanceInput;
    QSpinBox* feedDistanceInput;
    QSpinBox* outerDistanceInput;
    QSpinBox* innerDistanceInput;

    QDoubleSpinBox* stepoverInput;
    QDoubleSpinBox* cutDepthPerPassInput;
    QSpinBox* springPassesInput;
    QDoubleSpinBox* peckDepthInput;
    QSpinBox* dwellTimeInput;

    OperationConfigVisibility config;

    void setupUI();

    public:
        explicit OperationConfigurationView(QWidget *parent = nullptr);
        explicit OperationConfigurationView(const OperationConfigVisibility& visibilityConfig, QWidget *parent = nullptr);
        ~OperationConfigurationView() override = default;

        void setToolTable(const ToolTable& toolTable);

    signals:
        // Tool tab signals
        void toolSelectionChanged(int toolNumber);
        void rpmChanged(int rpm);
        void feedrateChanged(int feedrate);

        // Geometry tab signals
        void geometrySelectionToggled(bool enabled);
        void axialStartOffsetChanged(int offset);
        void axialEndOffsetChanged(int offset);

        // Radii tab signals
        void retractDistanceChanged(int distance);
        void clearanceDistanceChanged(int distance);
        void feedDistanceChanged(int distance);
        void outerDistanceChanged(int distance);
        void innerDistanceChanged(int distance);

        // Passes tab signals
        void stepoverChanged(double stepover);
        void cutDepthPerPassChanged(double depth);
        void springPassesChanged(int passes);
        void peckDepthChanged(double depth);
        void dwellTimeChanged(int time);

    private:
        void connectSignals();
};


#endif //TURNLAB_OPERATIONCONFIGURATIONVIEW_H