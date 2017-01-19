#include "TicketsWidget.h"
#include "ui_TicketsWidget.h"
#include "TicketModel.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QTimer>
#include "TicketItem.h"
#include <QSqlQuery>
#include "FilterObject.h"
#include "Global.h"
#include "NewTicket.h"
#include "TicketDetails.h"
#include "TicketsChartView.h"

TicketsWidget::TicketsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketsWidget)
{
    ui->setupUi(this);
    m_model = new TicketModel();
    ui->ticketView->setModel(m_model);
    ui->ticketView->horizontalHeader()->setVisible(true);
    ui->ticketView->verticalHeader()->setVisible(false);
    ui->ticketView->setAutoScroll(true);

    m_chartView = new TicketsChartView();
    QVBoxLayout *chartLay = new QVBoxLayout();
    chartLay->addWidget(m_chartView);
    ui->statisticWidget->setLayout(chartLay);

    m_model->setHeaders(QStringList()<<"TicketID"<<"Titel"<<"Kunde"<<"Zugewiesen an"<<"Deadline"<<"Priorität"<<"");

    refreshModel();
    int i = 0;
    ui->ticketView->setColumnWidth(i++, 60);
    ui->ticketView->setColumnWidth(i++, 500);
    ui->ticketView->setColumnWidth(i++, 250);
    ui->ticketView->setColumnWidth(i++, 100);
    ui->ticketView->setColumnWidth(i++, 150);
    ui->ticketView->setColumnWidth(i++, 100);

    ui->ticketView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ticketView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ticketView->horizontalHeader()->setStretchLastSection(true);
    ui->ticketView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ticketView->verticalHeader()->setDefaultSectionSize(20);
    setupeBoxes();

    m_ticketDetails = new TicketDetails(m_model);
    QVBoxLayout *lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(m_ticketDetails);
    ui->ticketDetailsWidget->setLayout(lay);
    m_ticketDetails->setTicketItem(nullptr);

    connect(ui->stateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->priorityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->toUserBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->unitCategorieBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->systemBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->customerBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->department, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));

    connect(ui->ticketView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onSelectionChanged(QModelIndex, QModelIndex)));
    connect(m_ticketDetails, SIGNAL(changed()), this, SLOT(updateModel()));
    connect(ui->updateFilters, SIGNAL(clicked(bool)), this, SLOT(setupeBoxes()));

    QPixmap p(":/icon/icons/clipboard.png");
    ui->logoLabel->setPixmap(p.scaledToHeight(84, Qt::SmoothTransformation));
}

TicketsWidget::~TicketsWidget()
{
    delete ui;
}

void TicketsWidget::updateModel()
{
    m_model->updateModel();
}

void TicketsWidget::refreshModel()
{
    QModelIndex index = ui->ticketView->selectionModel()->currentIndex();
    int ticketID =0;
    if(m_model->item(index))
        ticketID = m_model->item(index)->ticketID();
    m_model->updateModel();
    if(m_model->index(ticketID).isValid())
        ui->ticketView->setCurrentIndex(m_model->index(ticketID));

    m_chartView->setModel(m_model);

    QTimer::singleShot(60000, this, SLOT(refreshModel()));
}

void TicketsWidget::onFilterChanged()
{
    std::shared_ptr<FilterObject> filter = m_model->filterObject();
    filter->setPriorityID(ui->priorityBox->itemData(ui->priorityBox->currentIndex()).toInt());
    filter->setStateID(ui->stateBox->itemData(ui->stateBox->currentIndex()).toInt());
    filter->setToUserID(ui->toUserBox->itemData(ui->toUserBox->currentIndex()).toInt());
    filter->setTypeID(ui->typeBox->itemData(ui->typeBox->currentIndex()).toInt());
    filter->setUnitCategorieID(ui->unitCategorieBox->itemData(ui->unitCategorieBox->currentIndex()).toInt());
    filter->setSystemVersionID(ui->systemBox->itemData(ui->systemBox->currentIndex()).toInt());
    filter->setCustomerID(ui->customerBox->itemData(ui->customerBox->currentIndex()).toInt());
    filter->setDepartmentID(ui->department->itemData(ui->department->currentIndex()).toInt());

    ui->ticketView->clearSelection();
    m_ticketDetails->setTicketItem(nullptr);
    m_model->updateModel();
    if(m_model->index(0,0).isValid())
    {
        ui->ticketView->selectionModel()->setCurrentIndex(m_model->index(0,0), QItemSelectionModel::SelectCurrent);
    }

    m_chartView->setModel(m_model);

}

void TicketsWidget::newTicket()
{
    NewTicket ticket;
    ticket.exec();
    ui->ticketView->clearSelection();
    updateModel();
}

void TicketsWidget::onSelectionChanged(const QModelIndex & current, const QModelIndex & previous)
{
    if(m_model->item(current))
    {
        m_ticketDetails->setTicketItem(m_model->item(current).get());
    }
}

void TicketsWidget::setupeBoxes()
{
    Global::i()->loadDBValues();

    ui->stateBox->clear();
    ui->typeBox->clear();
    ui->toUserBox->clear();
    ui->priorityBox->clear();
    ui->unitCategorieBox->clear();
    ui->systemBox->clear();
    ui->customerBox->clear();
    ui->department->clear();

    ui->stateBox->insertItem(0,tr("Alle"),0);
    ui->typeBox->insertItem(0,tr("Alle"),0);
    ui->toUserBox->insertItem(0,tr("Alle"),0);
    ui->priorityBox->insertItem(0,tr("Alle"),0);
    ui->unitCategorieBox->insertItem(0,tr("Alle"),0);
    ui->systemBox->insertItem(0,tr("Alle"),0);
    ui->customerBox->insertItem(0,tr("Alle"),0);
    ui->department->insertItem(0,tr("Alle"),0);

    int i=1;
    QMapIterator<int, QString>mip(Global::i()->users());
    while(mip.hasNext())
    {
        mip.next();
        ui->toUserBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->prioritys());
    while(mip.hasNext())
    {
        mip.next();
        ui->priorityBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->stats());
    while(mip.hasNext())
    {
        mip.next();
        ui->stateBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->ticketTypes());
    while(mip.hasNext())
    {
        mip.next();
        ui->typeBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->unitCategories());
    while(mip.hasNext())
    {
        mip.next();
        ui->unitCategorieBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->systemVersions());
    while(mip.hasNext())
    {
        mip.next();
        ui->systemBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->customers());
    while(mip.hasNext())
    {
        mip.next();
        ui->customerBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->departments());
    while(mip.hasNext())
    {
        mip.next();
        ui->department->insertItem(i++, mip.value(),mip.key());
    }
}
