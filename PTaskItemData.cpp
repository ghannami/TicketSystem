#include "PTaskItemData.h"

PTaskItemData::PTaskItemData(QSqlRecord record):
    m_record(record)
{
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
}

int PTaskItemData::columnCount()
{
    return 1;
}

bool PTaskItemData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant PTaskItemData::value(int column, int role)
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
