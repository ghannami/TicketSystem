#ifndef SYSTEMSWIDGET_H
#define SYSTEMSWIDGET_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class SystemsWidget;
}
class TreeModel;

class SystemsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemsWidget(QWidget *parent = 0);
    ~SystemsWidget();

public slots:
    void newSystemUnit();

protected slots:
    void onSystemSelectionChanged(const QModelIndex &curr, const QModelIndex &prev);
    void onTestsSelectionChanged(QModelIndex,QModelIndex);
    void updateSystemModel();
    void updateUnitsModel(int systemId, int systemVersionId);

private:
    Ui::SystemsWidget *ui;
    TreeModel *m_systemsModel;
    TreeModel *m_unitsModel;
};

#endif // SYSTEMSWIDGET_H
