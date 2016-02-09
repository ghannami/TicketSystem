#include <QStringList>
#include "TreeItem.h"
#include "TreeItemData.h"
#include "TreeModel.h"

TreeItem::TreeItem(TreeItemData *itemData, TreeItem *parent):
    m_treeDataItem(itemData)
{
    m_parentItem = parent;
    if(m_parentItem)
        setModel(m_parentItem->model());
}

TreeItem::~TreeItem()
{
    if(!m_childItems.isEmpty())
        qDeleteAll(m_childItems);
    if(m_treeDataItem)
        delete m_treeDataItem;
}

void TreeItem::addChild(TreeItem *child, int position)
{
    emit layoutAboutToBeChanged();
    emit beginInsertRows(m_model->index(this), position, position);
    m_childItems.insert(position, child);
    child->setParent(this);
    connect(child, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            this, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)));
    connect(child, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            this, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)));
    connect(child, SIGNAL(beginInsertRows(QModelIndex,int,int)), this, SIGNAL(beginInsertRows(QModelIndex,int,int)));
    connect(child, SIGNAL(endInsertRows()), this, SIGNAL(endInsertRows()));
    connect(child, SIGNAL(beginRemoveRows(QModelIndex,int,int)), this, SIGNAL(beginRemoveRows(QModelIndex,int,int)));
    connect(child, SIGNAL(endRemoveRows()), this, SIGNAL(endRemoveRows()));
    emit endInsertRows();
    emit layoutChanged();
}

void TreeItem::detachChild(TreeItem *child)
{
    emit layoutAboutToBeChanged();
    emit beginRemoveRows(m_model->index(child), child->position(), child->position());
    child->setParent(0);
    if(!m_childItems.isEmpty())
        m_childItems.removeAll(child);
    emit endRemoveRows();
    emit layoutChanged();
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

QList<TreeItem *> TreeItem::childItems()
{
    return m_childItems;
}

int TreeItem::childRow(TreeItem *child)
{
    return m_childItems.indexOf(child);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    if(m_treeDataItem)
        return m_treeDataItem->columnCount();
    return 0;
}

QVariant TreeItem::data(int column, int role) const
{
    if(m_treeDataItem)
        return m_treeDataItem->data(column, role);

    return QVariant();
}

void TreeItem::setData(int column, QVariant value, int role)
{
    if(m_treeDataItem)
       m_treeDataItem->setData(column, value, role);

}

TreeItem *TreeItem::parent()
{
    return m_parentItem;
}

TreeModel *TreeItem::model()
{
    return m_model;
}

void TreeItem::setModel(TreeModel *model)
{
    m_model = model;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

void TreeItem::setParent(TreeItem *parent)
{
    /// Parent und Child müssen im selben Model sein
    m_parentItem = parent;
    if(m_parentItem)
        setModel(m_parentItem->model());
}

Qt::ItemFlags TreeItem::flags(const QModelIndex &index)
{
    return m_treeDataItem->flags(index);
}

int TreeItem::position()
{
    if(parent())
        return parent()->childRow(this);
    return 0;
}

TreeItemData *TreeItem::itemData()
{
    return m_treeDataItem;
}
