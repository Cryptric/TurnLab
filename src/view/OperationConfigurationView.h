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
#include "../model/operation/OperationConfiguration.h"

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
    QDoubleSpinBox* axialStartOffset;
    QDoubleSpinBox* axialEndOffset;

    QDoubleSpinBox* retractDistanceInput;
    QDoubleSpinBox* clearanceDistanceInput;
    QDoubleSpinBox* feedDistanceInput;
    QDoubleSpinBox* outerDistanceInput;
    QDoubleSpinBox* innerDistanceInput;

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
        void setOperationConfiguration(const OperationConfiguration& config);

    signals:
        // Tool tab signals
        void toolSelectionChanged(int toolNumber);
        void rpmChanged(int rpm);
        void feedrateChanged(int feedrate);

        // Geometry tab signals
        void geometrySelectionToggled(bool enabled);
        void axialStartOffsetChanged(double offset);
        void axialEndOffsetChanged(double offset);

        // Radii tab signals
        void retractDistanceChanged(double distance);
        void clearanceDistanceChanged(double distance);
        void feedDistanceChanged(double distance);
        void outerDistanceChanged(double distance);
        void innerDistanceChanged(double distance);

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