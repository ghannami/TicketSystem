#include "TicketModel.h"
#include "TicketItem.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "FilterObject.h"
#include "Global.h"

TicketModel::TicketModel(QObject *parent):
    QAbstractItemModel(parent)
{
    m_filterObject = std::shared_ptr<FilterObject>(new FilterObject());
}

TicketModel::~TicketModel()
{
    m_items.clear();
}

QVariant TicketModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    TicketItem *tItem = item(index);
    return data(tItem, index.column(), role);
}

QVariant TicketModel::data(TicketItem *item, int column, int role) const
{
    return item->data(column, role);
}

bool TicketModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TicketItem * tItem = item(index);
    setData(tItem, index.column(), value, role);
    return true;
}

bool TicketModel::setData(TicketItem *item, int column, const QVariant &value, int role)
{
    item->setData(column, value, role);
    return true;
}

Qt::ItemFlags TicketModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return item(index)->flags();
}

QVariant TicketModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(!m_hHeaders.isEmpty() && section >= 0 && section < m_hHeaders.size())
            return m_hHeaders.at(section);
    }
    return QVariant();
}

void TicketModel::setHeaders(QStringList headers)
{
    m_hHeaders = headers;
    emit headerDataChanged(Qt::Horizontal, 0, headers.size() - 1);
}

QModelIndex TicketModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TicketItem *tItem = m_items.at(row);
    if (tItem)
        return createIndex(row, column, tItem);
    else
        return QModelIndex();
}

int TicketModel::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}

int TicketModel::columnCount(const QModelIndex &parent) const
{
    return m_hHeaders.size();
}

TicketItem *TicketModel::item(QModelIndex index) const
{
    if(!index.isValid())
        return nullptr;
    if(index.row() >= 0 && index.row() < m_items.size())
        return m_items.at(index.row());
    else
        return nullptr;
}

void TicketModel::addItem(TicketItem *child)
{
    layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items << child;
    endInsertRows();
    layoutChanged();
}

void TicketModel::removeItem(TicketItem *item)
{
    layoutAboutToBeChanged();
    beginRemoveRows(QModelIndex(), m_items.indexOf(item), m_items.indexOf(item));
    m_items.removeAll(item);
    endRemoveRows();
    layoutChanged();
}

QModelIndex TicketModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

void TicketModel::updateModel()
{
    layoutAboutToBeChanged();
    m_items.clear();

    QString q   = "SELECT t.id as id, t.title as title, s.id as state, s.name as statename , t.date as date, ";
    q           += "user1.name as fromuser, user2.name as touser, prio.name as priorityname, t.type, pro.name as projectname, cat.name as categoriename ";
    q           += "FROM ticket t, state s, user user1, user user2, priority prio, projects pro, categories cat ";
    q           += "where t.state = s.id and user1.id = t.from_user and user2.id = t.to_user and prio.id = t.priority and pro.id = t.project and cat.id = t.categorie ";
    if(filterObject()->stateID() > 0)
        q+= " AND s.id = " +QString::number(filterObject()->stateID()) + " ";
    if(filterObject()->typeID() > 0)
        q+= " AND t.type = " +QString::number(filterObject()->typeID()) + " ";
    if(filterObject()->toUserID() > 0)
        q+= " AND t.to_user = " +QString::number(filterObject()->toUserID()) + " ";
    if(filterObject()->priorityID() > 0)
        q+= " AND t.priority = " +QString::number(filterObject()->priorityID()) + " ";
    if(filterObject()->categorieID() > 0)
        q+= " AND t.categorie = " +QString::number(filterObject()->categorieID()) + " ";
    if(filterObject()->projectID() > 0)
        q+= " AND t.project = " +QString::number(filterObject()->projectID()) + " ";

    q           += "order by prio.number desc, t.date asc;";

    QSqlQuery query(q, Global::i()->db());
    while(query.next())
    {
        TicketItem *item = new TicketItem();
        item->setRecord(query.record());
        addItem(item);
    }
    layoutChanged();
}

std::shared_ptr<FilterObject> TicketModel::filterObject() const
{
    return m_filterObject;
}

QModelIndex TicketModel::index(int ticketID) const
{
    for(int i = 0; i < m_items.count(); i++)
    {
        if(ticketID == m_items.at(i)->ticketID())
            return index(i,0);
    }
    return QModelIndex();
}
