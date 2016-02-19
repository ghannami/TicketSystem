#include "SystemUnitCatData.h"
#include <QFont>

SystemUnitCatData::SystemUnitCatData(const QSqlRecord &record):
    m_record(record)
{
    setId(m_record.value("id").toInt());
    setName(m_record.value("name").toString());
}

bool SystemUnitCatData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant SystemUnitCatData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        case 0:
            return m_record.value("name");
    }
    else if(role == Qt::FontRole)
    {
        if(column == 0)
        {
            QFont f;
            f.setBold(true);
            return f;
        }
    }
    return QVariant();
}

QString SystemUnitCatData::name() const
{
    return m_name;
}

void SystemUnitCatData::setName(const QString &name)
{
    m_name = name;
}

int SystemUnitCatData::id() const
{
    return m_id;
}

void SystemUnitCatData::setId(int id)
{
    m_id = id;
}
