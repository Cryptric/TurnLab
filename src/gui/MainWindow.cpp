//
// Created by gawain on 9/6/25.
//

#include "MainWindow.h"
#include <QApplication>
#include <QAction>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), centralWidget(nullptr), mainLayout(nullptr), ribbonBar(nullptr) {
    setupUI();
    resize(1920, 1080);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    this->setWindowIcon(QIcon(":res/icons/lathe.png"));

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    createRibbonBar();
    
    QWidget* contentArea = new QWidget();
    contentArea->setStyleSheet("background-color: #f0f0f0;");
    mainLayout->addWidget(contentArea, 1);
}

void MainWindow::createRibbonBar() {
    ribbonBar = addToolBar("Ribbon");
    ribbonBar->setMovable(false);
    ribbonBar->setFloatable(false);
    ribbonBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ribbonBar->setIconSize(QSize(48, 48));
    
    QAction* newAction = ribbonBar->addAction(QIcon(":res/icons/import.png"), "Load DXF");
    QAction* openAction = ribbonBar->addAction(QIcon(":res/icons/folder.png"), "Open");
    QAction* saveAction = ribbonBar->addAction(QIcon(":res/icons/save.png"), "Save");

    ribbonBar->addSeparator();



    QAction* facingAction = ribbonBar->addAction(QIcon(":/res/icons/facing.png"), "Facing");
    QAction* turningAction = ribbonBar->addAction(QIcon(":/res/icons/turning.png"), "Turning");
    QAction* contouringAction = ribbonBar->addAction(QIcon(":/res/icons/contouring.png"), "Contouring");
    QAction* threadingAction = ribbonBar->addAction(QIcon(":/res/icons/threading.png"), "Threading");
    QAction* partingAction = ribbonBar->addAction(QIcon(":/res/icons/parting.png"), "Parting");
    QAction* boringAction = ribbonBar->addAction(QIcon(":/res/icons/drilling.png"), "Drilling");

    ribbonBar->addSeparator();

    QAction* toolTableAction = ribbonBar->addAction(QIcon(":res/icons/tool.png"), "Tool Table");
    QAction* machineConfigAction = ribbonBar->addAction(QIcon(":/res/icons/lathe.png"), "Machine Configuration");
    
    connect(toolTableAction, &QAction::triggered, this, &MainWindow::showToolTable);
}

void MainWindow::showToolTable() {
    ToolTableDialog dialog(this);
    dialog.exec();
}
