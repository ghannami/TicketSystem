
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

class TreeItemData;
class TreeModel;

class TreeItem
{
    friend class TreeModel;

public:
    TreeItem(TreeItemData * itemData, TreeItem *parent = 0);
    virtual ~TreeItem();

    TreeItem *child(int row);
    QList<TreeItem*> childItems();

    int childRow(TreeItem * child);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role) const;
    void setData(int column, QVariant value, int role);
    int row() const;
    void setParent(TreeItem * parent);
    TreeItem *parent();
    TreeModel * model();
    void setModel(TreeModel * model);
    Qt::ItemFlags flags();
    int position();   /// Die Position des Items bei seinem Parent
    TreeItemData * itemData();
private:
    /// Diese Funktionen dürfen nur vom Model benutzt werden,
    /// damit das Model Index für die neuen Elementen reservieren kann
    void appendChild(TreeItem *child);
    void addChild(TreeItem * child, int position);
    void detachChild(TreeItem * child);

    QList<TreeItem*> m_childItems;
    TreeItemData * m_treeDataItem;
    TreeItem *m_parentItem;
    TreeModel *m_model;
    QVector<QVariant> m_userRoleData;
};

#endif // TREEITEM_H
