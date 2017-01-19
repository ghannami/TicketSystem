#include "NewTicket.h"
#include "ui_NewTicket.h"
#include "Global.h"
#include <QSqlQuery>
#include <QDateTime>
#include "CommentItem.h"
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include "TicketItem.h"

NewTicket::NewTicket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTicket)
{
    ui->setupUi(this);

    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->systemBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSystemChanged(int)));

    setupeBoxes();
}

NewTicket::~NewTicket()
{
    delete ui;
}

void NewTicket::setType(int typeID)
{
    for(int i = 0; i < ui->typeBox->count(); i++)
    {
        if(ui->typeBox->itemData(i) == typeID)
            ui->typeBox->setCurrentIndex(i);
    }
}

void NewTicket::setUnitCategorie(int categorieID)
{
    for(int i = 0; i < ui->categorieBox->count(); i++)
    {
        if(ui->categorieBox->itemData(i) == categorieID)
            ui->categorieBox->setCurrentIndex(i);
    }
}

void NewTicket::setSystemVersion(int systemVersion)
{
    for(int i = 0; i < ui->systemBox->count(); i++)
    {
        if(ui->systemBox->itemData(i) == systemVersion)
            ui->systemBox->setCurrentIndex(i);
    }
}

void NewTicket::setTitle(const QString &title)
{
    ui->title->setText(title);
}

void NewTicket::onSave()
{
    if(ui->title->text().isEmpty())
    {
        ui->title->setStyleSheet("QLineEdit#title{background-color:rgb(255,0,0,100)}");
        return;
    }

    TicketItem ticketItem;
    ticketItem.setType(ui->typeBox->itemData(ui->typeBox->currentIndex()).toInt());
    ticketItem.setFromUser(Global::i()->userID());
    ticketItem.setToUser(ui->toUserBox->itemData(ui->toUserBox->currentIndex()).toInt());
    ticketItem.setSystemVersion(ui->systemBox->itemData(ui->systemBox->currentIndex()).toInt());
    ticketItem.setState(1);
    ticketItem.setUnitCategorie(ui->categorieBox->itemData(ui->categorieBox->currentIndex()).toInt());
    ticketItem.setPriority(ui->priorityBox->itemData(ui->priorityBox->currentIndex()).toInt());
    ticketItem.setTitle(ui->title->text());
    ticketItem.setDate(QDateTime::currentDateTime());
    ticketItem.setCustomer(ui->customerBox->itemData(ui->customerBox->currentIndex()).toInt());
    ticketItem.setDepartment(ui->department->itemData(ui->department->currentIndex()).toInt());
    ticketItem.setDeadline(ui->deadline->dateTime());
    int id = ticketItem.saveToDB();

    CommentItem cItem;
    cItem.setFromUserID(Global::i()->userID());
    cItem.setToUserID(ui->toUserBox->itemData(ui->toUserBox->currentIndex()).toInt());
    cItem.setTicketID(id);
    cItem.setDate(QDateTime::currentDateTime());
    cItem.setText(tr("Ticket erstellt."));
    cItem.setViewed(0);
    cItem.saveToDB();

    if(!ui->comment->toPlainText().isEmpty())
    {
        cItem.setDate(QDateTime::currentDateTime());
        cItem.setText(ui->comment->toPlainText());
        cItem.saveToDB();
    }
    accept();
}

void NewTicket::setupeBoxes()
{
    ui->typeBox->clear();
    ui->toUserBox->clear();
    ui->priorityBox->clear();
    ui->systemBox->clear();
    ui->customerBox->clear();
    ui->department->clear();

    int i = 0;
    QMapIterator<int, QString>mip(Global::i()->users());
    while(mip.hasNext())
    {
        mip.next();
        ui->toUserBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->prioritys());
    while(mip.hasNext())
    {
        mip.next();
        ui->priorityBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->ticketTypes());
    while(mip.hasNext())
    {
        mip.next();
        ui->typeBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->systemVersions());
    while(mip.hasNext())
    {
        mip.next();
        ui->systemBox->insertItem(i++, mip.value(),mip.key());
    }
    if(i > 0)
        ui->systemBox->setCurrentIndex(i-1);

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->customers());
    while(mip.hasNext())
    {
        mip.next();
        ui->customerBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->departments());
    while(mip.hasNext())
    {
        mip.next();
        ui->department->insertItem(i++, mip.value(),mip.key());
    }

    QDateTime dt = QDateTime::currentDateTime();
    ui->deadline->setDateTime(dt.addDays(8));
}

void NewTicket::onSystemChanged(int index)
{
    int i = 0;
   QMapIterator<int, QString>mip(Global::i()->systemUnitCategories(ui->systemBox->itemData(index).toInt()));
    while(mip.hasNext())
    {
        mip.next();
        ui->categorieBox->insertItem(i++, mip.value(),mip.key());
    }
}
