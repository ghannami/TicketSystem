/*!
 *##########################################################
 * \class   TrackingWidget
 * \package
 * \author  Aziz Ghannami
 * \date    16.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef TRACKINGWIDGET_H
#define TRACKINGWIDGET_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class TrackingWidget;
}

class TreeModel;
class TrackingDetails;

class TrackingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrackingWidget(QWidget *parent = 0);
    ~TrackingWidget();

protected slots:
    void onSystemSelectionChanged(const QModelIndex &curr, const QModelIndex &prev);
    void onTrackingSelectionChanged(const QModelIndex &curr, const QModelIndex &prev);
    void updateSystemModel();
    void updateTrackingModel(int systemId, int systemVersionId);
    void updateTrackingModel();
    void newChange();

private:
    Ui::TrackingWidget *ui;
    TreeModel *m_systemsModel;
    TreeModel *m_trackingModel;
    TrackingDetails *m_trackingDetails;
};

#endif // TRACKINGWIDGET_H
