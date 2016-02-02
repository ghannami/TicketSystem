#include "ProjectTasksWidget.h"
#include "ui_ProjectTasksWidget.h"
#include "TreeModel.h"
#include <QSqlQuery>
#include "Global.h"
#include "TreeItem.h"
#include "ProjectItemData.h"
#include "PTaskItemData.h"
#include <QTimer>
#include "TaskGanttDelegate.h"

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
    ui->projects->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->projects->horizontalHeader()->setStretchLastSection(true);
    ui->projects->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->projects->verticalHeader()->setDefaultSectionSize(20);

    connect(ui->projects->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onPrjSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->update, SIGNAL(clicked(bool)), this, SLOT(refreshModel()));

    m_tasksModel = new TreeModel();
    m_tasksModel->setColumnCount(54);
    QStringList headers;
    headers << "Tasks";
    for(int i = 1; i < 54; i++)
        headers << QString::number(i);

    m_tasksModel->setHeaders(headers);
    m_tasksModel->setRoot(m_rootItem);

    ui->tasks->setModel(m_tasksModel);
    ui->tasks->horizontalHeader()->setVisible(true);
    ui->tasks->verticalHeader()->setVisible(false);
    ui->tasks->setAutoScroll(true);
    ui->tasks->setItemDelegate(new TaskGanttDelegate());
    ui->tasks->setShowGrid(false);
    ui->tasks->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tasks->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    ui->tasks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tasks->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tasks->horizontalHeader()->setStretchLastSection(true);
    ui->tasks->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tasks->verticalHeader()->setDefaultSectionSize(20);

    connect(ui->tasks->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onTaskSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->tasks->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateEditWidgets()));
    connect(ui->saveTask, SIGNAL(clicked(bool)), this, SLOT(onSaveTask()));
    refreshModel();
    resizeDateColumns(30);
    updateEditWidgets();
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
    QString tq = "SELECT  tsk.id as id, tsk.name as name, tsk.task_start as task_start, tsk.task_end as task_end, ";
    tq +=" tsk.order as task_order, ";
    tq +=" tp.name as task_type, tsk.project_id as project_id ";
    tq += " FROM project_task tsk, project_task_type tp where tsk.project_task_type = tp.id order by tsk.order asc;";
    QSqlQuery tQuery(tq, Global::i()->db());
    while(tQuery.next())
    {
        int pId = tQuery.record().value("project_id").toInt();
        if(prjItems.contains(pId))
            m_projectsModel->addItem(new TreeItem(new PTaskItemData(tQuery.record())), prjItems.value(pId), prjItems.value(pId)->childCount());
    }

    if(!prjItems.isEmpty())
        ui->projects->selectionModel()->setCurrentIndex(m_projectsModel->index(prjItems.values().first()), QItemSelectionModel::SelectCurrent);
}

void ProjectTasksWidget::onPrjSelectionChanged(const QModelIndex &curr, const QModelIndex &prev)
{
    if(m_projectsModel->item(curr))
    {
        m_tasksModel->setRootView(m_projectsModel->item(curr));
        m_tasksModel->updateLayout();
    }
}

void ProjectTasksWidget::onTaskSelectionChanged(const QModelIndex &curr, const QModelIndex &prev)
{
    if(m_tasksModel->item(curr))
    {
        PTaskItemData *data = dynamic_cast<PTaskItemData*>(m_tasksModel->item(curr)->itemData());
        if(data)
        {
            ui->startDate->setDate(data->taskStart());
            ui->endDate->setDate(data->taskEnd());
            ui->taskName->setText(data->taskName());
            ui->taskOrder->setValue(data->order());
        }
    }
}

void ProjectTasksWidget::onSaveTask()
{
    QModelIndex cIndex = ui->tasks->selectionModel()->currentIndex();
    if(m_tasksModel->item(cIndex))
    {
        PTaskItemData *data = dynamic_cast<PTaskItemData*>(m_tasksModel->item(cIndex)->itemData());
        if(data)
        {
            data->setTaskName(ui->taskName->text());
            data->setTaskStart(ui->startDate->date());
            data->setTaskEnd(ui->endDate->date());
            data->setOrder(ui->taskOrder->value());
            data->saveToDB();
            refreshModel();
        }
    }
}

void ProjectTasksWidget::updateEditWidgets()
{
    bool enable = ui->tasks->selectionModel()->hasSelection();
    ui->taskDetailsWidget->setEnabled(enable);
}

void ProjectTasksWidget::resizeDateColumns(int width)
{
    for(int i = 1; i < 54; i++)
        ui->tasks->setColumnWidth(i, width);
}
