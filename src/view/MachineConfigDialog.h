//
// Created by Claude on $(date +%m/%d/%y).
//

#ifndef TURNLAB_MACHINECONFIGDIALOG_H
#define TURNLAB_MACHINECONFIGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QFileDialog>

#include "../model/MachineConfig.h"

class MachineConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit MachineConfigDialog(QWidget *parent = nullptr);
    ~MachineConfigDialog() override = default;

    void setMachineConfig(const MachineConfig& config);
    MachineConfig getMachineConfig() const;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onRestoreDefaultsClicked();
    void onBrowseScriptClicked();

private:
    void setupUI();
    void setupAxisDirectionGroup();
    void setupMachineLimitsGroup();
    void setupDisplaySettingsGroup();
    void setupPostProcessorGroup();
    void connectSignals();
    void updateUIFromConfig(const MachineConfig& config);
    MachineConfig getConfigFromUI() const;

    // UI Components
    QVBoxLayout* mainLayout;
    
    // Axis Direction Configuration
    QGroupBox* axisDirectionGroup;
    QFormLayout* axisDirectionLayout;
    
    QLabel* zAxisLabel;
    QButtonGroup* zAxisButtonGroup;
    QRadioButton* zPositiveTowardsTailstock;
    QRadioButton* zPositiveTowardsChuck;
    
    QLabel* xAxisLabel;
    QButtonGroup* xAxisButtonGroup;
    QRadioButton* xPositiveAwayFromCenter;
    QRadioButton* xPositiveTowardsCenter;
    
    // Machine Limits
    QGroupBox* machineLimitsGroup;
    QFormLayout* machineLimitsLayout;
    
    QDoubleSpinBox* maxSpindleSpeedSpinBox;
    QDoubleSpinBox* maxFeedRateMMMinSpinBox;
    QDoubleSpinBox* maxFeedRateMMRevSpinBox;
    QDoubleSpinBox* maxZTravelSpinBox;
    QDoubleSpinBox* maxXRadiusSpinBox;
    QDoubleSpinBox* minClearanceDistanceSpinBox;
    
    // Display Settings
    QGroupBox* displaySettingsGroup;
    QFormLayout* displaySettingsLayout;

    QSpinBox* displayPrecisionSpinBox;

    // PostProcessor Settings
    QGroupBox* postProcessorGroup;
    QFormLayout* postProcessorLayout;

    QLineEdit* postprocessorScriptPathLineEdit;
    QPushButton* browseScriptButton;
    QLineEdit* postprocessorClassNameLineEdit;

    // Dialog buttons
    QDialogButtonBox* buttonBox;
    QPushButton* restoreDefaultsButton;
    
    // Current configuration
    MachineConfig currentConfig;
};

#endif //TURNLAB_MACHINECONFIGDIALOG_H