#include "NewProject.h"
#include "ui_NewProject.h"
#include "ProjectItemData.h"

NewProject::NewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProject)
{
    ui->setupUi(this);
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

NewProject::~NewProject()
{
    delete ui;
}

void NewProject::onSave()
{
    if(ui->name->text().isEmpty())
    {
        ui->name->setStyleSheet("QLineEdit#name{background-color:rgb(255,0,0,100)}");
        return;
    }

    ProjectItemData prj;
    prj.setTaskName(ui->name->text());
    prj.saveToDB();

    accept();
}
