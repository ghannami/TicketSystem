#include "NewCustomer.h"
#include "ui_NewCustomer.h"
#include "CustomerData.h"

NewCustomer::NewCustomer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCustomer)
{
    ui->setupUi(this);

    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

NewCustomer::~NewCustomer()
{
    delete ui;
}

void NewCustomer::onSave()
{
    if(ui->name->text().isEmpty())
    {
        ui->name->setStyleSheet("QLineEdit#name{background-color:rgb(255,0,0,100)}");
        return;
    }

    CustomerData cstm;
    cstm.setName(ui->name->text());
    cstm.saveToDB();
    accept();
}
