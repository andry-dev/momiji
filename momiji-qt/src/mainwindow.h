#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <array>

#include <momiji/Emulator.h>

#include "MemoryModel.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private slots:
    void on_sourceCode_textChanged();

    void on_registers_itemChanged(QTableWidgetItem* item);

    void on_actionStep_triggered();

    void on_actionRollback_triggered();

    void on_actionReset_triggered();

    void on_actionParse_triggered();

  private:
    void updateEmuValues();
    void updateRegisters();

    void parse();

    Ui::MainWindow* ui;
    MemoryModel* m_memoryModel;
    MemoryModel* m_stackModel;
    momiji::Emulator m_emulator;
    std::array<QTableWidgetItem*, 8> m_dataRegisters;
    std::array<QTableWidgetItem*, 8> m_addressRegisters;
};

#endif // MAINWINDOW_H
