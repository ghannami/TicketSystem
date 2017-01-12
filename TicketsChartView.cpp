#include "TicketsChartView.h"
#include "ui_TicketsChartView.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QHorizontalBarSeries>
#include "TicketModel.h"
#include "TicketItem.h"
#include <QMultiMap>
#include "Global.h"
#include <QHBoxLayout>

TicketsChartView::TicketsChartView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketsChartView)
{
    ui->setupUi(this);    

    m_chartView = new QChartView(new QChart());
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(m_chartView);
    setLayout(lay);
}

TicketsChartView::~TicketsChartView()
{
    delete ui;
}

void TicketsChartView::setModel(TicketModel *model)
{
    int c = model->rowCount();
    QMap<int, TicketItem*> userTickets;/// <userID, Ticket>
    for(int i = 0; i < c; i ++)
    {
        TicketItem *item = model->item(model->index(i,0)).get();
        if(item)
            userTickets.insertMulti(item->toUser(), item);
    }
    QListIterator<int>keys(userTickets.uniqueKeys());

    QBarSet *set0 = new QBarSet(Global::i()->stats().value(1));
    set0->setColor(QColor(230,0,0,150));
    QBarSet *set1 = new QBarSet(Global::i()->stats().value(2));
    set1->setColor(QColor(0,0,250,150));

    QStringList categories;

    int total = 0;
    while(keys.hasNext())
    {
        int userID = keys.next();
        auto lst = userTickets.values(userID);
        QListIterator<TicketItem*>it(lst);
        int open = 0;
        int toTest = 0;

        while(it.hasNext())
        {
            auto item = it.next();
            auto stateID = item->record().value("state").toInt();
            if(stateID==1)
                open++;
            else if(stateID == 2)
                toTest++;
            total++;
        }
        categories << Global::i()->users().value(userID);
        *set0 << open;
        *set1 << toTest;
    }

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(set0);
    series->append(set1);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Offene Baustellen");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chart->setAxisY(axisY, series);
    QValueAxis *axisX = new QValueAxis();
    chart->setAxisX(axisX, series);
    axisX->applyNiceNumbers();

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    auto oldChart =  m_chartView->chart();
    m_chartView->setChart(chart);
    delete oldChart;
}
