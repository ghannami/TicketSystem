#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "TicketsWidget.h"
#include "ProjectTasksWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    showMaximized();
    m_tickets = new TicketsWidget();
    QHBoxLayout *tLay = new QHBoxLayout();
    tLay->addWidget(m_tickets);
    ui->tickets->setLayout(tLay);
    connect(ui->actionTicket, SIGNAL(triggered(bool)), m_tickets, SLOT(newTicket()));

    m_tasks = new ProjectTasksWidget();
    QHBoxLayout *pLay = new QHBoxLayout();
    pLay->addWidget(m_tasks);
    ui->projects->setLayout(pLay);
}

MainWindow::~MainWindow()
{
    delete ui;
}
