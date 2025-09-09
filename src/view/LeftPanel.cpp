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
    
    auto* operationsItem = new QStandardItem("Machining Operations");
    operationsItem->setEditable(false);
    operationsItem->setIcon(style()->standardIcon(QStyle::SP_DirIcon));
    rootItem->appendRow(operationsItem);
    
    QStringList operationTypes = {
        "Facing Operations",
        "Turning Operations", 
        "Contouring Operations",
        "Threading Operations",
        "Parting Operations",
        "Drilling Operations"
    };
    
    for (const QString& opType : operationTypes) {
        auto* categoryItem = new QStandardItem(opType);
        categoryItem->setEditable(false);
        categoryItem->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
        operationsItem->appendRow(categoryItem);
    }
    
    auto* toolsItem = new QStandardItem("Tool Configuration");
    toolsItem->setEditable(false);
    toolsItem->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    rootItem->appendRow(toolsItem);
    
    auto* profileItem = new QStandardItem("Profile Geometry");
    profileItem->setEditable(false);
    profileItem->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rootItem->appendRow(profileItem);
    
    treeView->expandAll();
}

QTreeView* LeftPanel::getTreeView() const {
    return treeView;
}

QStandardItemModel* LeftPanel::getModel() const {
    return model;
}