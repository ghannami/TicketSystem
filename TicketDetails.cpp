#include "TicketDetails.h"
#include "ui_TicketDetails.h"
#include <QSqlQuery>
#include "Global.h"
#include <QDateTime>
#include <QSqlError>
#include <QDebug>

TicketDetails::TicketDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketDetails)
{
    ui->setupUi(this);

    QSqlQuery query("SELECT name, id From user order by id asc;", Global::i()->db());
    int i = 0;
    while(query.next())
    {
        ui->toUserBox->insertItem(i, query.value("name").toString(),query.value("id"));
        ui->ticketToUserBox->insertItem(i++, query.value("name").toString(),query.value("id"));
        ui->testerBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }
    query = QSqlQuery("SELECT name, id From priority order by id asc;", Global::i()->db());
    i = 0;
    while(query.next())
    {
        ui->priorityBox->insertItem(i++, query.value("name").toString(),query.value("id"));
    }

    connect(ui->send, SIGNAL(clicked(bool)), this, SLOT(send()));
    connect(ui->priorityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->ticketToUserBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->changeState, SIGNAL(clicked(bool)), this, SLOT(changeTicketState()));
    connect(ui->reopen, SIGNAL(clicked(bool)), this, SLOT(reopenTicket()));

    ui->history->ensureCursorVisible();
}

TicketDetails::~TicketDetails()
{
    delete ui;
}

void TicketDetails::loadComments()
{
    ui->history->clear();

    QString q = "SELECT c.text, c.date, u1.name as fromuser, u2.name as touser ";
    q += "FROM comments c, user u1, user u2 ";
    q += "where c.from_user = u1.id and c.to_user = u2.id and c.ticket = ";
    q += QString::number(m_ticketID) + " ";
    q += "order by date asc;";
    QSqlQuery query(q, Global::i()->db());
    QString text = "<head></head><body>";
    while(query.next())
    {
        text += "<div style= \"background-color: #008AB8; display:block; widht:100%; hight:100%;\">";
        text += "<h4 style=\"color:#fff;\">" + query.value("fromuser").toString();
        text += " --> " + query.value("touser").toString()+ "";
        text += " [" + query.value("date").toDateTime().toString("dd.MM.yyyy - hh:mm") + "]</h4>";
        text += "<span style=\" font-size:10pt;\">" + query.value("text").toString()+ "</span><br \\>";
        text += "</div>";
    }
    text +="</body>";
    ui->history->appendHtml(text);
}

int TicketDetails::ticketID() const
{
    return m_ticketID;
}

void TicketDetails::setTicketID(int ticketID)
{
    m_ticketID = ticketID;
    ui->ticketIDLabel->setText(QString::number(m_ticketID));
    loadComments();
    QSqlQuery query("SELECT state, from_user, to_user, priority FROM ticket where id = "+QString::number(m_ticketID), Global::i()->db());
    if(query.next())
    {
        m_ticketState = query.value("state").toInt();
        m_fromUserID = query.value("from_user").toInt();
        m_toUserID = query.value("to_user").toInt();
    }
    for(int index = 0; index < ui->toUserBox->count(); index++)
    {
        if(ui->toUserBox->itemData(index).toInt() == m_toUserID)
        {
            ui->toUserBox->setCurrentIndex(index);
            ui->ticketToUserBox->setCurrentIndex(index);
        }
        if(ui->testerBox->itemData(index).toInt() == m_fromUserID)
        {
            ui->testerBox->setCurrentIndex(index);
        }
    }

    for(int index = 0; index < ui->priorityBox->count(); index++)
    {
        if(ui->priorityBox->itemData(index).toInt() == query.value("priority").toInt())
        {
            ui->priorityBox->setCurrentIndex(index);
        }
    }

    ui->reopen->setVisible(false);
    ui->testerLabel->setVisible(false);
    ui->testerBox->setVisible(false);

    if(m_ticketState == 1)
    {
        ui->changeState->setText(tr("Testen"));
        ui->testerLabel->setVisible(true);
        ui->testerBox->setVisible(true);
    }
    else if(m_ticketState == 2)
    {
        ui->changeState->setText(tr("Schließen"));
        ui->reopen->setVisible(true);
    }
    if(m_ticketID <= 0 || m_ticketState == 3)
    {
        ui->comment->setDisabled(true);
        ui->groupBox->setDisabled(true);
        ui->toUserBox->setDisabled(true);
        ui->send->setDisabled(true);
        ui->sendToLabel->setDisabled(true);
        ui->changeState->setDisabled(true);
    }
    else
    {
        ui->comment->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->toUserBox->setEnabled(true);
        ui->send->setEnabled(true);
        ui->sendToLabel->setEnabled(true);
        if(Global::i()->userID() != m_toUserID && (m_ticketState == 1 || m_ticketState == 2))
        {
            ui->changeState->setDisabled(true);
            ui->reopen->setDisabled(true);
        }
        else
        {
            ui->changeState->setEnabled(true);
            ui->reopen->setEnabled(true);
        }
    }
}

void TicketDetails::send()
{
    QSqlQuery query(Global::i()->db());
    query.prepare("INSERT INTO comments (date, from_user, to_user, ticket, text) "
                  "VALUES (:date, :from_user, :to_user, :ticket, :text)");
    query.bindValue(":date", QDateTime::currentDateTime());
    query.bindValue(":from_user", Global::i()->userID());
    query.bindValue(":to_user", ui->toUserBox->itemData(ui->toUserBox->currentIndex()));
    query.bindValue(":ticket", m_ticketID);
    query.bindValue(":text", ui->comment->toPlainText());
    query.exec();
    ui->comment->clear();
    loadComments();
    //emit changed();
}

void TicketDetails::onFieldsChanged()
{
    QSqlQuery query(QString("UPDATE ticket set priority = %1, to_user = %2 WHERE id = %3")
                    .arg(ui->priorityBox->itemData(ui->priorityBox->currentIndex()).toInt())
                    .arg(ui->ticketToUserBox->itemData(ui->ticketToUserBox->currentIndex()).toInt()).arg(m_ticketID), Global::i()->db());
    query.exec();
    setTicketID(m_ticketID);
    emit changed();
}

void TicketDetails::changeTicketState()
{
    if(m_ticketState == 1)
    {
        QSqlQuery query(QString("UPDATE ticket set state = 2, to_user = %1 where id = %2")
                            .arg(ui->testerBox->itemData(ui->testerBox->currentIndex()).toInt())
                            .arg(m_ticketID), Global::i()->db());
        query.exec();
        setTicketID(m_ticketID);
        emit changed();
    }
    else if(m_ticketState == 2)
    {
        QSqlQuery query(QString("UPDATE ticket set state = 3 where id = %1").arg(m_ticketID), Global::i()->db());
        query.exec();
        setTicketID(m_ticketID);
        emit changed();
    }
}

void TicketDetails::reopenTicket()
{
    if(m_ticketState == 2)
    {
        QSqlQuery query(QString("UPDATE ticket set state = 1 where id = %1").arg(m_ticketID), Global::i()->db());
        query.exec();
        setTicketID(m_ticketID);
        emit changed();
    }
}
