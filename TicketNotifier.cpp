#include "TicketNotifier.h"
#include <QDebug>
#include <QSqlError>
#include <QTimer>
#include <QSqlQuery>
#include "Global.h"

TicketNotifier *TicketNotifier::m_instance = 0;

TicketNotifier::TicketNotifier(QObject *parent):
    QObject(parent)
{
    m_firstStart = true;
    m_lastTicketTimer = std::shared_ptr<QTimer>(new QTimer());
    connect(m_lastTicketTimer.get(), SIGNAL(timeout()), this, SLOT(loadLastTicket()));
    m_lastTicketTimer->start(300000);

    m_unreadTicketTimer = std::shared_ptr<QTimer>(new QTimer());
    connect(m_unreadTicketTimer.get(), SIGNAL(timeout()), this, SLOT(loadUnreadTickets()));
    m_unreadTicketTimer->start(3600000);
}

TicketNotifier *TicketNotifier::i()
{
    if(m_instance == 0)
        m_instance = new TicketNotifier();
    return m_instance;
}

void TicketNotifier::loadLastTicket()
{
    QSqlQuery query("SELECT id  FROM ticketsystem.ticket order by id desc LIMIT 1;", Global::i()->db());
    if(query.next())
    {
        setLastTicketID(query.value("id").toInt());
    }
}

void TicketNotifier::loadUnreadTickets()
{
    QSqlQuery unredQuery(QString("select id, ticket, max(viewed) as viewed from comments where to_user = %1 and viewed = 0 group by ticket")
                         .arg(Global::i()->userID()), Global::i()->db());
    int unread = 0;
    if(unredQuery.size() >= 0)
        unread = unredQuery.size();
    if(unread == 0)
        return;

    QString msg;
    if(unread == 1)
    msg = QString("Du hast %1 ungelesene Nachricht.\nHier klicken um zu öffnen").arg(unread);
    else
        msg = QString("Du hast %1 ungelesene Nachrichten.\nHier klicken um zu öffnen").arg(unread);

    emit showMessage(tr("TicketSystem"), msg, 60000);

}

void TicketNotifier::setLastTicketID(int lastTicketID)
{
    if(lastTicketID > m_lastTicketID && !m_firstStart)
        emit showMessage(tr("TicketSystem"),
                         tr("Es gibt ein neues Ticket für dich.\nTicketID: %1\nHier klicken um zu öffnen").arg(m_lastTicketID), 60000);
    m_lastTicketID = lastTicketID;

    m_firstStart = false;
}
