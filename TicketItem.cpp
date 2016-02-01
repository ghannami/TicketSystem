#include "TicketItem.h"
#include <QColor>
#include <QPixmap>
#include <QFont>
#include "Global.h"
#include <QSqlQuery>

TicketItem::TicketItem()
{

}

TicketItem::~TicketItem()
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
            return QColor(230,0,0, 50);
        if(m_record.value("state").toInt() == 2)
            return QColor(0,0,250, 50);
        if(m_record.value("state").toInt() == 3)
            return QColor(0,250,50, 50);
    }
    else if(role == Qt::DecorationRole)
    {
        if(column == TicketIcon)
        {
            QPixmap p;
            if(m_record.value("type") == 1)
            {
                p = QPixmap(":/icon/bug");
                return p.scaledToHeight(22, Qt::SmoothTransformation);
            }
            else if(m_record.value("type")==2)
            {
                p = QPixmap(":/icon/tip");
                return p.scaledToHeight(22, Qt::SmoothTransformation);
            }
        }
    }
    else if(role == Qt::FontRole)
    {
        if(!m_viewed)
        {
            QFont f;
            f.setBold(true);
            return f;
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

bool TicketItem::viewed() const
{
    return m_viewed;
}

void TicketItem::setViewed(bool viewed)
{
    if(m_viewed != viewed)
    {
        m_viewed = viewed;
        int v = 0;
        if(m_viewed)
            v = 1;
        QSqlQuery query(QString("UPDATE comments set viewed = %1 where ticket = %2 and to_user = %3")
                        .arg(v).arg(ticketID()).arg(Global::i()->userID()), Global::i()->db());
        query.exec();
    }
}

int TicketItem::fromUser() const
{
    return m_record.value("from_user").toInt();
}

int TicketItem::toUser() const
{
    return m_record.value("to_user").toInt();
}

int TicketItem::processedBy() const
{
    return m_record.value("processed_by").toInt();
}

int TicketItem::testedBy() const
{
    return m_record.value("tested_by").toInt();
}

