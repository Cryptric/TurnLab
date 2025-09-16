//
// Created by gawain on 9/6/25.
//

#include "MainWindow.h"
#include <QApplication>
#include <QAction>
#include <QStyle>
#include <QSplitter>

MainWindow::MainWindow(const MachineConfig& config, QWidget *parent)
    : machineConfig(config), QMainWindow(parent), centralWidget(nullptr), mainLayout(nullptr), mainSplitter(nullptr), leftPanel(nullptr), rightContentArea(nullptr), ribbonBar(nullptr) {
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
    
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    leftPanel = new LeftPanel(this);
    mainSplitter->addWidget(leftPanel);
    
    geometryView = new GeometryView(machineConfig, this);
    mainSplitter->addWidget(geometryView);


    
    mainSplitter->setSizes({250, 1000});
    mainSplitter->setCollapsible(0, false);
    mainSplitter->setCollapsible(1, false);
    
    mainLayout->addWidget(mainSplitter, 1);
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
    connect(machineConfigAction, &QAction::triggered, this, [this]() {emit this->onMachineConfigPressed(); });
}

void MainWindow::showToolTable() {
    ToolTableDialog dialog(this);
    dialog.exec();
}

void MainWindow::setProject(const Project& project) const {
    geometryView->setGeometry(project.geometry);
    geometryView->plotStock(project.stockMaterial);
    geometryView->replot();
}
