//
// Created by Claude on $(date +%m/%d/%y).
//

#ifndef TURNLAB_MACHINECONFIGPRESENTER_H
#define TURNLAB_MACHINECONFIGPRESENTER_H

#include <QObject>
#include <memory>

#include "../view/MachineConfigDialog.h"
#include "../model/MachineConfig.h"

class MachineConfigPresenter : public QObject {
    Q_OBJECT

private:
    std::unique_ptr<MachineConfigDialog> dialog;
    MachineConfig currentConfig;
    
    void connectSignals();

public:
    explicit MachineConfigPresenter(QWidget* parent = nullptr);
    ~MachineConfigPresenter() = default;
    
    // Show the dialog and return the updated configuration
    MachineConfig showDialog(const MachineConfig& config);

private slots:
    void onConfigAccepted();
    void onConfigCancelled();

signals:
    void configurationUpdated(const MachineConfig& config);
    void configurationCancelled();
};

#endif //TURNLAB_MACHINECONFIGPRESENTER_H