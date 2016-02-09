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
#include "NewPTask.h"
#include "NewProject.h"
#include <QMessageBox>

ProjectTasksWidget::ProjectTasksWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectTasksWidget)
{
    ui->setupUi(this);

    connect(ui->update, SIGNAL(clicked(bool)), this, SLOT(refreshModel()));

    m_tasksModel = new TreeModel();
    m_tasksModel->setColumnCount(54);
    QStringList headers;
    headers << "Tasks \\ KW";
    for(int i = 1; i < 54; i++)
        headers << QString::number(i);

    m_tasksModel->setHeaders(headers);

    ui->tasks->setModel(m_tasksModel);
    ui->tasks->setAutoScroll(true);
    ui->tasks->setItemDelegate(new TaskGanttDelegate());
    ui->tasks->header()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tasks->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    ui->tasks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tasks->setSelectionMode(QAbstractItemView::SingleSelection);

    QPalette p = ui->tasks->palette();
    QColor sc = p.highlight().color();
    sc.setAlpha(150);
    p.setColor(QPalette::Highlight, sc);
    ui->tasks->setPalette(p);
    connect(ui->tasks->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onTaskSelectionChanged(QModelIndex,QModelIndex)));
    connect(ui->tasks->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateEditWidgets()));
    connect(ui->saveTask, SIGNAL(clicked(bool)), this, SLOT(onSaveTask()));
    connect(ui->removeTask, SIGNAL(clicked(bool)), this, SLOT(removeTask()));


    connect(ui->description, SIGNAL(textChanged()), this, SLOT(onValuesChanged()));
    connect(ui->endDate, SIGNAL(dateChanged(QDate)), this, SLOT(onValuesChanged()));
    connect(ui->startDate, SIGNAL(dateChanged(QDate)), this, SLOT(onValuesChanged()));
    connect(ui->percent, SIGNAL(valueChanged(int)), this, SLOT(onValuesChanged()));
    connect(ui->taskName, SIGNAL(textChanged(QString)), this, SLOT(onValuesChanged()));
    connect(ui->taskOrder, SIGNAL(valueChanged(int)), this, SLOT(onValuesChanged()));

    int i = 1;
    QMapIterator<int, QString> mip(Global::i()->taskTypes());
    while(mip.hasNext())
    {
        mip.next();
        ui->type->insertItem(i++, mip.value(),mip.key());
    }
    refreshModel();
    resizeDateColumns(30);
    updateEditWidgets();
    m_blockEmitChanges = false;

}

ProjectTasksWidget::~ProjectTasksWidget()
{
    delete ui;
    if(m_tasksModel)
        delete m_tasksModel;
}

void ProjectTasksWidget::refreshModel()
{
    m_tasksModel->clearModel();
    ui->tasks->selectionModel()->clear();
    ui->tasks->clearSelection();
    QString pq = "SELECT * FROM projects order by project_order asc;";
    QMap<int, TreeItem*> prjItems;
    QSqlQuery pQuery(pq, Global::i()->db());
    while(pQuery.next())
    {
        TreeItem *prj = new TreeItem(new ProjectItemData(pQuery.record()));
        m_tasksModel->addItem(prj, m_tasksModel->rootItem(), m_tasksModel->rootItem()->childCount());
        prjItems.insert(pQuery.value("id").toInt(), prj);
    }
    QString tq = "SELECT  tsk.id as id, tsk.name as name, tsk.task_start as task_start, tsk.task_end as task_end, ";
    tq += " tsk.task_order as task_order, tsk.percent as percent, tsk.description as description, tsk.project_task_type as project_task_type, ";
    tq += " tsk.date as date, ";
    tq += " tp.name as task_type, tsk.project_id as project_id ";
    tq += " FROM project_task tsk, project_task_type tp where tsk.project_task_type = tp.id order by tsk.task_order asc;";
    QSqlQuery tQuery(tq, Global::i()->db());
    while(tQuery.next())
    {
        int pId = tQuery.record().value("project_id").toInt();
        if(prjItems.contains(pId))
        {
            PTaskItemData *taskData = new PTaskItemData(tQuery.record());
            ProjectItemData *projectData = dynamic_cast<ProjectItemData *>(prjItems.value(pId)->itemData());
            if(taskData->taskStart() < projectData->taskStart() || !projectData->taskStart().isValid())
                projectData->setTaskStart(taskData->taskStart());
            if(taskData->taskEnd() > projectData->taskEnd() || !projectData->taskEnd().isValid())
                projectData->setTaskEnd(taskData->taskEnd());
            if(taskData->taskType() == 2)
            {
                if(projectData->percent() < 0)
                    projectData->setPercent(taskData->percent());
                else
                {
                    double x1 = projectData->percent();
                    double x2 = taskData->percent();
                    projectData->setPercent(100 * (x1+x2)/200);
                }
            }
            m_tasksModel->addItem(new TreeItem(taskData), prjItems.value(pId), prjItems.value(pId)->childCount());
        }
    }

    ui->tasks->expandAll();
    ui->tasks->resizeColumnToContents(0);
}

void ProjectTasksWidget::onTaskSelectionChanged(const QModelIndex &curr, const QModelIndex &prev)
{
    if(m_tasksModel->item(curr))
    {
        PTaskItemData *data = dynamic_cast<PTaskItemData*>(m_tasksModel->item(curr)->itemData());
        if(data)
        {
            m_blockEmitChanges = true;
            ui->createDate->setDate(data->date());
            ui->startDate->setDate(data->taskStart());
            ui->endDate->setDate(data->taskEnd());
            ui->taskName->setText(data->taskName());
            ui->taskOrder->setValue(data->order());
            ui->percent->setValue(data->percent());
            ui->description->setPlainText(data->description());

            for(int index = 0; index < ui->type->count(); index++)
            {
                if(ui->type->itemData(index).toInt() == data->taskType())
                {
                    ui->type->setCurrentIndex(index);
                }
            }
            m_blockEmitChanges = false;
        }
    }
}

void ProjectTasksWidget::onSaveTask()
{
    updateItem(true);
}

void ProjectTasksWidget::removeTask()
{
    QMessageBox msg(this);
    msg.addButton(tr("Abbrechen"), QMessageBox::RejectRole);
    QPushButton *rmButton = msg.addButton(tr("Löschen"), QMessageBox::ActionRole);
    msg.setWindowTitle(tr("Task löschen!"));
    msg.setText(tr("Task löschen?"));
    msg.exec();
    if (msg.clickedButton() == rmButton)
    {
        QModelIndex cIndex = ui->tasks->selectionModel()->currentIndex();
        if(m_tasksModel->item(cIndex))
        {
            PTaskItemData *data = dynamic_cast<PTaskItemData*>(m_tasksModel->item(cIndex)->itemData());
            if(data)
            {
                data->removeFromDB();
                refreshModel();
            }
        }
    }
}

void ProjectTasksWidget::updateItem(bool save)
{
    if(m_blockEmitChanges && !save)
        return;

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
            data->setPercent(ui->percent->value());
            data->setDescription(ui->description->toPlainText());
            if(save)
            {
                data->saveToDB();
                refreshModel();
            }
            else
            {
                m_tasksModel->updateLayout();
            }
        }
    }
}

void ProjectTasksWidget::onValuesChanged()
{
    updateItem(false);
}

void ProjectTasksWidget::updateEditWidgets()
{
    bool enable = ui->tasks->selectionModel()->hasSelection();
    ui->taskDetailsWidget->setEnabled(false);
    if(enable)
    {
        QModelIndex cIndex = ui->tasks->selectionModel()->currentIndex();
        if(m_tasksModel->item(cIndex))
        {
            PTaskItemData *data = dynamic_cast<PTaskItemData*>(m_tasksModel->item(cIndex)->itemData());
            if(data)
            {
                ui->taskDetailsWidget->setEnabled(enable);
                if(data->taskType() == 1 || data->taskType() == 3)
                {
                    ui->endDate->setDisabled(true);
                    ui->percent->setDisabled(true);
                }
                else
                {
                    ui->endDate->setEnabled(true);
                    ui->percent->setEnabled(true);
                }
            }
        }
    }
}

void ProjectTasksWidget::newTask()
{
    NewPTask task;
    task.exec();
    refreshModel();
}

void ProjectTasksWidget::newProject()
{
    NewProject prj;
    if(prj.exec() == QDialog::Accepted)
    {
        refreshModel();
        emit projectAdded();
    }
}

void ProjectTasksWidget::resizeDateColumns(int width)
{
    for(int i = 1; i < 54; i++)
        ui->tasks->setColumnWidth(i, width);
}
