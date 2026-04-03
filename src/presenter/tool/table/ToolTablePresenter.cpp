//
// Created by gawain on 4/3/26.
//

#include "ToolTablePresenter.h"


ToolTablePresenter::ToolTablePresenter(QWidget *parent) : QObject(parent) {
    dialog = std::make_unique<ToolTableDialog>(parent);
    connectSignals();
}


void ToolTablePresenter::connectSignals() {
    connect(dialog.get(), &QDialog::accepted, this, &ToolTablePresenter::onToolTableAccepted);
    connect(dialog.get(), &QDialog::rejected, this, &ToolTablePresenter::onToolTableCancelled);
}

ToolTable ToolTablePresenter::showDialog(const ToolTable &toolTable) {
    currentToolTable = toolTable;
    dialog->populateToolTable(toolTable);

    dialog->exec();

    return currentToolTable;
}

void ToolTablePresenter::onToolTableAccepted() {
    currentToolTable = dialog->getToolTable();
    emit toolTableUpdated(currentToolTable);
}

void ToolTablePresenter::onToolTableCancelled() {
    emit toolTableCancelled();
}