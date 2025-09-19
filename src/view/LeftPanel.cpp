#include "LeftPanel.h"

LeftPanel::LeftPanel(QWidget *parent)
    : QWidget(parent), layout(nullptr), titleLabel(nullptr), treeView(nullptr), model(nullptr) {
    setupUI();
    setupTreeModel();
    populateTree();
}

void LeftPanel::setupUI() {
    setFixedWidth(250);
    
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    
    titleLabel = new QLabel("Operations");

    
    treeView = new QTreeView(this);
    treeView->setHeaderHidden(true);
    treeView->setRootIsDecorated(true);
    treeView->setExpandsOnDoubleClick(true);

    
    layout->addWidget(titleLabel);
    layout->addWidget(treeView);
}

void LeftPanel::setupTreeModel() {
    model = new QStandardItemModel(this);
    treeView->setModel(model);
}

void LeftPanel::populateTree() {
    auto* rootItem = model->invisibleRootItem();
    
    operationsItem = new QStandardItem("Machining Operations");
    operationsItem->setEditable(false);
    operationsItem->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    rootItem->appendRow(operationsItem);
    

    toolsItem = new QStandardItem("Tool Configuration");
    toolsItem->setEditable(false);
    toolsItem->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    rootItem->appendRow(toolsItem);

    geometryItem = new QStandardItem("Profile Geometry");
    geometryItem->setEditable(false);
    geometryItem->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rootItem->appendRow(geometryItem);

    treeView->expandAll();
}

void LeftPanel::setProject(const Project& p) {
    project = p;
    operationsItem->removeRows(0, operationsItem->rowCount());

    for (const auto& op : project.operations) {
        auto* item = new QStandardItem(QString(toString(op.operationType).c_str()));
        item->setEditable(false);
        item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
        operationsItem->appendRow(item);

    }
}

QTreeView* LeftPanel::getTreeView() const {
    return treeView;
}

QStandardItemModel* LeftPanel::getModel() const {
    return model;
}