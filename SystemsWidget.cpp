#include "SystemsWidget.h"
#include "ui_SystemsWidget.h"
#include "TreeModel.h"
#include <QSqlQuery>
#include "Global.h"
#include "SystemItemData.h"
#include "TreeItem.h"
#include "NewSystemUnit.h"
#include "SystemUnitData.h"
#include "SystemUnitCatData.h"

SystemsWidget::SystemsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemsWidget)
{
    ui->setupUi(this);

    m_systemsModel = new TreeModel();
    m_systemsModel->setColumnCount(3);
    QStringList headers;
    headers << tr("System") << tr("Version") << tr("Datum") << tr("Revision") ;
    m_systemsModel->setHeaders(headers);

    ui->sysView->setModel(m_systemsModel);
    ui->sysView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->sysView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sysView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->sysView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onSystemSelectionChanged(QModelIndex,QModelIndex)));


    m_unitsModel = new TreeModel();
    m_unitsModel->setColumnCount(3);
    headers.clear();
    headers << tr("Testfall") << tr("Getestet von") << tr("Getestet am");
    m_unitsModel->setHeaders(headers);
    ui->testView->setModel(m_unitsModel);
    ui->testView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->testView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->testView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onTestsSelectionChanged(QModelIndex,QModelIndex)));


    updateSystemModel();
}

SystemsWidget::~SystemsWidget()
{
    delete ui;
}

void SystemsWidget::newSystemUnit()
{
    NewSystemUnit unit;
    unit.exec();
    ui->sysView->clearSelection();
    ui->testView->clearSelection();
    updateSystemModel();
}

void SystemsWidget::onSystemSelectionChanged(const QModelIndex &curr, const QModelIndex &prev)
{
    if(m_systemsModel->item(curr))
    {
        SystemItemData *data = dynamic_cast<SystemItemData*>(m_systemsModel->item(curr)->itemData());
        if(data)
        {
            updateUnitsModel(data->ID(), data->systemVersionId());
        }
        else
            updateUnitsModel(0, 0);
    }
    else
        updateUnitsModel(0, 0);

}

void SystemsWidget::onTestsSelectionChanged(QModelIndex, QModelIndex)
{

}

void SystemsWidget::updateSystemModel()
{
    m_systemsModel->clearModel();
    m_unitsModel->clearModel();
    QString q = "SELECT s.id as id, s.name as name, v.version as version, sv.date as date, sv.id as system_version FROM system_version sv, system s, version v ";
            q+= " WHERE sv.system = s.id AND sv.version = v.id ";
            q+= "order by sv.date desc;";

    QSqlQuery query(q, Global::i()->db());
    while(query.next())
    {
        TreeItem *sys = new TreeItem(new SystemItemData(query.record()));
        m_systemsModel->addItem(sys, m_systemsModel->rootItem(), m_systemsModel->rootItem()->childCount());
    }
}

void SystemsWidget::updateUnitsModel(int systemId, int systemVersionId)
{
    m_unitsModel->clearModel();
    QMap<int, TreeItem*> cats;
    QMap<int, SystemUnitData *> units;

    QString cq = "SELECT * from system_unit_categorie  ";
            cq+= " WHERE system = " + QString::number(systemId) +" ";
            cq+= "order by position asc;";

    QSqlQuery cQuery(cq, Global::i()->db());
    while(cQuery.next())
    {
        SystemUnitCatData *unitCatData = new SystemUnitCatData(cQuery.record());
        TreeItem *unitItem = new TreeItem(unitCatData);
        cats.insert(unitCatData->id(), unitItem);
        m_unitsModel->addItem(unitItem, m_unitsModel->rootItem(), m_unitsModel->rootItem()->childCount());
    }

    QString uq = "SELECT * from system_unit  ";
            uq+= " WHERE system = " + QString::number(systemId) +" ";
            uq+= "order by position asc;";

    QSqlQuery uQuery(uq, Global::i()->db());
    while(uQuery.next())
    {
        SystemUnitData *unitData = new SystemUnitData(uQuery.record());
        TreeItem *unit = new TreeItem(unitData);
        units.insert(unitData->id(), unitData);
        if(cats.contains(unitData->categorieId()))
            m_unitsModel->addItem(unit, cats.value(unitData->categorieId()), cats.value(unitData->categorieId())->childCount());
    }


    QString utq = " SELECT su.id as id, sut.id as unit_test_id, su.position as position, sut.test_state as test_state, s.id as system, ";
            utq+= " sut.user as user, su.categorie as categorie, sv.version as version, sv.id as system_version, su. title as title, sut.date as date ";
            utq+= " from system_unit_test sut, system_unit su, system_version sv, system s ";
            utq+= " WHERE sut.system_version = sv.id AND sut.system_unit = su.id AND su.system = s.id ";
            utq+= " AND s.id = " + QString::number(systemId) +" AND sv.id = "+ QString::number(systemVersionId);
            utq+= " order by position asc;";

    QSqlQuery utQuery(utq, Global::i()->db());
    while(utQuery.next())
    {
        SystemUnitData *unitData = new SystemUnitData(utQuery.record());
        if(units.contains(unitData->id()))
        {
            units.value(unitData->id())->setRecord(utQuery.record());
        }
    }

    ui->testView->expandAll();
}
