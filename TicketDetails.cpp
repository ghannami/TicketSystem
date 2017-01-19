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
    m_ticketItem = nullptr;
    int i = 0;
    m_blockFieldsChange = false;
    QMapIterator<int, QString>mip(Global::i()->users());
    while(mip.hasNext())
    {
        mip.next();
        ui->commentToUserBox->insertItem(i, mip.value(),mip.key());
        ui->ticketToUserBox->insertItem(i, mip.value(),mip.key());
        i++;
    }
    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->prioritys());
    while(mip.hasNext())
    {
        mip.next();
        ui->priorityBox->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->unitCategories());
    while(mip.hasNext())
    {
        mip.next();
        ui->categorie->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->departments());
    while(mip.hasNext())
    {
        mip.next();
        ui->department->insertItem(i++, mip.value(),mip.key());
    }

    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->systemVersions());
    while(mip.hasNext())
    {
        mip.next();
        ui->systemBox->insertItem(i++, mip.value(),mip.key());
    }

    connect(ui->send, SIGNAL(clicked(bool)), this, SLOT(send()));
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));

    connect(ui->priorityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->ticketToUserBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->department, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->categorie, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->systemBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->percent, SIGNAL(valueChanged(int)), this, SLOT(onFieldsChanged()));
    connect(ui->deadline, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onFieldsChanged()));

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
    q += QString::number(m_ticketItem->ticketID()) + " ";
    q += "order by date asc;";
    QSqlQuery query(q, Global::i()->db());
    QString text = "<head></head><body>";
    while(query.next())
    {
        text += "<div style= \"background-color: #008AB8; display:block; widht:100%; hight:100%;\">";
        text += "<h4 style=\"color:#fff;\">" + query.value("fromuser").toString();
        text += " --> " + query.value("touser").toString()+ "";
        text += " [" + query.value("date").toDateTime().toString(Global::i()->dateTimeFormat()) + "]</h4>";
        text += "<span style=\" font-size:10pt;\">" + query.value("text").toString()+ "</span><br \\>";
        text += "</div>";
    }
    text +="</body>";
    ui->history->appendHtml(text);
}

void TicketDetails::updateTicket()
{
    bool disableWidgets = false;
    if(m_ticketItem)
        if(m_ticketItem->state() == 3)
            disableWidgets = true;
    if(!m_ticketItem)
        disableWidgets = true;

    if(disableWidgets)
    {
        ui->comment->setDisabled(true);
        ui->groupBox->setDisabled(true);
        ui->commentToUserBox->setDisabled(true);
        ui->send->setDisabled(true);
        ui->sendToLabel->setDisabled(true);
        ui->changeState->setDisabled(true);
        if(m_ticketItem->state() == 3)
        {
            ui->processedByLabel1->setVisible(true);
            ui->processedByLabel2->setVisible(true);
            ui->testedByLabel1->setVisible(true);
            ui->testedByLabel2->setVisible(true);
            ui->processedByLabel2->setText(Global::i()->users().value(m_ticketItem->processedBy()));
            ui->testedByLabel2->setText(Global::i()->users().value(m_ticketItem->testedBy()));
        }
    }
    else
    {
        ui->comment->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->commentToUserBox->setEnabled(true);
        ui->send->setEnabled(true);
        ui->sendToLabel->setEnabled(true);
        if(Global::i()->userID() != m_ticketItem->toUser() && (m_ticketItem->state() == 1 || m_ticketItem->state() == 2))
        {
            ui->changeState->setDisabled(true);
            ui->reopen->setDisabled(true);
        }
        else
        {
            ui->changeState->setEnabled(true);
            ui->reopen->setEnabled(true);
        }
        bool isUser = Global::i()->userID() == m_ticketItem->fromUser();
        ui->deadline->setEnabled(isUser);
    }

    if(!m_ticketItem)
        return;

    m_blockFieldsChange = true;
    ui->ticketIDLabel->setText(QString::number(m_ticketItem->ticketID()));
    ui->createdOn->setText(m_ticketItem->date().toString("dd.MM.yyyy HH:mm"));
    ui->percent->setValue(m_ticketItem->percentComplete());
    ui->deadline->setDateTime(m_ticketItem->deadline());

    loadComments();

    ui->customerLabel->setText(Global::i()->customers().value(m_ticketItem->customer()));
    ui->title->setText(m_ticketItem->title());
    for(int index = 0; index < ui->commentToUserBox->count(); index++)
    {
        if(ui->ticketToUserBox->itemData(index).toInt() ==  m_ticketItem->toUser())
        {
            ui->ticketToUserBox->setCurrentIndex(index);
        }
        if(ui->commentToUserBox->itemData(index).toInt() == m_ticketItem->fromUser())
        {
            ui->commentToUserBox->setCurrentIndex(index);
        }
    }

    for(int index = 0; index < ui->priorityBox->count(); index++)
    {
        if(ui->priorityBox->itemData(index).toInt() == m_ticketItem->priority())
        {
            ui->priorityBox->setCurrentIndex(index);
        }
    }

    for(int index = 0; index < ui->department->count(); index++)
    {
        if(ui->department->itemData(index).toInt() == m_ticketItem->department())
        {
            ui->department->setCurrentIndex(index);
        }
    }

    for(int index = 0; index < ui->categorie->count(); index++)
    {
        if(ui->categorie->itemData(index).toInt() == m_ticketItem->unitCategorie())
        {
            ui->categorie->setCurrentIndex(index);
        }
    }
    for(int index = 0; index < ui->systemBox->count(); index++)
    {
        if(ui->systemBox->itemData(index).toInt() == m_ticketItem->systemVersion())
        {
            ui->systemBox->setCurrentIndex(index);
        }
    }

    ui->createdByLabel2->setText(Global::i()->users().value( m_ticketItem->fromUser()));

    ui->reopen->setVisible(false);
    ui->processedByLabel1->setVisible(false);
    ui->processedByLabel2->setVisible(false);
    ui->testedByLabel1->setVisible(false);
    ui->testedByLabel2->setVisible(false);

    if(m_ticketItem->state() == 1)
    {
        ui->changeState->setText(tr("Erledigt"));
    }
    else if(m_ticketItem->state() == 2)
    {
        ui->changeState->setText(tr("Schließen"));
        ui->reopen->setVisible(true);

        ui->processedByLabel1->setVisible(true);
        ui->processedByLabel2->setVisible(true);
        ui->processedByLabel2->setText(Global::i()->users().value(m_ticketItem->processedBy()));
    }

    m_blockFieldsChange = false;
}

TicketItem *TicketDetails::ticketItem() const
{
    return m_ticketItem;
}

void TicketDetails::setTicketItem(TicketItem *ticketItem)
{
    setDisabled(!ticketItem);
    if(m_ticketItem == ticketItem)
        return;

    m_ticketItem = ticketItem;

    if(!m_ticketItem)
        return;

    ticketItem->setViewed(true, true);
    updateTicket();
    ui->save->setDisabled(true);
}

void TicketDetails::send()
{
    CommentItem cItem;
    cItem.setDate(QDateTime::currentDateTime());
    cItem.setFromUserID(Global::i()->userID());
    cItem.setToUserID(ui->commentToUserBox->itemData(ui->commentToUserBox->currentIndex()).toInt());
    cItem.setTicketID(m_ticketItem->ticketID());
    cItem.setText(ui->comment->toPlainText());
    cItem.setViewed(0);
    cItem.saveToDB();

    ui->comment->clear();
    loadComments();
    //emit changed();
}

void TicketDetails::onFieldsChanged()
{
    ui->save->setEnabled(true);
}

void TicketDetails::changeTicketState()
{
    if(!m_ticketItem)
        return;

    CommentItem cItem;
    cItem.setDate(QDateTime::currentDateTime());
    cItem.setFromUserID(Global::i()->userID());
    cItem.setToUserID(ui->commentToUserBox->itemData(ui->commentToUserBox->currentIndex()).toInt());
    cItem.setTicketID(m_ticketItem->ticketID());
    cItem.setViewed(0);

    if(m_ticketItem->state() == 1)
    {
        m_ticketItem->setState(2);
        m_ticketItem->setProcessedBy(Global::i()->userID());
        m_ticketItem->setProcessedOn(QDateTime::currentDateTime());
        m_ticketItem->saveToDB();

        cItem.setText(tr("Neuer Status: ")+Global::i()->stats().value(2));
        cItem.saveToDB();
        updateTicket();
        emit changed();
    }
    else if(m_ticketItem->state() == 2)
    {        
        m_ticketItem->setState(3);
        m_ticketItem->setTestedBy(Global::i()->userID());
        m_ticketItem->setTestedOn(QDateTime::currentDateTime());
        m_ticketItem->saveToDB();

        cItem.setText(tr("Neuer Status: ")+Global::i()->stats().value(3));
        cItem.saveToDB();
        updateTicket();
        emit changed();
    }
}

void TicketDetails::reopenTicket()
{
    if(!m_ticketItem)
        return;

    if(m_ticketItem->state() == 2)
    {
        QSqlQuery query(QString("UPDATE ticket set state = 1 where id = %1").arg(m_ticketItem->ticketID()), Global::i()->db());
        query.exec();
        updateTicket();
        emit changed();
    }
}

void TicketDetails::onSave()
{
    if(!m_blockFieldsChange && m_ticketItem)
    {
        m_ticketItem->setDepartment(ui->department->itemData(ui->department->currentIndex()).toInt());
        m_ticketItem->setUnitCategorie(ui->categorie->itemData(ui->categorie->currentIndex()).toInt());
        m_ticketItem->setPriority(ui->priorityBox->itemData(ui->priorityBox->currentIndex()).toInt());
        m_ticketItem->setToUser(ui->ticketToUserBox->itemData(ui->ticketToUserBox->currentIndex()).toInt());
        m_ticketItem->setPercentComplete(ui->percent->value());
        m_ticketItem->setDeadline(ui->deadline->dateTime());
        m_ticketItem->setSystemVersion(ui->systemBox->itemData(ui->systemBox->currentIndex()).toInt());

        m_ticketItem->saveToDB();

        setTicketItem(nullptr);
        emit changed();
    }
}
