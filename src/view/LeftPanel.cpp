#include "LeftPanel.h"
#include <QMenu>

LeftPanel::LeftPanel(QWidget *parent)
    : QWidget(parent), layout(nullptr), titleLabel(nullptr), treeView(nullptr), model(nullptr) {
    setupUI();
    setupTreeModel();
    populateTree();

    connect(treeView, &QTreeView::customContextMenuRequested, this, &LeftPanel::showContextMenu);
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
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    
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
        addOperationItem(op);
    }
}

void LeftPanel::addOperationItem(const OperationConfiguration& op) {
    auto* item = new QStandardItem(QString(toString(op.operationType).c_str()));
    item->setEditable(false);
    item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    operationsItem->appendRow(item);
}

QTreeView* LeftPanel::getTreeView() const {
    return treeView;
}

QStandardItemModel* LeftPanel::getModel() const {
    return model;
}

void LeftPanel::showContextMenu(const QPoint& pos) {
    QModelIndex index = treeView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    QStandardItem* item = model->itemFromIndex(index);
    if (!item) {
        return;
    }

    // Check if the item is a child of operationsItem
    if (item->parent() != operationsItem) {
        return;
    }

    // Get the row index of the operation
    int operationIndex = item->row();

    // Create context menu
    QMenu contextMenu(this);
    QAction* editAction = contextMenu.addAction(QString::fromUtf8("\U0001F589  Edit"));
    QAction* deleteAction = contextMenu.addAction(QString::fromUtf8("\U0001F5D1  Delete"));

    // Show menu and handle action
    QAction* selectedAction = contextMenu.exec(treeView->viewport()->mapToGlobal(pos));

    if (selectedAction == deleteAction) {
        emit operationDeleteRequested(operationIndex);
    } else if (selectedAction == editAction) {
        emit operationEditRequested(operationIndex);
    }
}