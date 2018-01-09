#include "TicketItem.h"
#include <QColor>
#include <QPixmap>
#include <QFont>
#include "Global.h"
#include <QSqlQuery>
#include "CommentItem.h"
#include <QDebug>
#include <QSqlError>

TicketItem::TicketItem(const QSqlRecord &record)
{
    init();
    setRecord(record);
}

TicketItem::TicketItem()
{
    init();
}

void TicketItem::init()
{
    setProcessedBy(-1);
    setTestedBy(-1);
    setPercentComplete(0);
    setCustomer(-1);
    setDepartment(-1);
    setFromUser(-1);
    setID(-1);
    setPriority(-1);
    setState(-1);
    setSystemVersion(-1);
    setToUser(-1);
    setType(-1);
    setUnitCategorie(-1);
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
            return ticketID();
            break;
        case 1:
            return title();
            break;
        case 2:
            return Global::i()->customers().value(customer());
            break;
        case 3:
            return Global::i()->users().value(toUser());
            break;
        case 4:
            return deadline();
            break;
        case 5:
            return Global::i()->prioritys().value(priority());
            break;
        case 6:
            return Global::i()->ticketTypes().value(type());
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
        if(column == 4)
        {
            QPixmap p(22,22);
            if(state() == 1 || state() == 2)
            {
                if(QDateTime::currentDateTime().daysTo(deadline()) < 0)
                {
                    p = QPixmap(":/icon/icons/alarm-red.png");
                    return p.scaledToHeight(22, Qt::SmoothTransformation);
                }
                else if(QDateTime::currentDateTime().daysTo(deadline()) == 0)
                {
                    p = QPixmap(":/icon/icons/alarm-yellow.png");
                    return p.scaledToHeight(22, Qt::SmoothTransformation);
                }
            }
            p.fill(QColor(250,250,250,0));
            return p;
        }
    }
    else if(role == Qt::FontRole)
    {
        if(!m_viewed || column == 0)
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
    setID(m_record.value("id").toInt());
    setType(m_record.value("type").toInt());
    setFromUser(m_record.value("from_user").toInt());
    setToUser(m_record.value("to_user").toInt());
    setSystemVersion(m_record.value("system_version").toInt());
    setState(m_record.value("state").toInt());
    setPriority(m_record.value("priority").toInt());
    setPercentComplete(m_record.value("percent_complete").toInt());
    setTitle(m_record.value("title").toString());
    setDate(m_record.value("date").toDateTime());
    setProcessedBy(m_record.value("processed_by").toInt());
    setProcessedOn(m_record.value("processed_on").toDateTime());
    setTestedBy(m_record.value("tested_by").toInt());
    setTestedOn(m_record.value("tested_on").toDateTime());
    setUnitCategorie(m_record.value("unit_categorie").toInt());
    setCustomer(m_record.value("customer").toInt());
    setDepartment(m_record.value("department").toInt());
    setDeadline(m_record.value("deadline").toDateTime());
}

Qt::ItemFlags TicketItem::flags()
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

bool TicketItem::viewed() const
{
    return m_viewed;
}

void TicketItem::setViewed(bool viewed, bool updateSql)
{
    if(m_viewed != viewed)
    {
        m_viewed = viewed;
        int v = 0;
        if(m_viewed)
            v = 1;
        if(updateSql)
        {
            QSqlQuery query(QString("UPDATE comments set viewed = %1 where ticket = %2 and to_user = %3")
                            .arg(v).arg(ticketID()).arg(Global::i()->userID()), Global::i()->db());
            query.exec();
        }
    }
}

int TicketItem::ticketID() const
{
    return m_ID;
}

void TicketItem::setID(int ID)
{
    m_ID = ID;
}

int TicketItem::type() const
{
    return m_type;
}

void TicketItem::setType(int type)
{
    m_type = type;
}

int TicketItem::fromUser() const
{
    return m_fromUser;
}

void TicketItem::setFromUser(int fromUser)
{
    m_fromUser = fromUser;
}

int TicketItem::toUser() const
{
    return m_toUser;
}

void TicketItem::setToUser(int toUser)
{
    m_toUser = toUser;
}

int TicketItem::systemVersion() const
{
    return m_systemVersion;
}

void TicketItem::setSystemVersion(int systemVersion)
{
    m_systemVersion = systemVersion;
}

int TicketItem::state() const
{
    return m_state;
}

void TicketItem::setState(int state)
{
    m_state = state;
}

int TicketItem::priority() const
{
    return m_priority;
}

void TicketItem::setPriority(int priority)
{
    m_priority = priority;
}

int TicketItem::percentComplete() const
{
    return m_percentComplete;
}

void TicketItem::setPercentComplete(int percentComplete)
{
    m_percentComplete = percentComplete;
}

QString TicketItem::title() const
{
    return m_title;
}

void TicketItem::setTitle(const QString &title)
{
    m_title = title;
}

QDateTime TicketItem::date() const
{
    return m_date;
}

void TicketItem::setDate(const QDateTime &date)
{
    m_date = date;
}

int TicketItem::processedBy() const
{
    return m_processedBy;
}

void TicketItem::setProcessedBy(int processedBy)
{
    m_processedBy = processedBy;
}

QDateTime TicketItem::processedOn() const
{
    return m_processedOn;
}

void TicketItem::setProcessedOn(const QDateTime &processedOn)
{
    m_processedOn = processedOn;
}

int TicketItem::testedBy() const
{
    return m_testedBy;
}

void TicketItem::setTestedBy(int testedBy)
{
    m_testedBy = testedBy;
}

QDateTime TicketItem::testedOn() const
{
    return m_testedOn;
}

void TicketItem::setTestedOn(const QDateTime &testedOn)
{
    m_testedOn = testedOn;
}

int TicketItem::unitCategorie() const
{
    return m_unitCategorie;
}

void TicketItem::setUnitCategorie(int unitCategorie)
{
    m_unitCategorie = unitCategorie;
}

int TicketItem::customer() const
{
    return m_customer;
}

void TicketItem::setCustomer(int customer)
{
    m_customer = customer;
}

int TicketItem::department() const
{
    return m_department;
}

void TicketItem::setDepartment(int department)
{
    m_department = department;
}

QDateTime TicketItem::deadline() const
{
    return m_deadline;
}

void TicketItem::setDeadline(const QDateTime &deadline)
{
    m_deadline = deadline;
}

int TicketItem::saveToDB()
{
    if(ticketID() < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO ticket (type, from_user, to_user, system_version, state, priority, percent_complete, unit_categorie, title, "
                      "date, customer, department, deadline) "
                      "VALUES (:type, :from_user, :to_user, :system_version, :state, :priority, :percent_complete, :unit_categorie, :title, "
                      ":date, :customer, :department, :deadline)");
        query.bindValue(":type", type());
        query.bindValue(":from_user", fromUser());
        query.bindValue(":to_user", toUser());
        query.bindValue(":system_version", systemVersion());
        query.bindValue(":state", 1);
        query.bindValue(":priority", priority());
        query.bindValue(":percent_complete", percentComplete());
        query.bindValue(":unit_categorie", unitCategorie());
        query.bindValue(":title", title());
        query.bindValue(":date", date());
        query.bindValue(":customer", customer());
        query.bindValue(":department", department());
        query.bindValue(":deadline", deadline());

        if(!query.exec())
            qDebug() << query.lastError().text();

        return query.lastInsertId().toInt();
    }
    else
    {
        QString q = "UPDATE ticket set ";
        q+= "  ticket.type ="+ QString::number(type());
        q+= ", from_user="+ QString::number(fromUser());
        q+= ", to_user="+ QString::number(toUser());
        if(systemVersion() > 0)
            q+= ", system_version="+ QString::number(systemVersion());
        if(state() > 0)
            q+= ", state="+ QString::number(state());
        if(unitCategorie() > 0)
            q+= ", unit_categorie="+ QString::number(unitCategorie());
        if(priority() > 0)
            q+= ", priority="+ QString::number(priority());
        q+= ", title='"+title()+"'";
        if(!date().isNull() && date().isValid())
            q+= ", ticket.date='" + date().toString(Global::i()->dateTimeFormat()) + "'";
        if(customer() > 0)
            q+= ", customer="+ QString::number(customer());
        if(department() > 0)
            q+= ", department="+ QString::number(department());
        if(!deadline().isNull() && deadline().isValid())
            q+= ", deadline='"+ deadline().toString(Global::i()->dateTimeFormat()) +"'";
        q+= ", percent_complete ="+ QString::number(percentComplete());
        if(processedBy() > 0)
        {
            q+= ", processed_by ="+ QString::number(processedBy());
            if(!processedOn().isNull() && processedOn().isValid())
                q+= ", processed_on='" + processedOn().toString(Global::i()->dateTimeFormat()) + "'";
        }
        if(testedBy() > 0)
        {
            q+= ", tested_by ="+ QString::number(testedBy());
            if(!testedOn().isNull() && testedOn().isValid())
                q+= ", tested_on='" + testedOn().toString(Global::i()->dateTimeFormat()) + "'";
        }
        q+= " WHERE id="+ QString::number(ticketID());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            qDebug() << "Query: " << query.lastQuery();
        }
    }
    return ticketID();
}
