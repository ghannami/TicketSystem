#ifndef TICKETDETAILS_H
#define TICKETDETAILS_H

#include <QWidget>

namespace Ui {
class TicketDetails;
}

class TicketDetails : public QWidget
{
    Q_OBJECT

public:
    explicit TicketDetails(QWidget *parent = 0);
    ~TicketDetails();

    int ticketID() const;
    void setTicketID(int ticketID);

protected slots:
    void send();
    void onFieldsChanged();
    void changeTicketState();
    void reopenTicket();

protected:
    void loadComments();

signals:
    void changed();

private:
    Ui::TicketDetails *ui;
    int m_ticketID;
    int m_ticketState;
    int m_fromUserID;
    int m_toUserID;
};

#endif // TICKETDETAILS_H
