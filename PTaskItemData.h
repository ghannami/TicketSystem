#ifndef TASKITEMDATA_H
#define TASKITEMDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>
#include <QDate>

class PTaskItemData : public TreeItemData
{
public:
    enum DateMode{
        WeekDate,
        MonthDate
    };

    PTaskItemData(QSqlRecord record, DateMode dateMode = WeekDate);
    PTaskItemData(DateMode dateMode = WeekDate);
    virtual int columnCount() override;
    virtual Qt::ItemFlags flags(const QModelIndex &index);

    QSqlRecord record() const;

    QDate taskStart() const;
    void setTaskStart(const QDate &taskStart);

    QDate taskEnd() const;
    void setTaskEnd(const QDate &taskEnd);

    QString taskName() const;
    void setTaskName(const QString &taskName);

    int ID() const;
    void setID(int ID);

    void saveToDB();

    int projectId() const;
    void setProjectId(int projectId);

    int order() const;
    void setOrder(int order);

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
    DateMode m_dateMode;
    QDate m_taskStart;
    QDate m_taskEnd;
    QString m_taskName;
    int m_projectId;
    int m_ID;
    int m_order;
};

#endif // TASKITEMDATA_H
