#include "TicketItem.h"
#include <QColor>
#include <QPixmap>

TicketItem::TicketItem()
{

}

QVariant TicketItem::data(int column, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case 0:
            return QVariant();
        case TicketID:
            return m_record.value("id");
            break;
        case TicketTitle:
            return m_record.value("title");
            break;
        case StateName:
            return m_record.value("statename");
            break;
        case TicketDate:
            return m_record.value("date");
            break;
        case FromUser:
            return m_record.value("fromuser");
            break;
        case ToUser:
            return m_record.value("touser");
            break;
        case PriorityName:
            return m_record.value("priorityname");
            break;
        case Categorie:
            return m_record.value("categoriename");
            break;
        case Project:
            return m_record.value("projectname");
            break;
        }
    }
    else if(role == Qt::BackgroundColorRole)
    {
        if(m_record.value("state").toInt() == 1)
            return QColor(230,0,0,100);
        if(m_record.value("state").toInt() == 2)
            return QColor(0,0,230,100);
        if(m_record.value("state").toInt() == 3)
            return QColor(0,230,50,100);
    }
    else if(role == Qt::DecorationRole)
    {
        if(column == TicketIcon)
        {
            QPixmap p;
            if(m_record.value("type") == 1)
                p = QPixmap(":/icon/bug");
            else if(m_record.value("type")==2)
                p = QPixmap(":/icon/tip");

            return p.scaledToHeight(22, Qt::SmoothTransformation);
        }
    }
    return QVariant();

}

void TicketItem::setData(int column, QVariant value, int role)
{

}

QSqlRecord TicketItem::record() const
{
    return m_record;
}

void TicketItem::setRecord(const QSqlRecord &record)
{
    m_record = record;
}

Qt::ItemFlags TicketItem::flags()
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

int TicketItem::ticketID() const
{
    return m_record.value("id").toInt();
}

