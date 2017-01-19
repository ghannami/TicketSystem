#include "NewTracking.h"
#include "ui_NewTracking.h"
#include "Global.h"
#include "TrackingData.h"

NewTracking::NewTracking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTracking)
{
    ui->setupUi(this);

    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(reject()));

    int i = 0;
    QMapIterator<int, QString>mip(Global::i()->systemVersions());
    while(mip.hasNext())
    {
        mip.next();
        ui->systemBox->insertItem(i++, mip.value(),mip.key());
    }
    i = 0;

    mip = QMapIterator<int, QString>(Global::i()->trackingTypes());
    while(mip.hasNext())
    {
        mip.next();
        ui->typeBox->insertItem(i++, mip.value(),mip.key());
    }
    i = 0;

    mip = QMapIterator<int, QString>(Global::i()->trackingBehavior());
    while(mip.hasNext())
    {
        mip.next();
        ui->behaviorBox->insertItem(i++, mip.value(),mip.key());
    }
    i = 0;

    ui->title->setFocus();

}

NewTracking::~NewTracking()
{
    delete ui;
}

void NewTracking::onSave()
{
    if(ui->title->text().isEmpty())
    {
        ui->title->setStyleSheet("QLineEdit#name{background-color:rgb(255,0,0,100)}");
        return;
    }

    TrackingData trackingDate;
    trackingDate.setTitle(ui->title->text());
    trackingDate.setSystemVersionId(ui->systemBox->itemData(ui->systemBox->currentIndex()).toInt());
    trackingDate.setTypeId(ui->typeBox->itemData(ui->typeBox->currentIndex()).toInt());
    trackingDate.setBahaviorId(ui->behaviorBox->itemData(ui->behaviorBox->currentIndex()).toInt());
    trackingDate.setText(ui->text->toPlainText());
    trackingDate.saveToDB();
    accept();
}
