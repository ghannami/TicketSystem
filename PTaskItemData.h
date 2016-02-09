#ifndef TASKITEMDATA_H
#define TASKITEMDATA_H
#include "AbstractTaskData.h"
#include <QSqlRecord>
#include <QDate>

class PTaskItemData : public AbstractTaskData
{
public:

    PTaskItemData(QSqlRecord record, DateMode dateMode = WeekDate);
    PTaskItemData(DateMode dateMode = WeekDate);
    virtual int columnCount() override;
    virtual Qt::ItemFlags flags(const QModelIndex &index);

    virtual void saveToDB();
    virtual void removeFromDB();

    int projectId() const;
    void setProjectId(int projectId);

    int order() const;
    void setOrder(int order);

    QString description() const;
    void setDescription(const QString &description);

    QDate date() const;
    void setDate(const QDate &date);

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QDate m_date;
    int m_projectId;
    int m_order;
    QString m_description;
};

#endif // TASKITEMDATA_H
