#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MemoryModel.h"

#include <asl/types>
#include <iostream>

#include <momiji/Parser.h>

const QColor g_highlightColor { 61, 174, 233 };

namespace
{
    inline QString toString(momiji::ParserOperand op)
    {
        switch (op)
        {
        case momiji::ParserOperand::DataRegister:
            return MainWindow::tr("Data register");

        case momiji::ParserOperand::AddressRegister:
            return MainWindow::tr("Address register");

        case momiji::ParserOperand::Address:
            return MainWindow::tr("Address");

        case momiji::ParserOperand::AddressPre:
            return MainWindow::tr("Address with pre-decrement");

        case momiji::ParserOperand::AddressPost:
            return MainWindow::tr("Address with post-increment");

        case momiji::ParserOperand::AddressOffset:
            return MainWindow::tr("Address with offset");

        case momiji::ParserOperand::AddressIndex:
            return MainWindow::tr("Indexed Address");

        case momiji::ParserOperand::Immediate:
            return MainWindow::tr("Immediate");

        case momiji::ParserOperand::AbsoluteShort:
            return MainWindow::tr("Absolute short");

        case momiji::ParserOperand::AbsoluteLong:
            return MainWindow::tr("Absolute long");

        case momiji::ParserOperand::ProgramCounterIndex:
            return MainWindow::tr("Indexed PC");

        case momiji::ParserOperand::ProgramCounterOffset:
            return MainWindow::tr("PC with offset");
        }

        return "???";
    }

    inline QString toString(momiji::DataType op)
    {
        switch (op)
        {
        case momiji::DataType::Byte:
            return MainWindow::tr("Byte");

        case momiji::DataType::Word:
            return MainWindow::tr("Word");

        case momiji::DataType::Long:
            return MainWindow::tr("Long");
        }

        return "???";
    }

    template <typename Container>
    QString contToString(const Container& cont)
    {
        QString res {};

        for (std::size_t i = 0; i < cont.size(); ++i)
        {
            res += toString(cont[i]);

            if (i != (cont.size() - 1))
            {
                res += ", ";
            }
        }

        return res;
    }

    QString errorToString(const momiji::ParserError::ErrorType& error)
    {
        QString res {};

        using momiji::ParserError;
        using namespace momiji::errors;

        // clang-format off
        std::visit(asl::overloaded {
            [&](const UnknownError&) {
                res = MainWindow::tr("unknown error.");
            },

            [&](const NoInstructionFound& par) {
                res = MainWindow::tr("Instruction \"") +
                      QString::fromStdString(par.inputString) +
                      MainWindow::tr("\" not found");

                if (!par.alternatives.empty())
                {
                    res += "\nAlternatives:\n";
                    for (const auto& x : par.alternatives)
                    {
                        res += QString::fromStdString(x) + '\n';
                    }
                }
            },

            [&](const NoLabelFound& par) {
                res = MainWindow::tr("Label \"") +
                      QString::fromStdString(par.label) +
                      MainWindow::tr("\" not found.");
            },

            [&](const DataTypeMismatch& par) {
                res = MainWindow::tr("Data type ") +
                      toString(par.inputDataType) +
                      MainWindow::tr(" is not valid for this instruction.");

                if (!par.acceptedDataTypes.empty())
                {
                    res += MainWindow::tr("\nAccepted data types: ");
                    res += contToString(par.acceptedDataTypes);
                }
            },

            [&](const OperandTypeMismatch& par) {
                res = MainWindow::tr("Operand type ") +
                      toString(par.inputOp) +
                      MainWindow::tr(" is not valid for this instruction.");

                if (!par.acceptedOps.empty())
                {
                    res += MainWindow::tr("\nAccepted operand types: ");
                    res += contToString(par.acceptedOps);
                }
            },

            [&](const InvalidRegisterNumber& par) {
                res = MainWindow::tr("Invalid register number ") +
                      QString::number(par.input);
            },

            [&](const UnexpectedCharacter& par) {
                res = MainWindow::tr("Unexpected character '") +
                      QString{par.character} + "'.";
            },

            [&](const MissingCharacter& par) {
                res = MainWindow::tr("Missing a '") +
                      QString{par.character} + "'.";
            },

            [&](const UnknownOperand&) {
                res = MainWindow::tr("Unknown operand, are you sure the syntax is valid?");
            },

            [&](const UnexpectedSectionContent& par) {
                switch (par.section)
                {
                case momiji::ParserSection::Code:
                    res = MainWindow::tr("Unexpected variable definition in code section");
                    break;

                case momiji::ParserSection::Data:
                    res = MainWindow::tr("Unexpected executable instruction in data section");
                    break;
                }
            }

        }, error);
        // clang-format on

        return res;
    }
} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_memoryModel(new MemoryModel(MemoryType::Executable))
    , m_stackModel(new MemoryModel(MemoryType::Stack))
    , m_helpWindow(new HelpWindow)
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

    ui->dockParserOutput->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete m_memoryModel;
    delete m_stackModel;
    delete ui;
}

void MainWindow::on_sourceCode_textChanged()
{
    // parse();
}

void MainWindow::on_registers_itemChanged(QTableWidgetItem* /*item*/)
{
    // int reg = item->row();
}

void MainWindow::parse()
{
    std::string str = ui->sourceCode->toPlainText().toStdString();
    auto res        = m_emulator.newState(str);

    if (res.has_value())
    {
        const auto& error = *res;
        ui->dockParserOutput->setVisible(true);
        ui->textParserOutput->setText(
            MainWindow::tr("Error around line ") + QString::number(error.line) +
            MainWindow::tr(": ") + errorToString(error.errorType) +
            MainWindow::tr("\n\n\nContext: ") +
            QString::fromStdString(error.codeStr));

        ui->statusBar->showMessage(MainWindow::tr("Build error"));

        return;
    }
    else
    {
        ui->dockParserOutput->setVisible(false);
        ui->statusBar->showMessage(
            MainWindow::tr("Build completed successfully"));
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

    m_memoryModel->setMemory(lastSys.mem, pc, std::uint32_t(sp));
    m_stackModel->setMemory(lastSys.mem, pc, std::uint32_t(sp));

    updateRegisters();
}

void MainWindow::on_actionBuild_triggered()
{
    parse();
}

void MainWindow::on_actionExecute_triggered()
{
    // Execute code
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

void MainWindow::on_actionManual_triggered()
{
    m_helpWindow->show();
}
