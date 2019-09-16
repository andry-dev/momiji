#ifndef HELP_WINDOW_H
#define HELP_WINDOW_H

#include <QSortFilterProxyModel>
#include <QWidget>
#include <qwidget.h>

#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTableView>

#include <memory>

namespace Ui
{
    class HelpWindow;
}

struct ItemData
{
    QString title;
    QString mapping;
};

Q_DECLARE_METATYPE(ItemData)

// I'm too scared to move away from pointer hell, I don't know what Qt would do.
// So here I am, writing code that I don't like.

class TreeItem
{
public:
    explicit TreeItem(ItemData data, TreeItem* parent = nullptr);

    void appendChild(std::unique_ptr<TreeItem>&& child);

    TreeItem* child(int row);

    auto childCount() const noexcept;

    int row() const;

    int columnCount() const noexcept;

    ItemData data();

    TreeItem* parent() noexcept;

private:
    void setupModelData() {}

    std::vector<std::unique_ptr<TreeItem>> m_children;

    ItemData m_data;

    TreeItem* m_parent;
};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(ItemData data, QObject* parent = nullptr);

    virtual ~TreeModel() override;

    QVariant data(const QModelIndex& index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex
    index(int row,
          int column,
          const QModelIndex& parent = QModelIndex {}) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = QModelIndex {}) const override;

    int columnCount(const QModelIndex& parent = QModelIndex {}) const override;

private:
    void setupModelData();

    std::unique_ptr<TreeItem> m_root;
};

class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget* parent = nullptr);
    ~HelpWindow();

    void changeTo(const QString& str);

private slots:
    void on_searchResults_clicked(const QModelIndex& index);

    void on_searchBar_textChanged();

private:
    Ui::HelpWindow* ui;
    std::unique_ptr<TreeModel> m_treeItemModel;
    std::unique_ptr<QSortFilterProxyModel> m_filterModel;
};

#endif
