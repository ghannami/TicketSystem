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

    m_model->setHeaders(QStringList()<<""<<"TicketID"<<"Titel"<<"Status"<<"Erstellt am"
                        <<"Erstellt von"<<"Zugewiesen an"<<"Priorität"<<"Modul"<<"Projekt"<<"");

    refreshModel();

    ui->ticketView->setColumnWidth(TicketItem::TicketIcon, 32);
    ui->ticketView->setColumnWidth(TicketItem::TicketID, 60);
    ui->ticketView->setColumnWidth(TicketItem::TicketTitle, 600);
    ui->ticketView->setColumnWidth(TicketItem::StateName, 100);
    ui->ticketView->setColumnWidth(TicketItem::TicketDate, 100);
    ui->ticketView->setColumnWidth(TicketItem::FromUser, 100);
    ui->ticketView->setColumnWidth(TicketItem::ToUser, 100);
    ui->ticketView->setColumnWidth(TicketItem::PriorityName, 100);
    ui->ticketView->setColumnWidth(TicketItem::Categorie, 100);
    ui->ticketView->setColumnWidth(TicketItem::SystemVersion, 100);

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
    m_ticketDetails->setTicketID(0);

    connect(ui->stateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->priorityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->toUserBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->unitCategorieBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->systemBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));

    connect(ui->ticketView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onSelectionChanged(QModelIndex, QModelIndex)));
    connect(m_ticketDetails, SIGNAL(changed()), this, SLOT(updateModel()));
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

    ui->ticketView->clearSelection();
    m_ticketDetails->setTicketID(0);
    m_model->updateModel();
    if(m_model->index(0,0).isValid())
    {
        ui->ticketView->selectionModel()->setCurrentIndex(m_model->index(0,0), QItemSelectionModel::SelectCurrent);
    }
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
        m_ticketDetails->setTicketID(m_model->item(current)->ticketID());
    }
}

void TicketsWidget::setupeBoxes()
{
    ui->stateBox->clear();
    ui->typeBox->clear();
    ui->toUserBox->clear();
    ui->priorityBox->clear();
    ui->unitCategorieBox->clear();
    ui->systemBox->clear();

    ui->stateBox->insertItem(0,tr("Alle"),0);
    ui->typeBox->insertItem(0,tr("Alle"),0);
    ui->toUserBox->insertItem(0,tr("Alle"),0);
    ui->priorityBox->insertItem(0,tr("Alle"),0);
    ui->unitCategorieBox->insertItem(0,tr("Alle"),0);
    ui->systemBox->insertItem(0,tr("Alle"),0);

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
}
