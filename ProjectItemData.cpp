#include "ProjectItemData.h"

ProjectItemData::ProjectItemData(const QSqlRecord &record):
    m_record(record)
{
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

int ProjectItemData::columnCount()
{
    return 1;
}

bool ProjectItemData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant ProjectItemData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case 0:
            return m_record.value("name");
        }
    }
    return QVariant();
}
