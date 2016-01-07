#ifndef TICKETDETAILS_H
#define TICKETDETAILS_H

#include <QWidget>

namespace Ui {
class TicketDetails;
}

class TicketModel;

class TicketDetails : public QWidget
{
    Q_OBJECT

public:
    explicit TicketDetails(TicketModel *model, QWidget *parent = 0);
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
    void updateTicket();
signals:
    void changed();

private:
    Ui::TicketDetails *ui;
    int m_ticketID;
    int m_ticketState;
    int m_fromUserID;
    int m_toUserID;
    TicketModel *m_model;
    bool m_blockFieldsChange;
};

#endif // TICKETDETAILS_H
