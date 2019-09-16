#include "helpwindow.h"
#include "ui_helpwindow.h"

#include <QLocale>
#include <QSortFilterProxyModel>
#include <QTableView>

#include <memory>
#include <vector>

#include <array>

#include <QDebug>

#include <asl/types>

TreeItem::TreeItem(ItemData data, TreeItem* parent)
    : m_data(std::move(data))
    , m_parent(parent)
{
}

void TreeItem::appendChild(std::unique_ptr<TreeItem>&& child)
{
    m_children.push_back(std::move(child));
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= asl::ssize(m_children))
    {
        return nullptr;
    }

    return asl::saccess(m_children, row).get();
}

auto TreeItem::childCount() const noexcept
{
    return m_children.size();
}

int TreeItem::row() const
{
    if (m_parent)
    {
        const auto& parentVec = m_parent->m_children;

        return int(std::distance(
            std::begin(parentVec),
            std::find_if(std::begin(parentVec),
                         std::end(parentVec),
                         [this](const auto& x) { return x.get() == this; })));
    }

    return 0;
}

int TreeItem::columnCount() const noexcept
{
    return 1;
}

ItemData TreeItem::data()
{
    return m_data;
}

TreeItem* TreeItem::parent() noexcept
{
    return m_parent;
}

TreeModel::TreeModel(ItemData data, QObject* /*parent*/)
{
    m_root = std::make_unique<TreeItem>(std::move(data));

    setupModelData();
}

TreeModel::~TreeModel() {}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        qDebug() << "Not valid\n";
        return {};
    }

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    if (role != Qt::DisplayRole)
    {
        return {};
    }

    if (role == (Qt::UserRole))
    {
        qDebug() << "DIOCANE\n";
        return QVariant::fromValue<ItemData>(item->data());
    }

    return item->data().title;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int /*section*/,
                               Qt::Orientation /*orientation*/,
                               int /*role*/) const
{
    return {};
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return {};
    }

    TreeItem* child      = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = child->parent();

    if (parentItem == m_root.get())
    {
        return {};
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem = nullptr;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_root.get();
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return int(parentItem->childCount());
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }

    return m_root->columnCount();
}

void TreeModel::setupModelData()
{
    // Basics

    auto momijiBasics = std::make_unique<TreeItem>(
        ItemData { tr("Emulator basics"), "index" }, m_root.get());

    auto addBasicInfo = [&momijiBasics](auto displayName, auto pageName) {
        auto data = std::make_unique<TreeItem>(
            ItemData { displayName, pageName }, momijiBasics.get());

        momijiBasics->appendChild(std::move(data));
    };

    addBasicInfo(tr("Naming"), "naming");

    m_root->appendChild(std::move(momijiBasics));

    // Instructions

    auto instructions = std::make_unique<TreeItem>(
        ItemData { tr("Instructions"), "instr" }, m_root.get());

    auto addInstr = [&instructions](auto displayName, auto pageName) {
        auto instr = std::make_unique<TreeItem>(
            ItemData { displayName, pageName }, instructions.get());

        instructions->appendChild(std::move(instr));
    };

    addInstr("MOVE", "move");

    addInstr("ADD", "add");
    addInstr("SUB", "sub");
    addInstr("MULS", "muls");
    addInstr("MULU", "mulu");
    addInstr("DIVS", "divs");
    addInstr("DIVU", "divu");

    addInstr("CMP", "cmp");

    addInstr("B**", "bcc");
    addInstr("BRA", "bra");
    addInstr("BSR", "bsr");
    addInstr("RTS", "rts");

    addInstr("JMP", "jmp");
    addInstr("JSR", "jsr");

    addInstr("SWAP", "swap");
    addInstr("EXG", "exg");

    m_root->appendChild(std::move(instructions));
}

QModelIndex
TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return {};
    }

    TreeItem* parentItem = nullptr;

    if (!parent.isValid())
    {
        parentItem = m_root.get();
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* child = parentItem->child(row);
    if (child)
    {
        return createIndex(row, column, child);
    }

    return {};
}

HelpWindow::HelpWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::HelpWindow)
    , m_treeItemModel(
          std::make_unique<TreeModel>(ItemData { "Index", "index" }))
    , m_filterModel(std::make_unique<QSortFilterProxyModel>())

{
    ui->setupUi(this);
    qRegisterMetaType<ItemData>("ItemData");

    m_filterModel->setSourceModel(m_treeItemModel.get());
    m_filterModel->setRecursiveFilteringEnabled(true);
    m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    ui->searchResults->setModel(m_filterModel.get());
    ui->searchResults->setHeaderHidden(true);

    changeTo("index");
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::changeTo(const QString& str)
{
    ui->docView->setSource("qrc:/help/" + str + "_" + QLocale::system().name() +
                           ".html");
}

void HelpWindow::on_searchResults_clicked(const QModelIndex& index)
{
    const auto correctIndex = m_filterModel->mapToSource(index);

    auto ptr = correctIndex.internalPointer();

    ItemData item = static_cast<TreeItem*>(ptr)->data();

    changeTo(item.mapping);
}

void HelpWindow::on_searchBar_textChanged()
{
    m_filterModel->setFilterRegularExpression(ui->searchBar->toPlainText());
}
