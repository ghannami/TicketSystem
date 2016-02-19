#ifndef SYSTEMUNITCATDATA_H
#define SYSTEMUNITCATDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>


class SystemUnitCatData : public TreeItemData
{
public:
    SystemUnitCatData(const QSqlRecord &record);
    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
    int m_id;
    QString m_name;
};

#endif // SYSTEMUNITCATDATA_H
