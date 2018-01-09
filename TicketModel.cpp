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
    std::shared_ptr<TicketItem> tItem = item(index);
    return data(tItem, index.column(), role);
}

QVariant TicketModel::data(std::shared_ptr<TicketItem> item, int column, int role) const
{
    return item->data(column, role);
}

bool TicketModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    std::shared_ptr<TicketItem> tItem = item(index);
    setData(tItem, index.column(), value, role);
    return true;
}

bool TicketModel::setData(std::shared_ptr<TicketItem> item, int column, const QVariant &value, int role)
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

    std::shared_ptr<TicketItem> tItem = m_items.at(row);
    if (tItem)
        return createIndex(row, column, tItem.get());
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

std::shared_ptr<TicketItem> TicketModel::item(QModelIndex index) const
{
    if(!index.isValid())
        return nullptr;
    if(index.row() >= 0 && index.row() < m_items.size())
        return m_items.at(index.row());
    else
        return nullptr;
}

void TicketModel::addItem(std::shared_ptr<TicketItem>child)
{
    layoutAboutToBeChanged();
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items << child;
    endInsertRows();
    layoutChanged();
}

void TicketModel::removeItem(std::shared_ptr<TicketItem> item)
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
    //layoutAboutToBeChanged();
    beginResetModel();
    m_items.clear();

    QMap<int, int> unread;
    QSqlQuery unredQuery(QString("select id, ticket, max(viewed) as viewed from comments where to_user = %1 and viewed = 0 group by ticket")
                         .arg(Global::i()->userID()), Global::i()->db());
    while(unredQuery.next())
    {
        unread.insert(unredQuery.value("ticket").toInt(), unredQuery.value("viewed").toInt());
    }

//    QString q    = " SELECT t.id as id, t.title as title, s.id as state, s.name as statename , t.date as date, ";
//    q           += " t.processed_by as processed_by, t.tested_by as tested_by, t.from_user as from_user_id, t.to_user as to_user_id, ";
//    q           += " user1.name as fromuser, user2.name as touser, prio.name as priorityname, t.type as ticket_type, sys.id as systemversion, cat.name as categoriename ";
//    q           += " , cstm.id as customer, cstm.name as customername ";
//    q           += " FROM ticket t, state s, user user1, user user2, priority prio, system_version sys, system_unit_categorie cat, customer cstm ";
//    q           += " where t.state = s.id and user1.id = t.from_user and user2.id = t.to_user and prio.id = t.priority ";
//    q           += " and cat.id = t.unit_categorie and sys.id = t.system_version and cstm.id = t.customer ";
    QString q    = "  SELECT * from ticket tkt, priority prio  WHERE tkt.priority = prio.id ";
    if(filterObject()->stateID() > 0)
        q+= " AND tkt.state = " +QString::number(filterObject()->stateID()) + " ";
    if(filterObject()->typeID() > 0)
        q+= " AND tkt.type = " +QString::number(filterObject()->typeID()) + " ";
    if(filterObject()->toUserID() > 0)
        q+= " AND tkt.to_user = " +QString::number(filterObject()->toUserID()) + " ";
    if(filterObject()->fromUserID() > 0)
        q+= " AND tkt.from_user = " +QString::number(filterObject()->fromUserID()) + " ";
    if(filterObject()->priorityID() > 0)
        q+= " AND tkt.priority = " +QString::number(filterObject()->priorityID()) + " ";
    if(filterObject()->unitCategorieID() > 0)
        q+= " AND tkt.unit_categorie = " +QString::number(filterObject()->unitCategorieID()) + " ";
    if(filterObject()->systemVersionID() > 0)
        q+= " AND tkt.system_version = " +QString::number(filterObject()->systemVersionID()) + " ";
    if(filterObject()->customerID() > 0)
        q+= " AND tkt.customer = " +QString::number(filterObject()->customerID()) + " ";
    if(filterObject()->departmentID() > 0)
        q+= " AND tkt.department = " +QString::number(filterObject()->departmentID()) + " ";

    q+= " order by tkt.state asc, tkt.deadline asc, prio.number desc;";

    QSqlQuery query(q, Global::i()->db());
    while(query.next())
    {
        TicketItem *item = new TicketItem();
        item->setRecord(query.record());
        if(unread.contains(query.value("id").toInt()))
            item->setViewed(unread.value(query.value("id").toBool()));
        else
            item->setViewed(true);

        addItem(std::shared_ptr<TicketItem>(item));
    }
    //layoutChanged();
    endResetModel();
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
