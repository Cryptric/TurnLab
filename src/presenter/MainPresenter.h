//
// Created by gawain on 9/11/25.
//

#ifndef TURNLAB_MAINPRESENTER_H
#define TURNLAB_MAINPRESENTER_H

#include <string>
#include <filesystem>

#include "../view/MainWindow.h"
#include "DXFImportPresenter.h"
#include "../model/MachineConfig.h"


class MainPresenter : public QObject {
    Q_OBJECT

    MachineConfig machineConfig;

    MainWindow window;

    void connectSignals();
    MachineConfig loadMachineConfig();
    void saveMachineConfig();
    std::filesystem::path getConfigPath();

    void showMachineConfigDialog();
    void showDXFImportDialog(const std::string& inputDXF);

public:
    MainPresenter();
    explicit MainPresenter(const std::string &inputDXF);
    ~MainPresenter() = default;

};


#endif //TURNLAB_MAINPRESENTER_H