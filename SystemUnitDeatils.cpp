#include "SystemUnitDeatils.h"
#include "ui_SystemUnitDeatils.h"
#include "SystemUnitData.h"

SystemUnitDeatils::SystemUnitDeatils(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemUnitDeatils)
{
    ui->setupUi(this);
    ui->testState->insertItem(0, tr("Nicht getestet"));
    ui->testState->insertItem(1, tr("Test bestanden"));
    ui->testState->insertItem(2, tr("Test nicht bestanden"));

    connect(ui->testState, SIGNAL(currentIndexChanged(int)), this, SLOT(onTestStateChanged(int)));
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
    ui->testState->setCurrentIndex(m_systemUnitData->state());
}

void SystemUnitDeatils::onTestStateChanged(int index)
{
    if(m_systemUnitData)
    {
        m_systemUnitData->setState(index);
        m_systemUnitData->saveToDB();
    }
}
