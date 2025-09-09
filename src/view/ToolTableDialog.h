#ifndef TOOLTABLEDIALOG_H
#define TOOLTABLEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>

class ToolTableDialog : public QDialog {
    Q_OBJECT

private:
    QTableWidget* toolTable;
    QPushButton* addButton;
    QPushButton* okButton;
    QPushButton* cancelButton;

public:
    explicit ToolTableDialog(QWidget *parent = nullptr);
    ~ToolTableDialog() = default;

private slots:
    void addTool();
    void removeTool();
    void removeTool(int row);
    void editTool();
    void editTool(int row);
    void onToolSelectionChanged();

private:
    void setupUI();
    void setupTable();
    void populateTable();
    void connectSignals();
};

#endif // TOOLTABLEDIALOG_H