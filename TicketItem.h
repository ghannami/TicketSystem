#ifndef TICKETITEM_H
#define TICKETITEM_H
#include <QVariant>
#include <QSqlRecord>

class TicketItem
{
public:
    enum TicketColumns
    {
        TicketIcon      = 0,
        TicketID        = 1,
        TicketTitle     = 2,
        CostumerName    = 3,
        TicketDate      = 4,
        FromUser        = 5,
        ToUser          = 6,
        PriorityName    = 7,
        Categorie       = 8,
        SystemVersion   = 9
    };

    TicketItem();
    ~TicketItem();

    QVariant data(int column, int role) const;
    void setData(int column, QVariant value, int role);

    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);
    Qt::ItemFlags flags();
    int ticketID() const;


    bool viewed() const;
    void setViewed(bool viewed, bool updateSql = false);

    int fromUser() const;
    int toUser() const;
    int processedBy() const;
    int testedBy() const;
private:
    QSqlRecord m_record;
    bool m_viewed;
};

#endif // TICKETITEM_H
