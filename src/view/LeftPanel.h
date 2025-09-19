#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLabel>

#include "../model/Project.h"

class LeftPanel : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout* layout;
    QLabel* titleLabel;
    QTreeView* treeView;
    QStandardItemModel* model;

    QStandardItem* operationsItem;
    QStandardItem* toolsItem;
    QStandardItem* geometryItem;

    Project project;

public:
    explicit LeftPanel(QWidget *parent = nullptr);
    ~LeftPanel() = default;

    QTreeView* getTreeView() const;
    QStandardItemModel* getModel() const;

    void setProject(const Project& p);

private:
    void setupUI();
    void setupTreeModel();
    void populateTree();
};

#endif // LEFTPANEL_H