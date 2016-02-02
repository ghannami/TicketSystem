#include "PTaskItemData.h"
#include <QDate>
#include <QSqlQuery>
#include "Global.h"
#include <QtCore>
#include <QDebug>
#include <QSqlError>

PTaskItemData::PTaskItemData(QSqlRecord record, DateMode dateMode):
    m_record(record), m_dateMode(dateMode)
{
    setTaskStart(m_record.value("task_start").toDate());
    setTaskEnd(m_record.value("task_end").toDate());
    setTaskName(m_record.value("name").toString());
    setID(m_record.value("id").toInt());
    setProjectId(m_record.value("project_id").toInt());
    setOrder(m_record.value("task_order").toInt());
}

PTaskItemData::PTaskItemData(PTaskItemData::DateMode dateMode):
    m_dateMode(dateMode), m_ID(-1)
{

}

int PTaskItemData::columnCount()
{
    return 1;
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
            return m_taskName;
        int startWeek = m_taskStart.weekNumber();
        int endWeek = m_taskEnd.weekNumber();
        if(column >= startWeek && column <= endWeek)
        {
            return 7;
        }
        else
            return 0;
    }
    return QVariant();
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

int PTaskItemData::ID() const
{
    return m_ID;
}

void PTaskItemData::setID(int ID)
{
    m_ID = ID;
}

void PTaskItemData::saveToDB()
{
    if(m_ID < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO project_task (name, project_id, task_start, task_end, order) "
                      "VALUES (:name, :project_id, :task_start, :task_end, :order)");
        query.bindValue(":name", taskName());
        query.bindValue(":project_id", projectId());
        query.bindValue(":task_start", taskStart());
        query.bindValue(":task_end", taskEnd());
        query.bindValue(":order", order());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    else
    {
        QString q = QString("UPDATE project_task set name = '%1', project_id = %2, task_start = '%3', task_end = '%4', project_task.order =%5 WHERE id = %6")
                .arg(taskName()).arg(projectId()).arg(taskStart().toString("yyyy-MM-dd")).arg(taskEnd().toString("yyyy-MM-dd")).arg(order()).arg(ID());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
}

QString PTaskItemData::taskName() const
{
    return m_taskName;
}

void PTaskItemData::setTaskName(const QString &taskName)
{
    m_taskName = taskName;
}

QDate PTaskItemData::taskEnd() const
{
    return m_taskEnd;
}

void PTaskItemData::setTaskEnd(const QDate &taskEnd)
{
    m_taskEnd = taskEnd;
}

QDate PTaskItemData::taskStart() const
{
    return m_taskStart;
}

void PTaskItemData::setTaskStart(const QDate &taskStart)
{
    m_taskStart = taskStart;
}

QSqlRecord PTaskItemData::record() const
{
    return m_record;
}
