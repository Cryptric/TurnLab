//
// Created by gawain on 9/6/25.
//

#include "MainWindow.h"
#include <QApplication>
#include <QAction>
#include <QStyle>
#include <QSplitter>

MainWindow::MainWindow(const MachineConfig& config, const ToolTable& tools, QWidget *parent)
    : machineConfig(config), toolTable(tools), QMainWindow(parent), centralWidget(nullptr), mainLayout(nullptr), mainSplitter(nullptr), leftPanel(nullptr), currentLeftWidget(nullptr), rightContentArea(nullptr), ribbonBar(nullptr) {
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
    currentLeftWidget = leftPanel;
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



    facingAction = ribbonBar->addAction(QIcon(":/res/icons/facing.png"), "Facing");
    turningAction = ribbonBar->addAction(QIcon(":/res/icons/turning.png"), "Turning");
    contouringAction = ribbonBar->addAction(QIcon(":/res/icons/contouring.png"), "Contouring");
    threadingAction = ribbonBar->addAction(QIcon(":/res/icons/threading.png"), "Threading");
    partingAction = ribbonBar->addAction(QIcon(":/res/icons/parting.png"), "Parting");
    drillingAction = ribbonBar->addAction(QIcon(":/res/icons/drilling.png"), "Drilling");

    ribbonBar->addSeparator();

    QAction* toolTableAction = ribbonBar->addAction(QIcon(":res/icons/tool.png"), "Tool Table");
    QAction* machineConfigAction = ribbonBar->addAction(QIcon(":/res/icons/lathe.png"), "Machine Configuration");

    ribbonBar->addSeparator();

    generateGCodeAction = ribbonBar->addAction(QIcon(":/res/icons/export.png"), "Generate GCode");

    connect(toolTableAction, &QAction::triggered, this, &MainWindow::showToolTable);
    connect(machineConfigAction, &QAction::triggered, this, [this]() {emit this->onMachineConfigPressed(); });
    connect(newAction, &QAction::triggered, this, [this]() {emit this->onLoadDXFPressed(); });
    connect(generateGCodeAction, &QAction::triggered, this, [this]() {emit this->onGenerateGCodePressed(); });

    connect(facingAction, &QAction::triggered, this, [this]() {emit this->onFacingPressed(); });
    connect(turningAction, &QAction::triggered, this, [this]() {emit this->onTurningPressed(); });
    connect(contouringAction, &QAction::triggered, this, [this]() {emit this->onContouringPressed(); });
    connect(threadingAction, &QAction::triggered, this, [this]() {emit this->onThreadingPressed(); });
    connect(partingAction, &QAction::triggered, this, [this]() {emit this->onPartingPressed(); });
    connect(drillingAction, &QAction::triggered, this, [this]() {emit this->onDrillingPressed(); });
}

void MainWindow::showToolTable() {
    ToolTableDialog dialog(this);
    dialog.exec();
}

void MainWindow::setProject(const Project& project) const {
    geometryView->setGeometry(project.geometry);
    geometryView->plotStock(project.stockMaterial);
    geometryView->replot();
    leftPanel->setProject(project);
}

GeometryView& MainWindow::getGeometryView() const {
    return *geometryView;
}

LeftPanel& MainWindow::getLeftPanel() const {
    return *leftPanel;
}

void MainWindow::enableOperationButtons() {
    facingAction->setEnabled(true);
    turningAction->setEnabled(true);
    contouringAction->setEnabled(true);
    threadingAction->setEnabled(true);
    partingAction->setEnabled(true);
    drillingAction->setEnabled(true);
}

void MainWindow::disableOperationButtons() {
    facingAction->setEnabled(false);
    turningAction->setEnabled(false);
    contouringAction->setEnabled(false);
    threadingAction->setEnabled(false);
    partingAction->setEnabled(false);
    drillingAction->setEnabled(false);
}

void MainWindow::replaceLeftPanel(QWidget* newWidget) {
    if (newWidget == nullptr) {
        return;
    }

    // Store current splitter sizes to maintain proportions
    QList<int> sizes = mainSplitter->sizes();

    // Remove current left widget from splitter (but don't delete it)
    mainSplitter->widget(0)->setParent(nullptr);

    // Insert new widget at position 0 (left side)
    mainSplitter->insertWidget(0, newWidget);
    currentLeftWidget = newWidget;

    // Restore splitter sizes
    mainSplitter->setSizes(sizes);
}

void MainWindow::restoreLeftPanel() {
    if (currentLeftWidget == leftPanel) {
        return; // Already showing left panel
    }

    // Store current splitter sizes to maintain proportions
    QList<int> sizes = mainSplitter->sizes();

    // Remove current left widget from splitter (but don't delete it)
    mainSplitter->widget(0)->setParent(nullptr);

    // Insert left panel at position 0 (left side)
    mainSplitter->insertWidget(0, leftPanel);
    currentLeftWidget = leftPanel;

    // Restore splitter sizes
    mainSplitter->setSizes(sizes);
}
