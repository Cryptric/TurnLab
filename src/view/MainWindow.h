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


class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QSplitter* mainSplitter;
    LeftPanel* leftPanel;
    QWidget* rightContentArea;
    QToolBar* ribbonBar;
    GeometryView* geometryView;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showToolTable();

private:
    void setupUI();
    void createRibbonBar();


};



#endif //MAINWINDOW_H
