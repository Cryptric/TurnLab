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

#include "ToolTableDialog.h"

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
};


#endif //TURNLAB_OPERATIONCONFIGURATIONVIEW_H