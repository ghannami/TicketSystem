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

private:
    Ui::ProjectTasksWidget *ui;
    TreeModel *m_projectsModel;
    TreeModel *m_tasksModel;
    TreeItem *m_rootItem;
};

#endif // PROJECTTASKSWIDGET_H
