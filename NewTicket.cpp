#include "NewTicket.h"
#include "ui_NewTicket.h"
#include "Global.h"
#include <QSqlQuery>
#include <QDateTime>
#include "CommentItem.h"
#include <QVariant>

NewTicket::NewTicket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTicket)
{
    ui->setupUi(this);

    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(close()));

    setupeBoxes();
}

NewTicket::~NewTicket()
{
    delete ui;
}

void NewTicket::onSave()
{
    QSqlQuery query(Global::i()->db());
    query.prepare("INSERT INTO ticket (type, from_user, to_user, project, state, categorie, priority, title, date) "
                  "VALUES (:type, :from_user, :to_user, :project, :state, :categorie, :priority, :title, :date)");
    query.bindValue(":type", ui->typeBox->itemData(ui->typeBox->currentIndex()));
    query.bindValue(":from_user", Global::i()->userID());
    query.bindValue(":to_user", ui->toUserBox->itemData(ui->toUserBox->currentIndex()));
    query.bindValue(":project", ui->projectBox->itemData(ui->projectBox->currentIndex()));
    query.bindValue(":state", 1);
    query.bindValue(":categorie", ui->categorieBox->itemData(ui->categorieBox->currentIndex()));
    query.bindValue(":priority", ui->priorityBox->itemData(ui->priorityBox->currentIndex()));
    query.bindValue(":title", ui->title->text());
    query.bindValue(":date", QDateTime::currentDateTime());
    query.exec();

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
    mip = QMapIterator<int, QString>(Global::i()->types());
    while(mip.hasNext())
    {
        mip.next();
        ui->typeBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->categories());
    while(mip.hasNext())
    {
        mip.next();
        ui->categorieBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->projects());
    while(mip.hasNext())
    {
        mip.next();
        ui->projectBox->insertItem(i++, mip.value(),mip.key());
    }
}
