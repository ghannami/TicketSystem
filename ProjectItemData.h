#ifndef PROJECTITEMDATA_H
#define PROJECTITEMDATA_H
#include "AbstractTaskData.h"
#include <QSqlRecord>

class ProjectItemData : public AbstractTaskData
{
public:
    ProjectItemData(const QSqlRecord &record);
    ProjectItemData();
    virtual void saveToDB();
protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;
};

#endif // PROJECTITEMDATA_H
