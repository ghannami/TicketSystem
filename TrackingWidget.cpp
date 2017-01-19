#include "TrackingWidget.h"
#include "ui_TrackingWidget.h"
#include "SystemsWidget.h"
#include <QSqlQuery>
#include "Global.h"
#include "TrackingData.h"
#include "TrackingDetails.h"
#include "SystemItemData.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "NewTracking.h"

TrackingWidget::TrackingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrackingWidget)
{
    ui->setupUi(this);

    m_systemsModel = new TreeModel();
    m_systemsModel->setColumnCount(3);
    QStringList headers;
    headers << tr("System") << tr("Version") << tr("Datum");
    m_systemsModel->setHeaders(headers);

    ui->sysView->setModel(m_systemsModel);
    ui->sysView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->sysView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sysView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->sysView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onSystemSelectionChanged(QModelIndex,QModelIndex)));

    m_trackingModel = new TreeModel();
    m_trackingModel->setColumnCount(3);
    headers.clear();
    headers << tr("Title") << tr("Erstellt von") << tr("Erstellt am");
    m_trackingModel->setHeaders(headers);
    ui->trackingView->setModel(m_trackingModel);
    ui->trackingView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->trackingView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->trackingView->setColumnWidth(0, 300);
    connect(ui->trackingView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onTrackingSelectionChanged(QModelIndex,QModelIndex)));

    m_trackingDetails = new TrackingDetails();
    ui->horizontalLayout->addWidget(m_trackingDetails);

    /// TODO: connect(m_trackingDetails, SIGNAL(trackingDataChanged()), this, SLOT(updateTrackingModel()));

    updateSystemModel();
}

TrackingWidget::~TrackingWidget()
{
    delete ui;
}

void TrackingWidget::onSystemSelectionChanged(const QModelIndex &curr, const QModelIndex &prev)
{
    if(m_systemsModel->item(curr))
    {
        SystemItemData *data = dynamic_cast<SystemItemData*>(m_systemsModel->item(curr)->itemData());
        if(data)
        {
            updateTrackingModel(data->ID(), data->systemVersionId());
        }
        else
            updateTrackingModel(0, 0);
    }
    else
        updateTrackingModel(0, 0);
}

void TrackingWidget::onTrackingSelectionChanged(const QModelIndex &curr, const QModelIndex &prev)
{
    m_trackingDetails->setDisabled(true);
    if(m_trackingModel->item(curr))
    {
        TrackingData *data = dynamic_cast<TrackingData*>(m_trackingModel->item(curr)->itemData());
        if(data)
        {
            m_trackingDetails->setEnabled(true);
            m_trackingDetails->setTrackingData(data);
        }
    }
}

void TrackingWidget::updateSystemModel()
{
    SystemsWidget::fillSystemModel(m_systemsModel, false);
}

void TrackingWidget::updateTrackingModel(int systemId, int systemVersionId)
{
    m_trackingDetails->setDisabled(true);
    m_trackingModel->clearModel();
    QString q = "SELECT * from change_tracking  ";
            q+= " WHERE version = " + QString::number(systemVersionId) +" order by date desc;";

    QSqlQuery query(q, Global::i()->db());
    while(query.next())
    {
        TrackingData *trackingData = new TrackingData(query.record());
        TreeItem *trackingItem = new TreeItem(trackingData);
        m_trackingModel->addItem(trackingItem, m_trackingModel->rootItem(), m_trackingModel->rootItem()->childCount());
    }
    ui->trackingView->expandAll();
}

void TrackingWidget::updateTrackingModel()
{
    QModelIndex index = ui->sysView->selectionModel()->currentIndex();
    int systemVersionId =0;
    if(m_systemsModel->item(index))
        systemVersionId = dynamic_cast<SystemItemData*>(m_systemsModel->item(index)->itemData())->systemVersionId();

    updateSystemModel();

    for(int i = 0; i < m_systemsModel->rootItem()->childCount(); i++)
    {
        SystemItemData *sysData = dynamic_cast<SystemItemData*>(m_systemsModel->rootItem()->child(i)->itemData());
        if(systemVersionId == sysData->systemVersionId())
        {
            if(m_systemsModel->index(m_systemsModel->rootItem()->child(i)).isValid())
            {
                ui->sysView->setCurrentIndex(m_systemsModel->index(m_systemsModel->rootItem()->child(i)));
                break;
            }
        }
    }

}

void TrackingWidget::newChange()
{
    NewTracking tracking;
    tracking.exec();
    ui->sysView->clearSelection();
    ui->trackingView->clearSelection();
    updateSystemModel();
}
