#ifndef PROJECTITEMDATA_H
#define PROJECTITEMDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>

class ProjectItemData : public TreeItemData
{
public:
    ProjectItemData(const QSqlRecord &record);
    virtual int columnCount() override;

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
};

#endif // PROJECTITEMDATA_H
