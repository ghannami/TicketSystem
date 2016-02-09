#include <QtGui>
#include "TreeItem.h"
#include "TreeModel.h"
#include "AddTreeItemCommand.h"
#include "RemoveTreeItemCommand.h"
#include <QUndoGroup>

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)

{
    m_columnCount = 1;
    setRoot(new TreeItem(0,0));
    m_undoStack = new QUndoStack();
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
    delete m_undoStack;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if(m_columnCount != 0)
        return m_columnCount;

    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItemView()->columnCount();
}

void TreeModel::setRoot(TreeItem *root)
{
    m_rootItem = root;
    m_rootItem->setModel(this);
    setRootView(m_rootItem);
    if(m_columnCount == 0)
        m_columnCount = m_rootItem->columnCount();
    connect(m_rootItem, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            this, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)));
    connect(m_rootItem, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            this, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)));
    connect(m_rootItem, SIGNAL(beginInsertRows(QModelIndex,int,int)), this, SLOT(onBeginInsertRows(QModelIndex,int,int)));
    connect(m_rootItem, SIGNAL(beginRemoveRows(QModelIndex,int,int)), this, SLOT(onBeginRemoveRows(QModelIndex,int,int)));
    connect(m_rootItem, SIGNAL(endInsertRows()), this, SLOT(onEndInsertRows()));
    connect(m_rootItem, SIGNAL(endRemoveRows()), this, SLOT(onEndRemoveRows()));
}

TreeItem *TreeModel::rootItem()
{
    return m_rootItem;
}

void TreeModel::setRootView(TreeItem *root)
{
    m_rootItemView = root;
}

TreeItem *TreeModel::rootItemView() const
{
    return m_rootItemView;
}

void TreeModel::addItem(TreeItem * child, TreeItem * parentItem, int position, bool undoable)
{
    if(undoable)
    {
        m_undoStack->push(new AddTreeItemCommand(this, child, parentItem, position));
    }
    else
        insertItem(child, parentItem, position);
}

void TreeModel::removeItem(TreeItem *treeItem, bool undoable)
{
    TreeItem *parentItem = treeItem->parent();
    if(!parentItem)
        return;

    if(undoable)
    {
        m_undoStack->push(new RemoveTreeItemCommand(this, treeItem, parentItem));
    }
    else
        detachItem(treeItem, parentItem);
}

void TreeModel::removeItems(QList<TreeItem *> treeItems, bool undoable)
{
    if(treeItems.isEmpty())
        return;

    m_undoStack->beginMacro("");
    QListIterator<TreeItem*> it(treeItems);
    while(it.hasNext())
        removeItem(it.next(), undoable);
    m_undoStack->endMacro();
}

void TreeModel::deleteItem(TreeItem *treeItem)
{
    removeItem(treeItem, false);
    delete treeItem;
}

void TreeModel::insertItem(TreeItem *child, TreeItem *parentItem, int position)
{
    if(!parentItem)
    {
        qWarning()<<"TreeModel::addItem der parent ist ungueltig";
        return;
    }
//    emit layoutAboutToBeChanged();
//    beginInsertRows(index(parentItem), position, position);
    parentItem->addChild(child, position);
//    endInsertRows();
//    emit layoutChanged();
}

void TreeModel::detachItem(TreeItem *treeItem, TreeItem *parentItem)
{
    if(!parentItem)
        return;

    if(!index(treeItem).isValid())
        return;
    //beginRemoveRows(index(parentItem), treeItem->position(), treeItem->position());
    parentItem->detachChild(treeItem);
    //endRemoveRows();
}

QUndoStack *TreeModel::undoStack() const
{
    return m_undoStack;
}

void TreeModel::updateLayout()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}



void TreeModel::onBeginInsertRows(const QModelIndex &parent, int first, int last)
{
    beginInsertRows(parent, first, last);
}

void TreeModel::onEndInsertRows()
{
    endInsertRows();
}

void TreeModel::onBeginRemoveRows(const QModelIndex &parent, int first, int last)
{
    beginRemoveRows(parent, first, last);
}

void TreeModel::onEndRemoveRows()
{
    endRemoveRows();
}

void TreeModel::clearModel()
{
    beginResetModel();
    setRootView(m_rootItem);
    /// löscht alle Unterelemente von m_rootItem
    /// die einzelne TreeItem löschen wiederum ihre Children im Destruktor
    foreach(TreeItem * child, m_rootItem->childItems())
    {
        delete child;
    }
    m_rootItem->m_childItems.clear();
    emit endResetModel();
}

void TreeModel::changeParent(TreeItem *item, TreeItem * newParent, int position)
{
    if(item->parent() == newParent)
        return;
    if(position < 0 || position > newParent->childCount())
        position = newParent->childCount();

    beginMoveRows(index(item->parent()), item->position(), item->position(), index(newParent), position);
    item->parent()->detachChild(item);
    newParent->addChild(item, position);
    endMoveRows();
}

void TreeModel::setHeaders(QStringList headers, Qt::Orientation orientation)
{
    if (orientation == Qt::Horizontal)
        m_hHeaders = headers;
    else
        m_vHeaders = headers;
    emit headerDataChanged(orientation, 0, headers.size() - 1);
}

void TreeModel::setColumnCount(int count)
{
    m_columnCount = count;
}
/*!
 * \brief TreeModel::findItem sucht nach einem bestimmten element.
 * \param keys enthählt die column und die Werte nach denen gesucht werden muss
 * \param role
 * \return gibt das erste gefundene TreeItem zurück
 */
TreeItem *TreeModel::findItem(QMap<int, QVariant> keys, int role)
{
    QStack<TreeItem*> stack;
    stack.push(m_rootItem);

    while(!stack.isEmpty())
    {
        TreeItem * tmp = stack.pop();
        for(int i = 0; i < tmp->childCount(); i++)
        {
            TreeItem * child = tmp->child(i);
            if(child)
            {
                bool b = true;
                QMapIterator<int, QVariant> it(keys);
                while(it.hasNext())
                {
                    it.next();
                    if(child->data(it.key(), role) != it.value())
                    {
                        b = false;
                        continue;
                    }
                }
                if(b)
                {
                    stack.clear();
                    return child;
                }
                stack.push(child);
            }
        }
    }
    return 0;
}

TreeItem *TreeModel::item(QModelIndex index) const
{
    return static_cast<TreeItem*>(index.internalPointer());
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    TreeItem *treeItem = item(index);
    return data(treeItem, index.column(), role);
}

QVariant TreeModel::data(TreeItem *treeItem, int column, int role) const
{
//    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::UserRole)
//    {
        return treeItem->data(column, role);
//    }
    return QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TreeItem * treeItem = item(index);
    setData(treeItem, index.column(), value, role);
    return true;
}

bool TreeModel::setData(TreeItem *treeItem, int column, const QVariant &value, int role)
{
    treeItem->setData(column, value, role);
    return true;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return item(index)->flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(!m_hHeaders.isEmpty() && section >= 0 && section < m_hHeaders.size())
            return m_hHeaders.at(section);
        else
            return m_rootItem->data(section, role);
    }
    else{
        if(!m_vHeaders.isEmpty() && section >= 0 && section < m_vHeaders.size())
            return m_vHeaders.at(section);
        return QVariant();
    }
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
//        parentItem = m_rootItem;
        parentItem = rootItemView();

    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::index(TreeItem *item)
{
    QStack<TreeItem *> parents;
    TreeItem *curr = item;
    parents.push(curr);
    while(curr->parent())
    {
        curr = curr->parent();
        parents.push(curr);
    }
    /// der erste Index ist immer vom rootItem
    QModelIndex mIndex = createIndex(-1,-1, rootItemView());
    parents.pop();
    TreeItem * tmpParent = rootItemView();
    QModelIndex pIndex = mIndex;

    while(!parents.isEmpty())
    {
        TreeItem *tmp = parents.pop();
        mIndex = index(tmpParent->childRow(tmp), 0, pIndex);
        tmpParent = tmp;
        pIndex = mIndex;
    }

    return mIndex;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItemView())
        return QModelIndex();

	if (!parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItemView();
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
