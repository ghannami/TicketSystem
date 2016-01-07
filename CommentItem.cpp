#include "CommentItem.h"
#include <QSqlQuery>
#include "Global.h"
#include <QtCore>

CommentItem::CommentItem()
{
    m_ID = -1;
    m_viewed = 0;
}

int CommentItem::fromUserID() const
{
    return m_fromUserID;
}

void CommentItem::setFromUserID(int fromUserID)
{
    m_fromUserID = fromUserID;
}

int CommentItem::toUserID() const
{
    return m_toUserID;
}

void CommentItem::setToUserID(int toUserID)
{
    m_toUserID = toUserID;
}

QDateTime CommentItem::date() const
{
    return m_date;
}

void CommentItem::setDate(const QDateTime &date)
{
    m_date = date;
}

int CommentItem::ticketID() const
{
    return m_ticketID;
}

void CommentItem::setTicketID(int ticketID)
{
    m_ticketID = ticketID;
}

int CommentItem::ID() const
{
    return m_ID;
}

void CommentItem::setID(int ID)
{
    m_ID = ID;
}

QString CommentItem::text() const
{
    return m_text;
}

void CommentItem::setText(const QString &text)
{
    m_text = text;
}

int CommentItem::viewed() const
{
    return m_viewed;
}

void CommentItem::setViewed(int viewed)
{
    m_viewed = viewed;
}

void CommentItem::saveToDB()
{
    if(m_ID < 0)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("INSERT INTO comments (date, from_user, to_user, ticket, text, viewed) "
                      "VALUES (:date, :from_user, :to_user, :ticket, :text, :viewed)");
        query.bindValue(":date", date());
        query.bindValue(":from_user", fromUserID());
        query.bindValue(":to_user", toUserID());
        query.bindValue(":ticket", ticketID());
        query.bindValue(":text", text());
        query.bindValue(":viewed", viewed());
        query.exec();
    }
    else
    {
        QSqlQuery query(QString("UPDATE comments set date = %1, from_user = %2, to_user = %3, ticket = %4, text = '%5', viewed = %6")
                        .arg(date().toString()).arg(fromUserID()).arg(toUserID()).arg(ticketID()).arg(text()).arg(viewed())
                        , Global::i()->db());
        query.exec();
    }
}

