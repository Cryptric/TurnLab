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

    void setupUI();

    public:
        OperationConfigurationView(QWidget *parent = nullptr);
        ~OperationConfigurationView() override = default;
};


#endif //TURNLAB_OPERATIONCONFIGURATIONVIEW_H