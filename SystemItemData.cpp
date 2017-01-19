#include "SystemItemData.h"
#include <QColor>

SystemItemData::SystemItemData(QSqlRecord record):
    m_record(record)
{
    m_passedCount = 0;
    m_notPassedCount = 0;
    m_unitsCount = -1;
    setName(m_record.value("name").toString());
    setVersion(m_record.value("version").toString());
    setVersionName(m_record.value("version_name").toString());
    setRevision(m_record.value("revision").toString());
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
        {
            QString t = name();
            if(!versionName().isEmpty())
                t += "("+versionName()+")";
            return t;
        }
        case 1:
            return version()+" rev."+revision();
        case 2:
            return date();
        case 3:
            return passedCount();
        case 4:
            return notPassedCount();
        case 5:
            return unitsCount() - passedCount() - notPassedCount();
        }
    }
    else if(role == Qt::BackgroundColorRole)
    {
        if(passedCount() == unitsCount() && unitsCount() > 0)    // bestanden
        {
            return QColor(0,255,0, 50);
        }
        return QColor(230,0,0, 50);
    }
    return QVariant();
}

QString SystemItemData::revision() const
{
    return m_revision;
}

void SystemItemData::setRevision(const QString &revision)
{
    m_revision = revision;
}

QString SystemItemData::versionName() const
{
    return m_versionName;
}

void SystemItemData::setVersionName(const QString &versionName)
{
    m_versionName = versionName;
}

int SystemItemData::unitsCount() const
{
    return m_unitsCount;
}

void SystemItemData::setUnitsCount(int unitsCount)
{
    m_unitsCount = unitsCount;
}

int SystemItemData::notPassedCount() const
{
    return m_notPassedCount;
}

void SystemItemData::setNotPassedCount(int notPassedCount)
{
    m_notPassedCount = notPassedCount;
}

int SystemItemData::passedCount() const
{
    return m_passedCount;
}

void SystemItemData::setPassedCount(int passedCount)
{
    m_passedCount = passedCount;
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
