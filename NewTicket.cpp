#include "NewTicket.h"
#include "ui_NewTicket.h"
#include "Global.h"
#include <QSqlQuery>
#include <QDateTime>

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

    if(!ui->comment->toPlainText().isEmpty())
    {
        int id = query.lastInsertId().toInt();
        query.prepare("INSERT INTO comments (date, from_user, to_user, ticket, text) "
                      "VALUES (:date, :from_user, :to_user, :ticket, :text)");
        query.bindValue(":date", QDateTime::currentDateTime());
        query.bindValue(":from_user", Global::i()->userID());
        query.bindValue(":to_user", ui->toUserBox->itemData(ui->toUserBox->currentIndex()));
        query.bindValue(":ticket", id);
        query.bindValue(":text", ui->comment->toPlainText());
        query.exec();
    }
    accept();
}

void NewTicket::setupeBoxes()
{
    ui->typeBox->clear();
    ui->toUserBox->clear();
    ui->priorityBox->clear();

    QSqlQuery query("SELECT name, id From tickettype order by id asc;", Global::i()->db());
    int i = 0;
    while(query.next())
    {
        ui->typeBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From user order by id asc;", Global::i()->db());
    i = 0;
    while(query.next())
    {
        ui->toUserBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From priority order by id asc;", Global::i()->db());
    i = 0;
    while(query.next())
    {
        ui->priorityBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From categories order by id asc;", Global::i()->db());
    i = 0;
    while(query.next())
    {
        ui->categorieBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    query = QSqlQuery("SELECT name, id From projects order by id asc;", Global::i()->db());
    i = 0;
    while(query.next())
    {
        ui->projectBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }
}
