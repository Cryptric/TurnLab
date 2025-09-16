//
// Created by gawain on 9/6/25.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QLabel>
#include <QSplitter>

#include "GeometryView.h"
#include "ToolTableDialog.h"
#include "LeftPanel.h"
#include "OperationConfigurationView.h"
#include "../model/Project.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    const MachineConfig& machineConfig;

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QSplitter* mainSplitter;
    LeftPanel* leftPanel;
    QWidget* rightContentArea;
    QToolBar* ribbonBar;
    GeometryView* geometryView;

    OperationConfigurationView* operationConfigView;


public:
    explicit MainWindow(const MachineConfig& config, QWidget *parent = nullptr);
    ~MainWindow();

    void setProject(const Project& project) const;

private slots:
    void showToolTable();

private:
    void setupUI();
    void createRibbonBar();

signals:
    void onMachineConfigPressed();
    void onToolTablePressed();

};



#endif //MAINWINDOW_H
