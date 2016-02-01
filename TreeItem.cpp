#include <QStringList>
#include "TreeItem.h"
#include "TreeItemData.h"

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

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

void TreeItem::addChild(TreeItem *child, int position)
{
//    if(position < 0 || position > childItems.size())
//        return;
    m_childItems.insert(position, child);
    child->setParent(this);
}

void TreeItem::detachChild(TreeItem *child)
{
    child->setParent(0);
    if(!m_childItems.isEmpty())
        m_childItems.removeAll(child);
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
//    /// Spezielle Daten die versteckt sind, werden nur zur Hilfe benutzt wie etwa <RecordId> für einen Antrag in der Datenbank
//    if(role ==  Qt::UserRole)
//    {
//        if(!m_userRoleData.isEmpty())
//            return m_userRoleData.at(column);
//    }

    if(m_treeDataItem)
        return m_treeDataItem->data(column, role);

    return QVariant();
}

void TreeItem::setData(int column, QVariant value, int role)
{
    /// Spezielle Daten die versteckt sind, werden nur zur Hilfe benutzt wie etwa <RecordId> für einen Antrag in der Datenbank
//    if(role ==  Qt::UserRole)
//        m_userRoleData.insert(column, value);
//    /// Daten für Display- und EditRole
//    else if(role == Qt::EditRole)
//    {
//        if(m_treeDataItem)
//           m_treeDataItem->setValue(column, value.toString());
//    }
//    else{
//        QPair<int,int> pair;
//        pair.first = column;
//        pair.second = role;
//        m_additionalData.insert(pair, value);
//    }

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

Qt::ItemFlags TreeItem::flags()
{
    return m_treeDataItem->flags();
}

int TreeItem::position()
{
    return parent()->childRow(this);
}

TreeItemData *TreeItem::itemData()
{
    return m_treeDataItem;
}
