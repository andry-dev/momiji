#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MemoryModel.h"
#include <iostream>

const QColor g_highlightColor { 61, 174, 233 };

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_memoryModel(new MemoryModel(MemoryType::Executable))
    , m_stackModel(new MemoryModel(MemoryType::Stack))
{
    ui->setupUi(this);

    ui->tblMemView->setModel(m_memoryModel);
    ui->tblMemView->horizontalHeader()->setStretchLastSection(true);
    ui->tblStackView->setModel(m_stackModel);
    ui->tblStackView->horizontalHeader()->setStretchLastSection(true);

    auto& registers = *ui->registers;

    for (std::size_t i = 0; i < m_dataRegisters.size(); ++i)
    {
        m_dataRegisters[i] = registers.item(int(i), 1);
    }

    for (std::size_t i = 0; i < m_addressRegisters.size(); ++i)
    {
        m_addressRegisters[i] = registers.item(int(i + 8), 1);
    }
}

MainWindow::~MainWindow()
{
    delete m_memoryModel;
    delete m_stackModel;
    delete ui;
}

void MainWindow::on_sourceCode_textChanged()
{
    parse();
}

void MainWindow::on_registers_itemChanged(QTableWidgetItem* item)
{
    int reg = item->row();
}

void MainWindow::parse()
{
    std::string str = ui->sourceCode->toPlainText().toStdString();
    auto res        = m_emulator.newState(str);

    if (res.has_value())
    {
        ui->statusBar->showMessage("Error around line " +
                                   QString::number((*res).line) + ": " +
                                   QString::fromStdString((*res).codeStr));

        return;
    }
    else
    {
        ui->statusBar->showMessage("Build OK");
        updateEmuValues();
    }
}

void MainWindow::updateRegisters()
{
    const auto& states  = m_emulator.getStates();
    const auto& lastSys = states.back();

    const auto pc = lastSys.cpu.programCounter.address;

    for (std::size_t i = 0; i < lastSys.cpu.dataRegisters.size(); ++i)
    {
        auto& datareg = lastSys.cpu.dataRegisters[i].value;
        auto& addreg  = lastSys.cpu.addressRegisters[i].value;
        m_dataRegisters[i]->setText(QString::number(datareg));
        m_addressRegisters[i]->setText(QString::number(addreg));
    }

    ui->registers->item(16, 1)->setText(QString::number(pc));
}

void MainWindow::updateEmuValues()
{
    const auto& lastSys = m_emulator.getStates().back();
    const auto pc       = lastSys.cpu.programCounter.address;
    const auto sp       = lastSys.cpu.addressRegisters[7].value;

    m_memoryModel->setMemory(lastSys.mem, pc, sp);
    m_stackModel->setMemory(lastSys.mem, pc, sp);

    updateRegisters();
}

void MainWindow::on_actionStep_triggered()
{
    if (m_emulator.step())
    {
        updateEmuValues();
    }
}

void MainWindow::on_actionRollback_triggered()
{
    if (m_emulator.rollbackSys())
    {
        updateEmuValues();
    }
}

void MainWindow::on_actionReset_triggered()
{
    m_emulator.reset();

    updateEmuValues();
}

void MainWindow::on_actionParse_triggered()
{
    parse();
}
