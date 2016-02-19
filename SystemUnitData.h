#ifndef SYSTEMUNITDATA_H
#define SYSTEMUNITDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>
#include <QDate>

class SystemUnitData : public TreeItemData
{
public:
    SystemUnitData(const QSqlRecord &record);
    SystemUnitData();

    int id() const;
    void setId(int id);

    int systemId() const;
    void setSystemId(int systemId);

    int versionId() const;
    void setVersionId(int value);

    int categorieId() const;
    void setCategorieId(int categorieId);

    QString title() const;
    void setTitle(const QString &title);

    int position() const;
    void setPosition(int position);

    int userId() const;
    void setUserId(int userId);

    int state() const;
    void setState(int state);

    QDate date() const;
    void setDate(const QDate &date);

    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);

    int systemVersionId() const;
    void setSystemVersionId(int systemVersionId);

    void saveToDB();

    int unitTestId() const;
    void setUnitTestId(int unitTestId);


protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
    int m_id;
    int m_unitTestId;
    int m_systemId;
    int m_versionId;
    int m_systemVersionId;
    int m_categorieId;
    QString m_title;
    int m_position;
    int m_userId;
    int m_state;
    QDate m_date;
};

#endif // SYSTEMUNITDATA_H
