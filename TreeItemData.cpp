#include "TreeItemData.h"

void TreeItemData::setData(int column, const QVariant &value, int role)
{
    /// Wenn die Unterklasse diesen Wert für diesen Role und Column nicht speichern kann,
    /// dann wird es unter m_additionalData gespeichert
    if(!setValue(column, value, role))
    {
        QPair<int,int> pair;
        pair.first = column;
        pair.second = role;
        m_additionalData.insert(pair, value);
    }
}

QVariant TreeItemData::data(int column, int role)
{
    QVariant v = value(column, role);
    if(v.isNull())
    {
        QPair<int,int> pair;
        pair.first = column;
        pair.second = role;
        v = m_additionalData.value(pair);
    }
    return v;
}


Qt::ItemFlags TreeItemData::flags(const QModelIndex &index)
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
