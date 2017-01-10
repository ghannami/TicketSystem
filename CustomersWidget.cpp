#include "CustomersWidget.h"
#include "ui_CustomersWidget.h"
#include "NewCustomer.h"

CustomersWidget::CustomersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomersWidget)
{
    ui->setupUi(this);
}

CustomersWidget::~CustomersWidget()
{
    delete ui;
}

void CustomersWidget::newCustomer()
{
    NewCustomer cstm;
    if(cstm.exec() == QDialog::Accepted)
    {
        ///
    }
}
