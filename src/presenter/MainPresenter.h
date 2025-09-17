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

    std::unique_ptr<OperationConfigurationPresenter> operationConfigPresenter;

    void connectSignals();

    void showMachineConfigDialog();
    void showDXFImportDialog(const std::string& inputDXF);

    void setProject(Project p);

public:
    MainPresenter();
    explicit MainPresenter(const std::string &inputDXF);
    ~MainPresenter() = default;

};


#endif //TURNLAB_MAINPRESENTER_H