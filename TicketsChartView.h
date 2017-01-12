/*!
 *##########################################################
 * \class   TicketsChartView
 * \package
 * \author  Aziz Ghannami
 * \date    12.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef TICKETSCHARTVIEW_H
#define TICKETSCHARTVIEW_H

#include <QWidget>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class TicketModel;

namespace Ui {
class TicketsChartView;
}

class TicketsChartView : public QWidget
{
    Q_OBJECT

public:
    explicit TicketsChartView(QWidget *parent = 0);
    ~TicketsChartView();
    void setModel(TicketModel *model);

private:
    Ui::TicketsChartView *ui;
    QHorizontalBarSeries *m_series;
    QChart *m_chart;
    QChartView *m_chartView;
};

#endif // TICKETSCHARTVIEW_H
