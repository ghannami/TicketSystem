#include "TicketDetails.h"
#include "ui_TicketDetails.h"
#include <QSqlQuery>
#include "Global.h"
#include <QDateTime>
#include <QSqlError>
#include <QDebug>
#include "TicketModel.h"
#include "CommentItem.h"
#include "TicketItem.h"

TicketDetails::TicketDetails(TicketModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TicketDetails),
    m_model(model)
{
    ui->setupUi(this);
    int i = 0;
    m_blockFieldsChange = false;
    QMapIterator<int, QString>mip(Global::i()->users());
    while(mip.hasNext())
    {
        mip.next();
        ui->commentToUserBox->insertItem(i, mip.value(),mip.key());
        ui->ticketToUserBox->insertItem(i, mip.value(),mip.key());
        ui->testerBox->insertItem(i, mip.value(),mip.key());
        i++;
    }
    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->prioritys());
    while(mip.hasNext())
    {
        mip.next();
        ui->priorityBox->insertItem(i++, mip.value(),mip.key());
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

    QString q = "SELECT c.text, c.date, c.viewed, u1.name as fromuser, u2.name as touser ";
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
    if(m_ticketID == ticketID)
        return;

    m_ticketID = ticketID;
    if(m_model->item(m_model->index(m_ticketID)))
    {
        m_model->item(m_model->index(m_ticketID))->setViewed(true, true);
    }
    updateTicket();
}

void TicketDetails::updateTicket()
{
    m_blockFieldsChange = true;
    ui->ticketIDLabel->setText(QString::number(m_ticketID));
    loadComments();
    QSqlQuery query("SELECT title, state, from_user, to_user, priority, customer FROM ticket where id = "+QString::number(m_ticketID), Global::i()->db());
    if(query.next())
    {
        m_ticketState = query.value("state").toInt();
        m_fromUserID = query.value("from_user").toInt();
        m_toUserID = query.value("to_user").toInt();
    }
    ui->customerLabel->setText(Global::i()->customers().value(query.value("customer").toInt()));
    ui->title->setText(query.value("title").toString());
    for(int index = 0; index < ui->commentToUserBox->count(); index++)
    {
        if(ui->ticketToUserBox->itemData(index).toInt() == m_toUserID)
        {
            ui->ticketToUserBox->setCurrentIndex(index);
        }
        if(ui->commentToUserBox->itemData(index).toInt() == m_fromUserID)
        {
            ui->commentToUserBox->setCurrentIndex(index);
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

    ui->createdByLabel2->setText(Global::i()->users().value(m_fromUserID));

    ui->reopen->setVisible(false);
    ui->testerLabel->setVisible(false);
    ui->testerBox->setVisible(false);
    ui->processedByLabel1->setVisible(false);
    ui->processedByLabel2->setVisible(false);
    ui->testedByLabel1->setVisible(false);
    ui->testedByLabel2->setVisible(false);

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

        ui->processedByLabel1->setVisible(true);
        ui->processedByLabel2->setVisible(true);
        std::shared_ptr<TicketItem> item = m_model->item(m_model->index(m_ticketID));
        if(item)
            ui->processedByLabel2->setText(Global::i()->users().value(item->processedBy()));
    }
    if(m_ticketID <= 0 || m_ticketState == 3)
    {
        ui->comment->setDisabled(true);
        ui->groupBox->setDisabled(true);
        ui->commentToUserBox->setDisabled(true);
        ui->send->setDisabled(true);
        ui->sendToLabel->setDisabled(true);
        ui->changeState->setDisabled(true);
        if(m_ticketState == 3)
        {
            ui->processedByLabel1->setVisible(true);
            ui->processedByLabel2->setVisible(true);
            ui->testedByLabel1->setVisible(true);
            ui->testedByLabel2->setVisible(true);
            std::shared_ptr<TicketItem> item = m_model->item(m_model->index(m_ticketID));
            if(item)
            {
                ui->processedByLabel2->setText(Global::i()->users().value(item->processedBy()));
                ui->testedByLabel2->setText(Global::i()->users().value(item->testedBy()));
            }
        }
    }
    else
    {
        ui->comment->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->commentToUserBox->setEnabled(true);
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
    m_blockFieldsChange = false;

}

void TicketDetails::send()
{
    CommentItem cItem;
    cItem.setDate(QDateTime::currentDateTime());
    cItem.setFromUserID(Global::i()->userID());
    cItem.setToUserID(ui->commentToUserBox->itemData(ui->commentToUserBox->currentIndex()).toInt());
    cItem.setTicketID(m_ticketID);
    cItem.setText(ui->comment->toPlainText());
    cItem.setViewed(0);
    cItem.saveToDB();

    ui->comment->clear();
    loadComments();
    //emit changed();
}

void TicketDetails::onFieldsChanged()
{
    if(!m_blockFieldsChange)
    {
        QSqlQuery query(QString("UPDATE ticket set priority = %1, to_user = %2 WHERE id = %3")
                        .arg(ui->priorityBox->itemData(ui->priorityBox->currentIndex()).toInt())
                        .arg(ui->ticketToUserBox->itemData(ui->ticketToUserBox->currentIndex()).toInt()).arg(m_ticketID), Global::i()->db());
        query.exec();
        updateTicket();
        emit changed();
    }
}

void TicketDetails::changeTicketState()
{
    CommentItem cItem;
    cItem.setDate(QDateTime::currentDateTime());
    cItem.setFromUserID(Global::i()->userID());
    cItem.setToUserID(ui->commentToUserBox->itemData(ui->commentToUserBox->currentIndex()).toInt());
    cItem.setTicketID(m_ticketID);
    cItem.setViewed(0);

    if(m_ticketState == 1)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("UPDATE ticket set state = :state, to_user = :to_user, processed_by = :processed_by, processed_on = :processed_on where id = :id");
        query.bindValue(":state", 2);
        query.bindValue(":to_user", ui->testerBox->itemData(ui->testerBox->currentIndex()).toInt());
        query.bindValue(":processed_by", Global::i()->userID());
        query.bindValue(":processed_on", QDateTime::currentDateTime());
        query.bindValue(":id", m_ticketID);
        query.exec();
        cItem.setText(tr("Neuer Status: ")+Global::i()->stats().value(2));
        cItem.saveToDB();
        updateTicket();
        emit changed();
    }
    else if(m_ticketState == 2)
    {
        QSqlQuery query(Global::i()->db());
        query.prepare("UPDATE ticket set state = :state, tested_by = :tested_by, tested_on = :tested_on where id = :id");
        query.bindValue(":state", 3);
        query.bindValue(":tested_by", Global::i()->userID());
        query.bindValue(":tested_on", QDateTime::currentDateTime());
        query.bindValue(":id", m_ticketID);
        query.exec();
        cItem.setText(tr("Neuer Status: ")+Global::i()->stats().value(3));
        cItem.saveToDB();
        updateTicket();
        emit changed();
    }
}

void TicketDetails::reopenTicket()
{
    if(m_ticketState == 2)
    {
        QSqlQuery query(QString("UPDATE ticket set state = 1 where id = %1").arg(m_ticketID), Global::i()->db());
        query.exec();
        updateTicket();
        emit changed();
    }
}
