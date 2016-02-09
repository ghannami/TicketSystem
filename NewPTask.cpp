#include "NewPTask.h"
#include "ui_NewPTask.h"
#include "PTaskItemData.h"
#include "Global.h"
#include <QDebug>

NewPTask::NewPTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPTask)
{
    ui->setupUi(this);
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(close()));

    QMapIterator<int, QString>mip(Global::i()->projects());
    int i = 0;
    while(mip.hasNext())
    {
        mip.next();
        ui->project->insertItem (i++, mip.value(),mip.key());
    }
    i = 0;
    mip = QMapIterator<int, QString>(Global::i()->taskTypes());
    while(mip.hasNext())
    {
        mip.next();
        ui->type->insertItem(i++, mip.value(),mip.key());
    }
    ui->type->setCurrentIndex(1);
    connect(ui->startDate, SIGNAL(dateChanged(QDate)), this, SLOT(onStartDateChanged(QDate)));
    ui->startDate->setDate(QDate::currentDate());
    ui->endDate->setDate(QDate::currentDate());
}

NewPTask::~NewPTask()
{
    delete ui;
}

void NewPTask::onSave()
{
    if(ui->name->text().isEmpty())
    {
        ui->name->setStyleSheet("QLineEdit#name{background-color:rgb(255,0,0,100)}");
        return;
    }
    PTaskItemData data;
    data.setID(-1);
    data.setOrder(-1);
    data.setProjectId(ui->project->itemData(ui->project->currentIndex()).toInt());
    data.setTaskType(ui->type->itemData(ui->type->currentIndex()).toInt());
    data.setTaskEnd(ui->endDate->date());
    data.setTaskStart(ui->startDate->date());
    data.setTaskName(ui->name->text());
    data.setPercent(0);
    data.setDescription(ui->description->toPlainText());
    data.setDate(QDate::currentDate());
    data.saveToDB();
    accept();
}

void NewPTask::onStartDateChanged(const QDate &date)
{
    qDebug() << ui->endDate->date();
    qDebug() << date;
    if(ui->endDate->date() < date)
        ui->endDate->setDate(date);
    ui->endDate->setMinimumDate(date);
}
