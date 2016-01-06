#include "MainWindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    showMaximized();
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
    ui->ticketView->setColumnWidth(TicketItem::Project, 100);

    ui->ticketView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ticketView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ticketView->horizontalHeader()->setStretchLastSection(true);
    ui->ticketView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ticketView->verticalHeader()->setDefaultSectionSize(24);
    setupeBoxes();

    m_ticketDetails = new TicketDetails();
    QVBoxLayout *lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(m_ticketDetails);
    ui->ticketDetailsWidget->setLayout(lay);
    m_ticketDetails->setTicketID(0);

    connect(ui->stateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->priorityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->typeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->toUserBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->moduleBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));
    connect(ui->projectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterChanged()));

    connect(ui->actionTicket, SIGNAL(triggered(bool)), this, SLOT(newTicket()));
    connect(ui->ticketView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onSelectionChanged(QModelIndex, QModelIndex)));
    connect(m_ticketDetails, SIGNAL(changed()), this, SLOT(updateModel()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateModel()
{
    m_model->updateModel();
}

void MainWindow::refreshModel()
{
    QModelIndex index = ui->ticketView->selectionModel()->currentIndex();
    int ticketID =0;
    if(m_model->item(index))
        ticketID = m_model->item(index)->ticketID();
    m_model->updateModel();
    if(m_model->index(ticketID).isValid())
        ui->ticketView->setCurrentIndex(m_model->index(ticketID));
    QTimer::singleShot(10000, this, SLOT(refreshModel()));
}

void MainWindow::onFilterChanged()
{
    std::shared_ptr<FilterObject> filter = m_model->filterObject();
    filter->setPriorityID(ui->priorityBox->itemData(ui->priorityBox->currentIndex()).toInt());
    filter->setStateID(ui->stateBox->itemData(ui->stateBox->currentIndex()).toInt());
    filter->setToUserID(ui->toUserBox->itemData(ui->toUserBox->currentIndex()).toInt());
    filter->setTypeID(ui->typeBox->itemData(ui->typeBox->currentIndex()).toInt());
    filter->setCategorieID(ui->moduleBox->itemData(ui->moduleBox->currentIndex()).toInt());
    filter->setProjectID(ui->projectBox->itemData(ui->projectBox->currentIndex()).toInt());

    ui->ticketView->clearSelection();
    m_ticketDetails->setTicketID(0);
    m_model->updateModel();
    if(m_model->index(0,0).isValid())
    {
        ui->ticketView->selectionModel()->setCurrentIndex(m_model->index(0,0), QItemSelectionModel::SelectCurrent);
    }
}

void MainWindow::newTicket()
{
    NewTicket ticket;
    ticket.exec();
    ui->ticketView->clearSelection();
    updateModel();
}

void MainWindow::onSelectionChanged(const QModelIndex & current, const QModelIndex & previous)
{
    if(m_model->item(current))
    {
        m_ticketDetails->setTicketID(m_model->item(current)->ticketID());
    }
}

void MainWindow::setupeBoxes()
{
    ui->stateBox->clear();
    ui->typeBox->clear();
    ui->toUserBox->clear();
    ui->priorityBox->clear();
    QSqlQuery query("SELECT name, id From state order by id asc;", Global::i()->db());
    int i = 0;
    ui->stateBox->insertItem(i++, tr("Alle"),0);
    while(query.next())
    {
        ui->stateBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From tickettype order by id asc;", Global::i()->db());
    i = 0;
    ui->typeBox->insertItem(i++, tr("Alle"),0);
    while(query.next())
    {
        ui->typeBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From user order by id asc;", Global::i()->db());
    i = 0;
    ui->toUserBox->insertItem(i++, tr("Alle"),0);
    while(query.next())
    {
        ui->toUserBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From priority order by id asc;", Global::i()->db());
    i = 0;
    ui->priorityBox->insertItem(i++, tr("Alle"),0);
    while(query.next())
    {
        ui->priorityBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From categories order by id asc;", Global::i()->db());
    i = 0;
    ui->moduleBox->insertItem(i++, tr("Alle"),0);
    while(query.next())
    {
        ui->moduleBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From projects order by id asc;", Global::i()->db());
    i = 0;
    ui->projectBox->insertItem(i++, tr("Alle"),0);
    while(query.next())
    {
        ui->projectBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }
}
