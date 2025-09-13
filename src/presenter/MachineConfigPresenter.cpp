//
// Created by Claude on $(date +%m/%d/%y).
//

#include "MachineConfigPresenter.h"

MachineConfigPresenter::MachineConfigPresenter(QWidget* parent) : QObject(parent) {
    dialog = std::make_unique<MachineConfigDialog>(parent);
    connectSignals();
}

void MachineConfigPresenter::connectSignals() {
    connect(dialog.get(), &QDialog::accepted, this, &MachineConfigPresenter::onConfigAccepted);
    connect(dialog.get(), &QDialog::rejected, this, &MachineConfigPresenter::onConfigCancelled);
}

MachineConfig MachineConfigPresenter::showDialog(const MachineConfig& config) {
    // Set the initial configuration in the dialog
    currentConfig = config;
    dialog->setMachineConfig(config);
    
    // Show the dialog modally
    dialog->exec();
    
    // Return the final configuration
    return currentConfig;
}

void MachineConfigPresenter::onConfigAccepted() {
    // Get final configuration from dialog
    currentConfig = dialog->getMachineConfig();
    
    // Emit signal with updated configuration
    emit configurationUpdated(currentConfig);
}

void MachineConfigPresenter::onConfigCancelled() {
    // Configuration was cancelled, no changes made
    emit configurationCancelled();
}