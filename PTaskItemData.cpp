#include "PTaskItemData.h"
#include <QDate>
#include <QSqlQuery>
#include "Global.h"
#include <QtCore>
#include <QDebug>
#include <QSqlError>


PTaskItemData::PTaskItemData(QSqlRecord rec, DateMode dateMode):
    AbstractTaskData(rec, dateMode)
{
    setDate(record().value("date").toDate());
    setTaskStart(record().value("task_start").toDate());
    setTaskEnd(record().value("task_end").toDate());
    setTaskName(record().value("name").toString());
    setID(record().value("id").toInt());
    setProjectId(record().value("project_id").toInt());
    setTaskType(record().value("project_task_type").toInt());
    setOrder(record().value("task_order").toInt());
    setPercent(record().value("percent").toInt());
    setDescription(record().value("description").toString());
}

PTaskItemData::PTaskItemData(PTaskItemData::DateMode dateMode)
{

}

Qt::ItemFlags PTaskItemData::flags(const QModelIndex &index)
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool PTaskItemData::setValue(int column, const QVariant &value, int role)
{
    return true;
}

QVariant PTaskItemData::value(int column, int role)
{
    if(role == Qt::DisplayRole)
    {
        if(column == 0)
            return taskName();
        int startWeek = taskStart().weekNumber();
        int endWeek = taskEnd().weekNumber();
        if(column >= startWeek && column <= endWeek)
        {
            return 7;
        }
        else
            return 0;
    }
    return QVariant();
}

QDate PTaskItemData::date() const
{
    return m_date;
}

void PTaskItemData::setDate(const QDate &date)
{
    m_date = date;
}

QString PTaskItemData::description() const
{
    return m_description;
}

void PTaskItemData::setDescription(const QString &description)
{
    m_description = description;
}

int PTaskItemData::order() const
{
    return m_order;
}

void PTaskItemData::setOrder(int order)
{
    m_order = order;
}

int PTaskItemData::projectId() const
{
    return m_projectId;
}

void PTaskItemData::setProjectId(int projectId)
{
    m_projectId = projectId;
}

void PTaskItemData::saveToDB()
{
    if(ID() < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO project_task (name, project_id, date, project_task_type, task_start, task_end, task_order, percent, description) "
                      "VALUES (:name, :project_id, :date, :project_task_type, :task_start, :task_end, :order, :percent, :description)");
        query.bindValue(":name", taskName());
        query.bindValue(":project_id", projectId());
        query.bindValue(":date", date());
        query.bindValue(":project_task_type", taskType());
        query.bindValue(":task_start", taskStart());
        query.bindValue(":task_end", taskEnd());
        query.bindValue(":order", order());
        query.bindValue(":percent", percent());
        query.bindValue(":description", description());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    else
    {
        QString q = QString("UPDATE project_task set name = '%1', project_id = %2, task_start = '%3', task_end = '%4', task_order =%5, percent = %6, description = '%7', date = '%8' WHERE id = %9")
                .arg(taskName()).arg(projectId()).arg(taskStart().toString("yyyy-MM-dd")).arg(taskEnd().toString("yyyy-MM-dd")).arg(order()).arg(percent()).arg(description()).arg(date().toString("yyyy-MM-dd")).arg(ID());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
}

void PTaskItemData::removeFromDB()
{
    if(ID() > 0)
    {
        QString q = QString("DELETE FROM project_task WHERE id = %1").arg(ID());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
}
