#include "ToolTableDialog.h"

ToolTableDialog::ToolTableDialog(QWidget *parent)
    : QDialog(parent), toolTable(nullptr), addButton(nullptr), okButton(nullptr), cancelButton(nullptr) {
    setWindowTitle("Tool Table");
    setModal(true);
    resize(800, 600);
    
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
    headers << "Tool Nr" << "Description" << "ISO Code" << "Insert Visualization" << "Edit" << "Delete";
    
    toolTable->setColumnCount(headers.size());
    toolTable->setHorizontalHeaderLabels(headers);
    
    toolTable->horizontalHeader()->setStretchLastSection(false);
    toolTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    toolTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    toolTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    toolTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    toolTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    toolTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    
    toolTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    toolTable->setSelectionMode(QAbstractItemView::SingleSelection);
    toolTable->setAlternatingRowColors(true);

}

void ToolTableDialog::populateTable() {
    struct ToolData {
        int number;
        QString description;
        QString isoCode;
    };
    
    QVector<ToolData> sampleTools = {
        {1, "External Turning Tool", "CNMG120408"},
        {2, "Internal Turning Tool", "VCMT160408"},
        {3, "Threading Tool", "16ER1.5ISO"},
        {4, "Facing Tool", "TNMG160408"},
        {5, "Parting Tool", "MGMN200"}
    };
    
    toolTable->setRowCount(sampleTools.size());
    
    for (int row = 0; row < sampleTools.size(); ++row) {
        const auto& tool = sampleTools[row];
        
        toolTable->setItem(row, 0, new QTableWidgetItem(QString::number(tool.number)));
        toolTable->setItem(row, 1, new QTableWidgetItem(tool.description));
        toolTable->setItem(row, 2, new QTableWidgetItem(tool.isoCode));
        
        auto* visualLabel = new QLabel("[Insert]");
        visualLabel->setAlignment(Qt::AlignCenter);
        visualLabel->setStyleSheet("background-color: #e0e0e0; border: 1px solid #ccc; padding: 4px;");
        toolTable->setCellWidget(row, 3, visualLabel);
        
        auto* editBtn = new QPushButton("Edit");
        editBtn->setMaximumSize(60, 25);
        editBtn->setStyleSheet("QPushButton { font-size: 10px; }");
        toolTable->setCellWidget(row, 4, editBtn);
        
        auto* deleteBtn = new QPushButton("Delete");
        deleteBtn->setMaximumSize(60, 25);
        deleteBtn->setStyleSheet("QPushButton { font-size: 10px; color: red; }");
        toolTable->setCellWidget(row, 5, deleteBtn);
        
        connect(editBtn, &QPushButton::clicked, [this, row]() { editTool(row); });
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
    toolTable->setItem(newRow, 2, new QTableWidgetItem("CNMG120408"));
    
    auto* visualLabel = new QLabel("[Insert]");
    visualLabel->setAlignment(Qt::AlignCenter);
    visualLabel->setStyleSheet("background-color: #e0e0e0; border: 1px solid #ccc; padding: 4px;");
    toolTable->setCellWidget(newRow, 3, visualLabel);
    
    auto* editBtn = new QPushButton("Edit");
    editBtn->setMaximumSize(60, 25);
    editBtn->setStyleSheet("QPushButton { font-size: 10px; }");
    toolTable->setCellWidget(newRow, 4, editBtn);
    
    auto* deleteBtn = new QPushButton("Delete");
    deleteBtn->setMaximumSize(60, 25);
    deleteBtn->setStyleSheet("QPushButton { font-size: 10px; color: red; }");
    toolTable->setCellWidget(newRow, 5, deleteBtn);
    
    connect(editBtn, &QPushButton::clicked, [this, newRow]() { editTool(newRow); });
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