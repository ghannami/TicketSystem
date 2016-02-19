#include "ProjectItemData.h"
#include <QSqlQuery>
#include "Global.h"
#include <QtCore>
#include <QDebug>
#include <QSqlError>

ProjectItemData::ProjectItemData(const QSqlRecord &rec):
    AbstractTaskData(rec)
{
    setTaskName(record().value("name").toString());
    setID(record().value("id").toInt());
    setTaskStart(QDate(-1,-1,-1));
    setTaskEnd(QDate(-1,-1,-1));
    setTaskType(10);
    setPercent(-1);
}

ProjectItemData::ProjectItemData():
    AbstractTaskData()
{

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
            return taskName();
        }
    }
    return QVariant();
}

void ProjectItemData::saveToDB()
{
    if(ID() < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO projects (name) "
                      "VALUES (:name)");
        query.bindValue(":name", taskName());

        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    else
    {
        QString q = QString("UPDATE project_task set name = '%1' WHERE id = %2")
                .arg(taskName()).arg(ID());
        QSqlQuery query(q , Global::i()->db());
        if(!query.exec())
            qDebug() << query.lastError().text();
    }
    Global::i()->projects(true);
}
