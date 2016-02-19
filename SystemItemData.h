#ifndef SYSTEMITEMDATA_H
#define SYSTEMITEMDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>
#include <QDate>

class SystemItemData : public TreeItemData
{
public:
    SystemItemData(QSqlRecord record);
    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);

    QString name() const;
    void setName(const QString &name);

    int ID() const;
    void setID(int ID);

    QString version() const;
    void setVersion(const QString &version);

    QDate date() const;
    void setDate(const QDate &date);

    int systemVersionId() const;
    void setSystemVersionId(int systemVersionId);

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
    QString m_name;
    QString m_version;
    QDate m_date;
    int m_ID;
    int m_systemVersionId;
};

#endif // SYSTEMITEMDATA_H
