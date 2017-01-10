#include "NewTicket.h"
#include "ui_NewTicket.h"
#include "Global.h"
#include <QSqlQuery>
#include <QDateTime>
#include "CommentItem.h"
#include <QVariant>
#include <QSqlError>
#include <QDebug>

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

    QSqlQuery query(Global::i()->db());
    query.prepare("INSERT INTO ticket (type, from_user, to_user, system_version, state, unit_categorie, priority, title, date) "
                  "VALUES (:type, :from_user, :to_user, :system_version, :state, :unit_categorie, :priority, :title, :date)");
    query.bindValue(":type", ui->typeBox->itemData(ui->typeBox->currentIndex()));
    query.bindValue(":from_user", Global::i()->userID());
    query.bindValue(":to_user", ui->toUserBox->itemData(ui->toUserBox->currentIndex()));
    query.bindValue(":system_version", ui->systemBox->itemData(ui->systemBox->currentIndex()));
    query.bindValue(":state", 1);
    query.bindValue(":unit_categorie", ui->categorieBox->itemData(ui->categorieBox->currentIndex()));
    query.bindValue(":priority", ui->priorityBox->itemData(ui->priorityBox->currentIndex()));
    query.bindValue(":title", ui->title->text());
    query.bindValue(":date", QDateTime::currentDateTime());
    if(!query.exec())
        qDebug() << query.lastError().text();

    int id = query.lastInsertId().toInt();
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

    i = 1;
    mip = QMapIterator<int, QString>(Global::i()->customers());
    while(mip.hasNext())
    {
        mip.next();
        ui->customerBox->insertItem(i++, mip.value(),mip.key());
    }

    if(i > 0)
        ui->systemBox->setCurrentIndex(i-1);
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
