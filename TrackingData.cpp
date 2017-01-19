#include "TrackingData.h"
#include <QSqlQuery>
#include <QSqlError>
#include "Global.h"
#include <QDebug>

TrackingData::TrackingData(const QSqlRecord &record)
{
    m_id = -1;
    m_systemVersionId = -1;
    m_userId = -1;
    m_typeId = -1;
    m_behaviorId = -1;
    setRecord(record);
}

TrackingData::TrackingData():
    m_id(-1)
{

}

int TrackingData::id() const
{
    return m_id;
}

void TrackingData::setId(int id)
{
    m_id = id;
}

int TrackingData::typeId() const
{
    return m_typeId;
}

void TrackingData::setTypeId(int typeId)
{
    m_typeId = typeId;
}

int TrackingData::behaviorId() const
{
    return m_behaviorId;
}

void TrackingData::setBahaviorId(int bahaviorId)
{
    m_behaviorId = bahaviorId;
}

int TrackingData::systemVersionId() const
{
    return m_systemVersionId;
}

void TrackingData::setSystemVersionId(int systemVersionId)
{
    m_systemVersionId = systemVersionId;
}

QString TrackingData::title() const
{
    return m_title;
}

void TrackingData::setTitle(const QString &title)
{
    m_title = title;
}

QString TrackingData::text() const
{
    return m_text;
}

void TrackingData::setText(const QString &text)
{
    m_text = text;
}

int TrackingData::userId() const
{
    return m_userId;
}

void TrackingData::setUserId(int userId)
{
    m_userId = userId;
}

QDateTime TrackingData::date() const
{
    return m_date;
}

void TrackingData::setDate(const QDateTime &date)
{
    m_date = date;
}

void TrackingData::saveToDB()
{
    if(id() < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO change_tracking (type, text, behavior, version, title, date, user) "
                      "VALUES (:type, :text, :behavior, :version, :title, :date, :user)");
        query.bindValue(":type", typeId());
        query.bindValue(":text", text());
        query.bindValue(":behavior", behaviorId());
        query.bindValue(":version", systemVersionId());
        query.bindValue(":title", title());
        query.bindValue(":date", QDateTime::currentDateTime());
        query.bindValue(":user", Global::i()->userID());


        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            qDebug() << "Query: "<<query.lastQuery();
        }
    }
    else
    {
        QString q = QString("UPDATE change_tracking set type ='%1', text ='%2', behavior ='%3', version ='%4', title ='%5', date ='%6', user ='%7' WHERE id = %8")
                .arg(typeId()).arg(text()).arg(behaviorId()).arg(systemVersionId()).arg(title()).arg(date().toString(Global::i()->dateTimeFormat())).arg(userId()).arg(id());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
}

QSqlRecord TrackingData::record() const
{
    return m_record;
}

void TrackingData::setRecord(const QSqlRecord &record)
{
    m_record = record;
    if(m_record.contains("id"))
        setId(m_record.value("id").toInt());
    if(m_record.contains("type"))
        setTypeId(m_record.value("type").toInt());
    if(m_record.contains("text"))
        setText(m_record.value("text").toString());
    if(m_record.contains("behavior"))
        setBahaviorId(m_record.value("behavior").toInt());
    if(m_record.contains("version"))
        setSystemVersionId(m_record.value("version").toInt());
    if(m_record.contains("title"))
        setTitle(m_record.value("title").toString());
    if(m_record.contains("date"))
        setDate(m_record.value("date").toDateTime());
    if(m_record.contains("user"))
        setUserId(m_record.value("user").toInt());
}

bool TrackingData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant TrackingData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case 0:
            return title();
        case 1:
            return Global::i()->users().value(userId());
            break;
        case 2:
            return date();
            break;
        }
    }
    return QVariant();
}
