#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "TicketsWidget.h"
#include "ProjectTasksWidget.h"
#include <QtWidgets>
#include "TicketNotifier.h"
#include "Global.h"
#include "SystemsWidget.h"
#include "CustomersWidget.h"
#include "TrackingWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    createActions();
    createTrayIcon();

    m_tickets = new TicketsWidget();
    QHBoxLayout *tLay = new QHBoxLayout();
    tLay->addWidget(m_tickets);
    ui->tickets->setLayout(tLay);

    m_tasks = new ProjectTasksWidget();
    QHBoxLayout *pLay = new QHBoxLayout();
    pLay->addWidget(m_tasks);
    ui->projects->setLayout(pLay);

    connect(m_tasks, SIGNAL(projectAdded()), m_tickets, SLOT(setupeBoxes()));

    connect(TicketNotifier::i(), SIGNAL(showMessage(QString,QString,int))
            , this, SLOT(showMessage(QString,QString,int)));
    TicketNotifier::i()->loadLastTicket();
    TicketNotifier::i()->loadUnreadTickets();

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    SystemsWidget *sysWidget = new SystemsWidget();
    QHBoxLayout *sLay = new QHBoxLayout();
    sLay->addWidget(sysWidget);
    ui->systems->setLayout(sLay);

    TrackingWidget *trackingWidget = new TrackingWidget();
    QHBoxLayout *trackingLay = new QHBoxLayout();
    trackingLay->addWidget(trackingWidget);
    ui->tracking->setLayout(trackingLay);

    /// TODO: TabWidget
    CustomersWidget *cstm = new CustomersWidget();

    setStartWithSystem(true);

    ui->mainToolBar->addAction(QIcon(":/icon/icons/graph.png"),tr("Neus Projekt"), m_tasks, SLOT(newProject()));
    ui->mainToolBar->addAction(QIcon(":/icon/icons/clipboard.png"),tr("Neues Ticket"), m_tickets, SLOT(newTicket()));
    ui->mainToolBar->addAction(QIcon(":/icon/icons/clock.png"),tr("Neuer Task"), m_tasks, SLOT(newTask()));
    ui->mainToolBar->addAction(QIcon(":/icon/icons/file.png"),tr("Neuer Testfall"), sysWidget, SLOT(newSystemUnit()));
    ui->mainToolBar->addAction(QIcon(":/icon/icons/person.png"),tr("Neuer Kunde"), cstm, SLOT(newCustomer()));
    ui->mainToolBar->addAction(QIcon(":/icon/icons/change.png"),tr("Neue Änderung"), trackingWidget, SLOT(newChange()));

    connect(cstm, SIGNAL(customerAdded()), m_tickets, SLOT(setupeBoxes()));
    connect(sysWidget, SIGNAL(ticketCreated()), m_tickets, SLOT(updateModel()));

    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Verkleinern"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Vergrößern"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("Anzeigen"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("Verlassen"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/icon/icons/clipboard.png"));
    trayIcon->show();
}

void MainWindow::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        wakeUpWindow();
}

void MainWindow::showMessage(const QString &title, const QString &msg, int ms)
{
    trayIcon->showMessage(title, msg, QSystemTrayIcon::Information, ms);
}

void MainWindow::messageClicked()
{
    wakeUpWindow();
}

void MainWindow::wakeUpWindow()
{
    setWindowState(windowState() & ~Qt::WindowMinimized);
    raise();
    activateWindow();
    showMaximized();
}

void MainWindow::setStartWithSystem(bool v)
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                       QSettings::NativeFormat);
    if(v)
    {
        QString file(qApp->applicationFilePath());
        file.replace("/","\\");
        settings.setValue("TicketSystemStart", "\""+file+"\" -d");
    }
    else
    {
        settings.remove("TicketSystemStart");
    }
#endif
}
