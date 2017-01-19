#ifndef TICKETDETAILS_H
#define TICKETDETAILS_H

#include <QWidget>

namespace Ui {
class TicketDetails;
}

class TicketModel;
class TicketItem;

class TicketDetails : public QWidget
{
    Q_OBJECT

public:
    explicit TicketDetails(TicketModel *model, QWidget *parent = 0);
    ~TicketDetails();

    TicketItem *ticketItem() const;
    void setTicketItem(TicketItem *ticketItem);

protected slots:
    void send();
    void onFieldsChanged();
    void changeTicketState();
    void reopenTicket();
    void onSave();

protected:
    void loadComments();
    void updateTicket();
signals:
    void changed();

private:
    Ui::TicketDetails *ui;
    TicketModel *m_model;
    bool m_blockFieldsChange;
    TicketItem *m_ticketItem;
};

#endif // TICKETDETAILS_H
