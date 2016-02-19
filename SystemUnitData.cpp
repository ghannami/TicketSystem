#include "SystemUnitData.h"
#include <QColor>
#include <QSqlQuery>
#include "Global.h"
#include <QSqlError>
#include <QDebug>
#include "Global.h"

SystemUnitData::SystemUnitData(const QSqlRecord &record):
    m_record(record)
{
    m_state = 0;
    setRecord(m_record);
}

SystemUnitData::SystemUnitData():
    m_id(-1)
{
    m_state = 0;
}

bool SystemUnitData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant SystemUnitData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        switch(column)
        {
        case 0:
            return title();
        case 1:
            return Global::i()->users().value(userId());
        case 2:
            return date();
        }
    }
    else if(role == Qt::BackgroundColorRole)
    {
        if(state() == 1)    // bestanden
        {
            return QColor(0,255,0, 50);
        }
        else if(state() == 2) // nicht bestanden
        {
            return QColor(230,0,0, 50);
        }
    }
    return QVariant();
}

void SystemUnitData::setRecord(const QSqlRecord &record)
{
    m_record = record;
    if(m_record.contains("id"))
        setId(m_record.value("id").toInt());
    if(m_record.contains("unit_test_id"))
        setUnitTestId(m_record.value("unit_test_id").toInt());
    if(m_record.contains("position"))
        setPosition(m_record.value("position").toInt());
    if(m_record.contains("test_state"))
        setState(m_record.value("test_state").toInt());
    if(m_record.contains("system"))
        setSystemId(m_record.value("system").toInt());
    if(m_record.contains("user"))
        setUserId(m_record.value("user").toInt());
    if(m_record.contains("categorie"))
        setCategorieId(m_record.value("categorie").toInt());
    if(m_record.contains("version"))
        setVersionId(m_record.value("version").toInt());
    if(m_record.contains("system_version"))
        setSystemVersionId(m_record.value("system_version").toInt());
    if(m_record.contains("title"))
        setTitle(m_record.value("title").toString());
    if(m_record.contains("date"))
        setDate(m_record.value("date").toDate());
}

int SystemUnitData::unitTestId() const
{
    return m_unitTestId;
}

void SystemUnitData::setUnitTestId(int unitTestId)
{
    m_unitTestId = unitTestId;
}

int SystemUnitData::systemVersionId() const
{
    return m_systemVersionId;
}

void SystemUnitData::setSystemVersionId(int systemVersionId)
{
    m_systemVersionId = systemVersionId;
}

void SystemUnitData::saveToDB()
{
    if(id() < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO system_unit (system, title, categorie) "
                      "VALUES (:system, :title, :categorie)");
        query.bindValue(":system", systemId());
        query.bindValue(":title", title());
        query.bindValue(":categorie", categorieId());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    else
    {
        QString q = QString("UPDATE system_unit set title = '%1' WHERE id = %2")
                .arg(title()).arg(id());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();

        if(unitTestId() < 0)
        {
            QSqlQuery query(Global::i()->db());
            query.prepare("INSERT INTO system_unit_test (system_version, system_unit, user, test_state, date) "
                          "VALUES (:system_version, :system_unit, :user, :test_state, :date)");
            query.bindValue(":system_version", systemVersionId());
            query.bindValue(":system_unit", id());
            query.bindValue(":user", userId());
            query.bindValue(":test_state", state());
            query.bindValue(":date", date());

            if(!query.exec())
                qDebug() << query.lastError().text();
        }
        else
        {
            QString q = QString("UPDATE system_unit set user = '%1', test_state = '%2', date='%3' WHERE id = %4")
                    .arg(userId()).arg(state()).arg(date().toString("yyyy-MM-dd")).arg(id());
            QSqlQuery query(q , Global::i()->db());
            if(!query.exec())
                qDebug() << query.lastError().text();

        }
    }
}

QSqlRecord SystemUnitData::record() const
{
    return m_record;
}

QDate SystemUnitData::date() const
{
    return m_date;
}

void SystemUnitData::setDate(const QDate &date)
{
    m_date = date;
}

int SystemUnitData::state() const
{
    return m_state;
}

void SystemUnitData::setState(int state)
{
    m_state = state;
}

int SystemUnitData::userId() const
{
    return m_userId;
}

void SystemUnitData::setUserId(int userId)
{
    m_userId = userId;
}

int SystemUnitData::position() const
{
    return m_position;
}

void SystemUnitData::setPosition(int position)
{
    m_position = position;
}

QString SystemUnitData::title() const
{
    return m_title;
}

void SystemUnitData::setTitle(const QString &title)
{
    m_title = title;
}

int SystemUnitData::categorieId() const
{
    return m_categorieId;
}

void SystemUnitData::setCategorieId(int categorieId)
{
    m_categorieId = categorieId;
}

int SystemUnitData::versionId() const
{
    return m_versionId;
}

void SystemUnitData::setVersionId(int value)
{
    m_versionId = value;
}

int SystemUnitData::systemId() const
{
    return m_systemId;
}

void SystemUnitData::setSystemId(int systemId)
{
    m_systemId = systemId;
}

int SystemUnitData::id() const
{
    return m_id;
}

void SystemUnitData::setId(int id)
{
    m_id = id;
}
