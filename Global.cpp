#include "Global.h"
#include "LoginWidget.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QVariant>

Global *Global::m_instance = 0;

Global::Global()
{
    connectDB();

    QSqlQuery query("SELECT * FROM user order by id asc;", db());
    while(query.next())
    {
        m_users.insert(query.value("id").toInt(), query.value("name").toString());
    }

    query = QSqlQuery("SELECT * FROM tickettype order by id asc", db());
    while(query.next())
    {
        m_types.insert(query.value("id").toInt(), query.value("name").toString());
    }

    query = QSqlQuery("SELECT * FROM projects order by id asc", db());
    while(query.next())
    {
        m_projects.insert(query.value("id").toInt(), query.value("name").toString());
    }

    query = QSqlQuery("SELECT * FROM state order by id asc", db());
    while(query.next())
    {
        m_stats.insert(query.value("id").toInt(), query.value("name").toString());
    }

    query = QSqlQuery("SELECT * FROM categories order by id asc", db());
    while(query.next())
    {
        m_categories.insert(query.value("id").toInt(), query.value("name").toString());
    }

    query = QSqlQuery("SELECT * FROM priority order by number asc", db());
    while(query.next())
    {
        m_prioritys.insert(query.value("id").toInt(), query.value("name").toString());
    }
}

Global *Global::instance()
{
    if(m_instance == 0)
        m_instance = new Global();
    return m_instance;
}

Global *Global::i()
{
    return instance();
}

bool Global::login()
{
    LoginWidget w;
    if(w.exec() == QDialog::Accepted)
    {
        return true;
    }

    return false;
}

QString Global::userName() const
{
    return m_userName;
}

void Global::connectDB()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL", "TicketSystem");
    m_db.setHostName("PC-10-CC");
    m_db.setDatabaseName("ticketsystem");
    m_db.setUserName("TicketSystem");
    m_db.setPassword("ticketsystem");
    m_db.setPort(3307);
    m_db.open();
}

QSqlDatabase Global::db()
{
    return m_db;
}

int Global::userID() const
{
    return m_userID;
}

void Global::setUserID(int userID)
{
    m_userID = userID;
}

QMap<int, QString> Global::stats() const
{
    return m_stats;
}

QMap<int, QString> Global::users() const
{
    return m_users;
}

QMap<int, QString> Global::projects() const
{
    return m_projects;
}

QMap<int, QString> Global::types() const
{
    return m_types;
}

QMap<int, QString> Global::categories() const
{
    return m_categories;
}

QMap<int, QString> Global::prioritys() const
{
    return m_prioritys;
}

void Global::setUserName(const QString &userName)
{
    m_userName = userName;
}

