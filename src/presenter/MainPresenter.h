//
// Created by gawain on 9/11/25.
//

#ifndef TURNLAB_MAINPRESENTER_H
#define TURNLAB_MAINPRESENTER_H

#include <string>
#include <filesystem>

#include "../view/MainWindow.h"
#include "DXFImportPresenter.h"
#include "OperationConfigurationPresenter.h"
#include "../model/MachineConfig.h"


class MainPresenter : public QObject {
    Q_OBJECT

    MachineConfig machineConfig;
    ToolTable toolTable;

    MainWindow window;

    std::unique_ptr<Project> project;

    std::unique_ptr<OperationConfigurationView> currentOpConfigView;
    std::unique_ptr<OperationConfigurationPresenter> currentOpConfigPresenter;

    void connectSignals();

    void showMachineConfigDialog();
    void showDXFImportDialog(std::string inputDXF="");

private slots:
    void onFacingPressed();
    void onOperationConfigOkPressed();
    void onOperationConfigCancelPressed();

public:
    MainPresenter();
    explicit MainPresenter(const std::string &inputDXF);
    explicit MainPresenter(Project project);
    ~MainPresenter() = default;

    void setProject(Project p);

};


#endif //TURNLAB_MAINPRESENTER_H