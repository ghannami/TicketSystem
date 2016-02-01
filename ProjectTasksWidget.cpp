#include "ProjectTasksWidget.h"
#include "ui_ProjectTasksWidget.h"
#include "TreeModel.h"
#include <QSqlQuery>
#include "Global.h"
#include "TreeItem.h"
#include "ProjectItemData.h"
#include "PTaskItemData.h"
#include <QTimer>

ProjectTasksWidget::ProjectTasksWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectTasksWidget)
{
    ui->setupUi(this);
    m_rootItem = new TreeItem(0);

    m_projectsModel = new TreeModel();
    m_projectsModel->setHeaders(QStringList()<<"Projekt");
    m_projectsModel->setRoot(m_rootItem);

    ui->projects->setModel(m_projectsModel);
    ui->projects->horizontalHeader()->setVisible(true);
    ui->projects->verticalHeader()->setVisible(false);
    ui->projects->setAutoScroll(true);

    ui->projects->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->projects->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->projects->horizontalHeader()->setStretchLastSection(true);
    ui->projects->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->projects->verticalHeader()->setDefaultSectionSize(20);

    m_tasksModel = new TreeModel();
    m_tasksModel->setHeaders(QStringList()<<"Tasks");
    m_tasksModel->setRoot(m_rootItem);

    ui->tasks->setModel(m_tasksModel);
    ui->tasks->horizontalHeader()->setVisible(true);
    ui->tasks->verticalHeader()->setVisible(false);
    ui->tasks->setAutoScroll(true);

    ui->tasks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tasks->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tasks->horizontalHeader()->setStretchLastSection(true);
    ui->tasks->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tasks->verticalHeader()->setDefaultSectionSize(20);

    refreshModel();
}

ProjectTasksWidget::~ProjectTasksWidget()
{
    delete ui;
    if(m_projectsModel)
        delete m_projectsModel;
}

void ProjectTasksWidget::refreshModel()
{
    m_projectsModel->clearModel();
    m_tasksModel->clearModel();
    QString pq = "SELECT * FROM projects;";
    QMap<int, TreeItem*> prjItems;
    QSqlQuery pQuery(pq, Global::i()->db());
    while(pQuery.next())
    {
        TreeItem *prj = new TreeItem(new ProjectItemData(pQuery.record()));
        m_projectsModel->addItem(prj, m_rootItem, m_rootItem->childCount());
        prjItems.insert(pQuery.value("id").toInt(), prj);
    }

    QString tq = "SELECT  tsk.id as id, tsk.name as name, tsk.task_start as task_start, tsk.task_end as task_end, tp.name as task_type, tsk.project_id as project_id ";
    tq += " FROM project_task tsk, project_task_type tp where tsk.project_task_type = tp.id;";
    QSqlQuery tQuery(tq, Global::i()->db());
    while(tQuery.next())
    {
        int pId = tQuery.record().value("project_id").toInt();
        if(prjItems.contains(pId))
            m_projectsModel->addItem(new TreeItem(new PTaskItemData(tQuery.record())), prjItems.value(pId), prjItems.value(pId)->childCount());
    }

    m_tasksModel->setRootView(prjItems.value(1));

    QTimer::singleShot(10000, this, SLOT(refreshModel()));
}
