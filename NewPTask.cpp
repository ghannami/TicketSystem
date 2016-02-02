#include "NewPTask.h"
#include "ui_NewPTask.h"
#include "PTaskItemData.h"

NewPTask::NewPTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewPTask)
{
    ui->setupUi(this);
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(close()));
}

NewPTask::~NewPTask()
{
    delete ui;
}

void NewPTask::onSave()
{
    PTaskItemData data();
    data.setID(-1);
    data.setOrder(-1);
    data.setProjectId(ui->project->itemData(ui->project->currentIndex()).toInt());
    data.setTaskEnd(ui->endDate->date());
    data.setTaskStart(ui->startDate->date());
    data.setTaskName(ui->name->text());
    data.saveToDB();
}
