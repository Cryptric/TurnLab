//
// Created by gawain on 4/3/26.
//

#ifndef TURNLAB_TOOLTABLEPRESENTER_H
#define TURNLAB_TOOLTABLEPRESENTER_H
#include <QObject>

#include "../../../model/Tool.h"
#include "../../../view/tooltable/ToolTableDialog.h"


class ToolTablePresenter : public QObject {
    Q_OBJECT

    std::unique_ptr<ToolTableDialog> dialog;
    ToolTable currentToolTable;

    void connectSignals();

public:
    explicit ToolTablePresenter(QWidget *parent = nullptr);

    ToolTable showDialog(const ToolTable& toolTable);

private slots:
    void onToolTableAccepted();
    void onToolTableCancelled();

signals:
    void toolTableUpdated(const ToolTable& toolTable);
    void toolTableCancelled();
};


#endif //TURNLAB_TOOLTABLEPRESENTER_H