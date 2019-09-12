#include "MemoryModel.h"

#include <QBrush>

#include <momiji/Decoder.h>

const QBrush g_defColor { QColor { 200, 200, 100 } };

MemoryModel::MemoryModel(MemoryType type)
    : m_memory { momiji::NullMemoryView {} }
    , m_type(type)
{
}

void MemoryModel::setMemory(momiji::ConstExecutableMemoryView mem,
                            std::uint32_t pc,
                            std::uint32_t sp)
{
    m_memory         = mem;
    m_programCounter = pc;
    m_stackPointer   = sp;

    /*
    setHeaderData(0, Qt::Horizontal, QString{"Address"});
    setHeaderData(1, Qt::Horizontal, QString{"Value"});
    setHeaderData(2, Qt::Horizontal, QString{"Code"});
    */

    emit layoutChanged();
}

int MemoryModel::rowCount(const QModelIndex& parent) const
{
    std::int64_t begin = 0;
    std::int64_t end   = 0;

    switch (m_type)
    {
    case MemoryType::Executable:
        begin = m_memory.executableMarker.begin;
        end   = m_memory.executableMarker.end;
        break;

    case MemoryType::Stack:
        begin = m_memory.stackMarker.begin;
        end   = m_memory.stackMarker.end;
        break;
    }

    return (end - begin) / 2;
}

int MemoryModel::columnCount(const QModelIndex& parent) const
{
    switch (m_type)
    {
    case MemoryType::Executable:
        return 3;

    case MemoryType::Stack:
        return 2;
    }

    return 0;
}

QVariant MemoryModel::getExecData(const QModelIndex& index, int role) const
{
    const auto& marker = m_memory.executableMarker;

    const auto begin = marker.begin + (index.row() * 2);
    const auto end   = marker.end;

    if ((role == Qt::BackgroundRole) && (m_programCounter == begin))
    {
        return g_defColor;
    }

    switch (index.column())
    {
    case 0:
        return { QString { "%1" }.arg(begin, 8, 16, QChar { '0' }) };

    case 1:
    {
        std::uint8_t higher = m_memory.read8(std::uint64_t(begin)).value_or(0);
        std::uint8_t lower  = 0;
        if ((begin + 1) < end)
        {
            lower = m_memory.read8(std::uint64_t(begin + 1)).value_or(0);
        }

        return { QString::number(higher, 16) + " " +
                 QString::number(lower, 16) };
    }

    case 2:
    {
        const auto decodedInstr =
            momiji::decode(m_memory, std::uint64_t(begin));
        return { QString::fromStdString(decodedInstr.string) };
    }
    }

    return {};
}

QVariant MemoryModel::getStackData(const QModelIndex& index, int role) const
{
    const auto& marker = m_memory.stackMarker;

    const auto begin = marker.begin;
    const auto end   = marker.end - (index.row() * 2) - 2;

    if ((role == Qt::BackgroundRole) && (m_stackPointer == end))
    {
        return g_defColor;
    }

    switch (index.column())
    {
    case 0:
        return { QString { "%1" }.arg(end, 8, 16, QChar { '0' }) };

    case 1:
    {
        std::uint8_t higher = m_memory.read8(std::uint64_t(end)).value_or(0);
        std::uint8_t lower  = 0;
        if ((end - 1) >= begin)
        {
            lower = m_memory.read8(std::uint64_t(end - 1)).value_or(0);
        }

        return { QString::number(higher, 16) + " " +
                 QString::number(lower, 16) };
    }
    }

    return {};
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const
{
    const QBrush color { QColor { 200, 200, 100 } };

    if (role != Qt::DisplayRole && role != Qt::BackgroundRole)
    {
        return {};
    }

    if (m_memory.empty())
    {
        return {};
    }

    switch (m_type)
    {
    case MemoryType::Executable:
        return getExecData(index, role);

    case MemoryType::Stack:
        return getStackData(index, role);
    }

    return {};
}

Qt::ItemFlags MemoryModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant
MemoryModel::headerData(int column, Qt::Orientation orientation, int role) const
{
    if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
    {
        return {};
    }

    switch (column)
    {
    case 0:
        return QString { tr("Address") };

    case 1:
        return QString { tr("Value") };

    case 2:
        return QString { tr("Code") };

    default:
        return {};
    }
}
