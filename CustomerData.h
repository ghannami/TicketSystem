/*!
 *##########################################################
 * \class   CustomerData
 * \package
 * \author  Aziz Ghannami
 * \date    10.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef CUSTOMERDATA_H
#define CUSTOMERDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>

class CustomerData : public TreeItemData
{
public:
    CustomerData(QSqlRecord record);
    CustomerData();
    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);

    QString name() const;
    void setName(const QString &name);

    int ID() const;
    void setID(int ID);

    virtual void saveToDB();

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
    QString m_name;
    int m_ID;
};

#endif // CUSTOMERDATA_H
