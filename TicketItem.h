#ifndef TICKETITEM_H
#define TICKETITEM_H
#include <QVariant>
#include <QSqlRecord>
#include <QDateTime>

class TicketItem
{
public:
    TicketItem(const QSqlRecord &record);
    TicketItem();
    ~TicketItem();

    QVariant data(int column, int role) const;
    void setData(int column, QVariant value, int role);

    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);
    Qt::ItemFlags flags();

    bool viewed() const;
    void setViewed(bool viewed, bool updateSql = false);

    int ticketID() const;
    void setID(int ticketID);

    int type() const;
    void setType(int type);

    int fromUser() const;
    void setFromUser(int fromUser);

    int toUser() const;
    void setToUser(int toUser);

    int systemVersion() const;
    void setSystemVersion(int systemVersion);

    int state() const;
    void setState(int state);

    int priority() const;
    void setPriority(int priority);

    int percentComplete() const;
    void setPercentComplete(int percentComplete);

    QString title() const;
    void setTitle(const QString &title);

    QDateTime date() const;
    void setDate(const QDateTime &date);

    int processedBy() const;
    void setProcessedBy(int processedBy);

    QDateTime processedOn() const;
    void setProcessedOn(const QDateTime &processedOn);

    int testedBy() const;
    void setTestedBy(int testedBy);

    QDateTime testedOn() const;
    void setTestedOn(const QDateTime &testedOn);

    int unitCategorie() const;
    void setUnitCategorie(int unitCategorie);

    int customer() const;
    void setCustomer(int customer);

    int department() const;
    void setDepartment(int department);

    QDateTime deadline() const;
    void setDeadline(const QDateTime &deadline);

    int saveToDB();

protected:
    void init();
private:
    QSqlRecord m_record;
    bool m_viewed;
    int m_ID;
    int m_type;
    int m_fromUser;
    int m_toUser;
    int m_systemVersion;
    int m_state;
    int m_priority;
    int m_percentComplete;
    QString m_title;
    QDateTime m_date;
    int m_processedBy;
    QDateTime m_processedOn;
    int m_testedBy;
    QDateTime m_testedOn;
    int m_unitCategorie;
    int m_customer;
    int m_department;
    QDateTime m_deadline;
};

#endif // TICKETITEM_H
