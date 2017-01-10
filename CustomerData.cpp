#include "CustomerData.h"
#include <QSqlQuery>
#include "Global.h"
#include <QtCore>
#include <QDebug>
#include <QSqlError>

CustomerData::CustomerData(QSqlRecord record):
    m_record(record)
{
    setName(m_record.value("name").toString());
    setID(m_record.value("id").toInt());
}

CustomerData::CustomerData():
    m_ID(-1)
{

}

bool CustomerData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant CustomerData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case 0:
        {
            return name();
        }
        }
    }
    return QVariant();
}

int CustomerData::ID() const
{
    return m_ID;
}

void CustomerData::setID(int ID)
{
    m_ID = ID;
}

QString CustomerData::name() const
{
    return m_name;
}

void CustomerData::setName(const QString &name)
{
    m_name = name;
}

QSqlRecord CustomerData::record() const
{
    return m_record;
}

void CustomerData::setRecord(const QSqlRecord &record)
{
    m_record = record;
}

void CustomerData::saveToDB()
{
    if(ID() < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO customer (name) "
                      "VALUES (:name)");
        query.bindValue(":name", name());

        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    else
    {
        QString q = QString("UPDATE customer set name = '%1' WHERE id = %2")
                .arg(name()).arg(ID());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    Global::i()->customers(true);
}
