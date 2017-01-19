/*!
 *##########################################################
 * \class   TrackingData
 * \package
 * \author  Aziz Ghannami
 * \date    16.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef TRACKINGDATA_H
#define TRACKINGDATA_H
#include "TreeItemData.h"
#include <QSqlRecord>
#include <QDate>


class TrackingData : public TreeItemData
{
public:
    TrackingData(const QSqlRecord &record);
    TrackingData();

    int id() const;
    void setId(int id);

    int typeId() const;
    void setTypeId(int typeId);

    int behaviorId() const;
    void setBahaviorId(int behaviorId);

    int systemVersionId() const;
    void setSystemVersionId(int systemVersionId);

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

    int userId() const;
    void setUserId(int userId);

    QDateTime date() const;
    void setDate(const QDateTime &date);

    void saveToDB();

    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);

protected:
    virtual bool setValue(int column, const QVariant & value, int role) override;
    virtual QVariant value(int column, int role) override;

private:
    QSqlRecord m_record;
    int m_id;
    int m_typeId;
    int m_behaviorId;
    int m_systemVersionId;
    QString m_title;
    QString m_text;
    int m_userId;
    QDateTime m_date;
};

#endif // TRACKINGDATA_H
