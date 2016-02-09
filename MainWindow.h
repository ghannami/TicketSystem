#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class TicketsWidget;
class ProjectTasksWidget;
class TicketNotifier;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;
    void wakeUpWindow();

    void setStartWithSystem(bool v);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage(const QString &title, const QString &msg, int ms);
    void messageClicked();

private:
    void createActions();
    void createTrayIcon();

private:
    Ui::MainWindow *ui;
    TicketsWidget *m_tickets;
    ProjectTasksWidget *m_tasks;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};

#endif // MAINWINDOW_H
