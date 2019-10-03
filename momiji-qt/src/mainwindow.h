#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <array>
#include <memory>

#include <momiji/Emulator.h>

#include "MemoryModel.h"

#include "helpwindow.h"
#include "aboutdialog.h"

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

    void on_actionBuild_triggered();

    void on_actionExecute_triggered();

    void on_actionStep_triggered();

    void on_actionRollback_triggered();

    void on_actionReset_triggered();

    void on_actionManual_triggered();

    void on_actionRetain_system_states_triggered();

    void on_actionAbout_triggered();

private:
    void updateEmuValues();
    void updateRegisters();

    void parse();

    momiji::Emulator m_emulator;

    Ui::MainWindow* ui;
    MemoryModel* m_memoryModel;
    MemoryModel* m_stackModel;
    std::unique_ptr<HelpWindow> m_helpWindow;
    std::unique_ptr<AboutDialog> m_aboutDialog;

    std::array<QTableWidgetItem*, 8> m_dataRegisters;
    std::array<QTableWidgetItem*, 8> m_addressRegisters;
};

#endif // MAINWINDOW_H
