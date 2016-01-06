#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QItemSelection>

class TicketModel;
class TicketDetails;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void updateModel();
    void refreshModel();
    void onFilterChanged();
    void newTicket();
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

protected:
    void setupeBoxes();

private:
    Ui::MainWindow *ui;
    TicketModel *m_model;
    TicketDetails *m_ticketDetails;
};

#endif // MAINWINDOW_H
