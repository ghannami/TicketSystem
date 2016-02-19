#include "NewSystemUnit.h"
#include "ui_NewSystemUnit.h"
#include "SystemUnitData.h"
#include "Global.h"

NewSystemUnit::NewSystemUnit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSystemUnit)
{
    ui->setupUi(this);
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->system, SIGNAL(currentIndexChanged(int)), this, SLOT(onSystemChanged(int)));
    int i = 0;
    QMapIterator<int, QString>mip(Global::i()->systems());
    while(mip.hasNext())
    {
        mip.next();
        ui->system->insertItem(i++, mip.value(),mip.key());
    }
    i = 0;

}

NewSystemUnit::~NewSystemUnit()
{
    delete ui;
}

void NewSystemUnit::onSave()
{
    if(ui->title->text().isEmpty())
    {
        ui->title->setStyleSheet("QLineEdit#name{background-color:rgb(255,0,0,100)}");
        return;
    }

    SystemUnitData unit;
    unit.setTitle(ui->title->text());
    unit.setSystemId(ui->system->itemData(ui->system->currentIndex()).toInt());
    unit.setCategorieId(ui->categorie->itemData(ui->categorie->currentIndex()).toInt());

    unit.saveToDB();
    accept();
}

void NewSystemUnit::onSystemChanged(int index)
{
    int i = 0;
   QMapIterator<int, QString>mip(Global::i()->systemUnitCategories(ui->system->itemData(index).toInt()));
    while(mip.hasNext())
    {
        mip.next();
        ui->categorie->insertItem(i++, mip.value(),mip.key());
    }
}
