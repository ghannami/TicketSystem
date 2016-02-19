#ifndef ABSTRACTTASKDATA_H
#define ABSTRACTTASKDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>
#include <QDate>

class AbstractTaskData : public TreeItemData
{
public:
    enum DateMode{
        WeekDate,
        MonthDate
    };
    AbstractTaskData(QSqlRecord record, DateMode dateMode = WeekDate);
    AbstractTaskData(DateMode dateMode = WeekDate);

    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);

    DateMode dateMode() const;
    void setDateMode(const DateMode &dateMode);

    QDate taskStart() const;
    void setTaskStart(const QDate &taskStart);

    QDate taskEnd() const;
    void setTaskEnd(const QDate &taskEnd);

    QString taskName() const;
    void setTaskName(const QString &taskName);

    int taskType() const;
    void setTaskType(int taskType);

    int ID() const;
    void setID(int ID);

    int percent() const;
    void setPercent(int percent);

    virtual void saveToDB();
    virtual void removeFromDB();


private:
    QSqlRecord m_record;
    DateMode m_dateMode;
    QDate m_taskStart;
    QDate m_taskEnd;
    QString m_taskName;
    int m_taskType;
    int m_ID;
    int m_percent;
};

#endif // ABSTRACTTASKDATA_H
