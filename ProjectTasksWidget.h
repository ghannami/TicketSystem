#ifndef PROJECTTASKSWIDGET_H
#define PROJECTTASKSWIDGET_H

#include <QWidget>

class TreeModel;
class TreeItem;

namespace Ui {
class ProjectTasksWidget;
}

class ProjectTasksWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectTasksWidget(QWidget *parent = 0);
    ~ProjectTasksWidget();

protected slots:
    void refreshModel();
//    void onPrjSelectionChanged(const QModelIndex &curr, const QModelIndex &prev);
    void onTaskSelectionChanged(const QModelIndex &curr, const QModelIndex &prev);
    void onSaveTask();
    void removeTask();

    void updateEditWidgets();
    void newTask();
    void newProject();

    void updateItem(bool save = false);
    void onValuesChanged();

signals:
    void projectAdded();

protected:
    void resizeDateColumns(int width);
private:
    Ui::ProjectTasksWidget *ui;
//    TreeModel *m_projectsModel;
    TreeModel *m_tasksModel;
//    TreeItem *m_rootItem;
    bool m_blockEmitChanges;
};

#endif // PROJECTTASKSWIDGET_H
