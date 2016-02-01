#ifndef TASKITEMDATA_H
#define TASKITEMDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>

class PTaskItemData : public TreeItemData
{
public:
    PTaskItemData(QSqlRecord record);
    virtual int columnCount() override;

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
};

#endif // TASKITEMDATA_H
