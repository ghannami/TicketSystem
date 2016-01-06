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
        StateName       = 3,
        TicketDate      = 4,
        FromUser        = 5,
        ToUser          = 6,
        PriorityName    = 7,
        Categorie       = 8,
        Project         = 9
    };

    TicketItem();
    QVariant data(int column, int role) const;
    void setData(int column, QVariant value, int role);

    QSqlRecord record() const;
    void setRecord(const QSqlRecord &record);
    Qt::ItemFlags flags();
    int ticketID() const;

private:
    QSqlRecord m_record;
};

#endif // TICKETITEM_H
