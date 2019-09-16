#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include <QAbstractTableModel>
#include <momiji/Memory.h>

enum class MemoryType
{
    Executable,
    Stack,
};

class MemoryModel final : public QAbstractTableModel
{
    Q_OBJECT
public:
    MemoryModel(MemoryType type);
    virtual ~MemoryModel() override = default;

    void setMemory(momiji::ConstExecutableMemoryView mem,
                   std::uint32_t pc,
                   std::uint32_t sp);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int column,
                        Qt::Orientation orientation,
                        int role) const override;

signals:

public slots:

private:
    QVariant getExecData(const QModelIndex& index, int role) const;
    QVariant getStackData(const QModelIndex& index, int role) const;

    momiji::ConstExecutableMemoryView m_memory;
    std::uint32_t m_stackPointer;
    std::uint32_t m_programCounter;
    MemoryType m_type;
};

#endif // MEMORYMODEL_H
