#ifndef COMMENTITEM_H
#define COMMENTITEM_H
#include <QDateTime>

class CommentItem
{
public:
    CommentItem();

    int fromUserID() const;
    void setFromUserID(int fromUserID);

    int toUserID() const;
    void setToUserID(int toUserID);

    QDateTime date() const;
    void setDate(const QDateTime &date);

    int ticketID() const;
    void setTicketID(int ticketID);

    int ID() const;
    void setID(int ID);

    QString text() const;
    void setText(const QString &text);

    int viewed() const;
    void setViewed(int viewed);

    void saveToDB();

private:
    int m_ID;
    int m_fromUserID;
    int m_toUserID;
    QDateTime m_date;
    int m_ticketID;
    QString m_text;
    int m_viewed;
};

#endif // COMMENTITEM_H
