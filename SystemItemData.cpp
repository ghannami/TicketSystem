#include "SystemItemData.h"

SystemItemData::SystemItemData(QSqlRecord record):
    m_record(record)
{
    setName(m_record.value("name").toString());
    setVersion(m_record.value("version").toString());
    setID(m_record.value("id").toInt());
    setSystemVersionId(m_record.value("system_version").toInt());
    setDate(m_record.value("date").toDate());
}

bool SystemItemData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant SystemItemData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case 0:
            return name();
        case 1:
            return version();
        case 2:
            return date();
        }
    }
    return QVariant();
}

int SystemItemData::systemVersionId() const
{
    return m_systemVersionId;
}

void SystemItemData::setSystemVersionId(int systemVersionId)
{
    m_systemVersionId = systemVersionId;
}

QDate SystemItemData::date() const
{
    return m_date;
}

void SystemItemData::setDate(const QDate &date)
{
    m_date = date;
}

QString SystemItemData::version() const
{
    return m_version;
}

void SystemItemData::setVersion(const QString &version)
{
    m_version = version;
}

int SystemItemData::ID() const
{
    return m_ID;
}

void SystemItemData::setID(int ID)
{
    m_ID = ID;
}

QString SystemItemData::name() const
{
    return m_name;
}

void SystemItemData::setName(const QString &name)
{
    m_name = name;
}

QSqlRecord SystemItemData::record() const
{
    return m_record;
}

void SystemItemData::setRecord(const QSqlRecord &record)
{
    m_record = record;
}
