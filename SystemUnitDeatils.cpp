#include "SystemUnitDeatils.h"
#include "ui_SystemUnitDeatils.h"
#include "SystemUnitData.h"
#include "Global.h"
#include "NewTicket.h"

SystemUnitDeatils::SystemUnitDeatils(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemUnitDeatils)
{
    ui->setupUi(this);
    m_blockUpdate = false;
    ui->testState->insertItem(0, tr("Nicht getestet"));
    ui->testState->insertItem(1, tr("Test bestanden"));
    ui->testState->insertItem(2, tr("Test nicht bestanden"));

    connect(ui->testState, SIGNAL(currentIndexChanged(int)), this, SLOT(onTestStateChanged(int)));
    connect(ui->newTicket, SIGNAL(clicked(bool)), this, SLOT(newTicket()));
}

SystemUnitDeatils::~SystemUnitDeatils()
{
    delete ui;
}

SystemUnitData *SystemUnitDeatils::systemUnitData() const
{
    return m_systemUnitData;
}

void SystemUnitDeatils::setSystemUnitData(SystemUnitData *systemUnitData)
{
    m_systemUnitData = systemUnitData;
    m_blockUpdate = true;
    ui->testState->setCurrentIndex(m_systemUnitData->state());
    ui->testedFrom->setText(Global::i()->users().value(m_systemUnitData->userId()));
    ui->testetdOn->setText(m_systemUnitData->date().toString("dd.MM.yyyy"));
    ui->title->setPlainText(m_systemUnitData->title());
    m_blockUpdate = false;
}

void SystemUnitDeatils::onTestStateChanged(int index)
{
    if(m_systemUnitData && !m_blockUpdate)
    {
        m_systemUnitData->setState(index);
        m_systemUnitData->saveToDB();
        emit unitDataChanged();
    }
}

void SystemUnitDeatils::newTicket()
{
    NewTicket ticket;
    ticket.setTitle(m_systemUnitData->title());
    ticket.setSystemVersion(m_systemUnitData->systemVersionId());
    ticket.setUnitCategorie(m_systemUnitData->categorieId());
    ticket.setType(1); // Bug
    ticket.exec();

    emit ticketCreated();
}
