#include "ToolTableDialog.h"

ToolTableDialog::ToolTableDialog(QWidget *parent)
    : QDialog(parent), toolTable(nullptr), addButton(nullptr), okButton(nullptr), cancelButton(nullptr) {
    setWindowTitle("Tool Table");
    setModal(true);
    resize(500, 600);
    
    setupUI();
    setupTable();
    populateTable();
    connectSignals();
}

void ToolTableDialog::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    
    auto* titleLabel = new QLabel("Tool Table Configuration");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    toolTable = new QTableWidget(this);
    mainLayout->addWidget(toolTable);
    
    auto* buttonLayout = new QHBoxLayout();
    
    addButton = new QPushButton("Add Tool");
    addButton->setIcon(style()->standardIcon(QStyle::SP_DialogOkButton));
    
    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();
    
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
    
    okButton->setDefault(true);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
}

void ToolTableDialog::setupTable() {
    QStringList headers;
    headers << "Tool Nr" << "Description" << "Delete";
    
    toolTable->setColumnCount(headers.size());
    toolTable->setHorizontalHeaderLabels(headers);
    
    toolTable->horizontalHeader()->setStretchLastSection(false);
    toolTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    toolTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    toolTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    
    toolTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolTable->setSelectionMode(QAbstractItemView::SingleSelection);
    toolTable->setAlternatingRowColors(true);

}

void ToolTableDialog::populateTable() {
    struct ToolData {
        int number;
        QString description;
    };
    
    QVector<ToolData> sampleTools = {
        {1, "External Turning Tool"},
        {2, "Internal Turning Tool"},
        {3, "Threading Tool"},
        {4, "Facing Tool"},
        {5, "Parting Tool"}
    };
    
    toolTable->setRowCount(sampleTools.size());
    
    for (int row = 0; row < sampleTools.size(); ++row) {
        const auto& tool = sampleTools[row];
        
        toolTable->setItem(row, 0, new QTableWidgetItem(QString::number(tool.number)));
        toolTable->setItem(row, 1, new QTableWidgetItem(tool.description));
        
        auto* deleteBtn = new QPushButton("Delete");
        deleteBtn->setMaximumSize(60, 25);
        deleteBtn->setStyleSheet("QPushButton { font-size: 10px; color: red; }");
        toolTable->setCellWidget(row, 2, deleteBtn);

        connect(deleteBtn, &QPushButton::clicked, [this, row]() { removeTool(row); });
    }
}

void ToolTableDialog::connectSignals() {
    connect(addButton, &QPushButton::clicked, this, &ToolTableDialog::addTool);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ToolTableDialog::addTool() {
    int newRow = toolTable->rowCount();
    toolTable->insertRow(newRow);
    
    toolTable->setItem(newRow, 0, new QTableWidgetItem(QString::number(newRow + 1)));
    toolTable->setItem(newRow, 1, new QTableWidgetItem("New Tool"));
    
    auto* deleteBtn = new QPushButton("Delete");
    deleteBtn->setMaximumSize(60, 25);
    deleteBtn->setStyleSheet("QPushButton { font-size: 10px; color: red; }");
    toolTable->setCellWidget(newRow, 2, deleteBtn);

    connect(deleteBtn, &QPushButton::clicked, [this, newRow]() { removeTool(newRow); });
    
    toolTable->selectRow(newRow);
}

void ToolTableDialog::removeTool() {
    int currentRow = toolTable->currentRow();
    if (currentRow >= 0) {
        removeTool(currentRow);
    }
}

void ToolTableDialog::removeTool(int row) {
    if (row >= 0 && row < toolTable->rowCount()) {
        int ret = QMessageBox::question(this, "Remove Tool", 
                                      "Are you sure you want to remove this tool?",
                                      QMessageBox::Yes | QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            toolTable->removeRow(row);
        }
    }
}

void ToolTableDialog::editTool() {
    int currentRow = toolTable->currentRow();
    if (currentRow >= 0) {
        editTool(currentRow);
    }
}

void ToolTableDialog::editTool(int row) {
    if (row >= 0 && row < toolTable->rowCount()) {
        QTableWidgetItem* item = toolTable->item(row, 1);
        if (item) {
            toolTable->editItem(item);
        }
    }
}

void ToolTableDialog::onToolSelectionChanged() {
}