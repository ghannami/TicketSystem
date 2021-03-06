#ifndef TICKETSWIDGET_H
#define TICKETSWIDGET_H

#include <QWidget>

#include <QSqlDatabase>
#include <QItemSelection>

class TicketModel;
class TicketDetails;
class TicketsChartView;

namespace Ui {
class TicketsWidget;
}

class TicketsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TicketsWidget(QWidget *parent = 0);
    ~TicketsWidget();

protected slots:
    void updateModel();
    void refreshModel();
    void onFilterChanged();
    void newTicket();
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void setupeBoxes();

private:
    Ui::TicketsWidget *ui;
    TicketModel *m_model;
    TicketDetails *m_ticketDetails;
    TicketsChartView *m_chartView;
};

#endif // TICKETSWIDGET_H
