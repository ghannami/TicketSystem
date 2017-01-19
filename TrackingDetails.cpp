#include "TrackingDetails.h"
#include "ui_TrackingDetails.h"
#include "Global.h"
#include "TrackingData.h"

TrackingDetails::TrackingDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackingDetails)
{
    ui->setupUi(this);
}

TrackingDetails::~TrackingDetails()
{
    delete ui;
}

TrackingData *TrackingDetails::trackingData() const
{
    return m_trackingData;
}

void TrackingDetails::setTrackingData(TrackingData *trackingData)
{
    m_trackingData = trackingData;
    ui->behaviorLabel->setText(Global::i()->trackingBehavior().value(trackingData->behaviorId()));
    ui->dateLabel->setText(trackingData->date().toString("dd.MM.yyyy"));
    ui->typeLabel->setText(Global::i()->trackingTypes().value(trackingData->typeId()));
    ui->systemLabel->setText(Global::i()->systemVersions().value(trackingData->systemVersionId()));
    ui->userLabel->setText(Global::i()->users().value(trackingData->userId()));
    ui->title->setPlainText(trackingData->title());
    ui->text->setPlainText(trackingData->text());
}
