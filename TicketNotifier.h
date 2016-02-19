#ifndef TICKETNOTIFIER_H
#define TICKETNOTIFIER_H
#include <QTimer>
#include <memory>

class TicketNotifier : public QObject
{
    Q_OBJECT

private:
    TicketNotifier(QObject *parent = 0);

public:
    static TicketNotifier *i();
    void setLastTicketID(int lastTicketID);

public slots:
    void loadLastTicket();
    void loadUnreadTickets();

signals:
    void showMessage(QString title, QString msg, int ms);

private:
    static TicketNotifier *m_instance;
    std::shared_ptr<QTimer> m_lastTicketTimer;
    std::shared_ptr<QTimer> m_unreadTicketTimer;
    int m_lastTicketID;
    bool m_firstStart;
};

#endif // TICKETNOTIFIER_H
