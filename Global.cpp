#include "Global.h"
#include "LoginWidget.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QVariant>
#include <QFile>
#include <QDomDocument>

Global *Global::m_instance = 0;

Global::Global()
{
    connectDB();

    users(true);
    types(true);
    projects(true);
    stats(true);
    categories(true);
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
    QDomDocument doc("DBSettings");
    QFile file("settings.xml");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement().firstChildElement("db");

    m_db = QSqlDatabase::addDatabase("QMYSQL", "TicketSystem");
    m_db.setHostName(root.firstChildElement("host").text());
    m_db.setDatabaseName(root.firstChildElement("database").text());
    m_db.setUserName(root.firstChildElement("user").text());
    m_db.setPassword(root.firstChildElement("password").text());
    m_db.setPort(root.firstChildElement("port").text().toInt());
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

QMap<int, QString> Global::stats(bool reload)
{
    if(reload)
    {
        m_stats.clear();
        QSqlQuery query("SELECT * FROM state order by id asc", db());
        while(query.next())
        {
            m_stats.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_stats;
}

QMap<int, QString> Global::users(bool reload)
{
    if(reload)
    {
        m_users.clear();
        QSqlQuery query("SELECT * FROM user order by id asc;", db());
        while(query.next())
        {
            m_users.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_users;
}

QMap<int, QString> Global::projects(bool reload)
{
    if(reload)
    {
        m_projects.clear();
        QSqlQuery query("SELECT * FROM projects order by id asc", db());
        while(query.next())
        {
            m_projects.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_projects;
}

QMap<int, QString> Global::types(bool reload)
{
    if(reload)
    {
        m_types.clear();
        QSqlQuery query("SELECT * FROM tickettype order by id asc", db());
        while(query.next())
        {
            m_types.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_types;
}

QMap<int, QString> Global::categories(bool reload)
{
    if(reload)
    {
        m_categories.clear();
        QSqlQuery query("SELECT * FROM categories order by id asc", db());
        while(query.next())
        {
            m_categories.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_categories;
}

QMap<int, QString> Global::prioritys(bool reload)
{
    if(reload)
    {
        m_prioritys.clear();
        QSqlQuery query("SELECT * FROM priority order by number asc", db());
        while(query.next())
        {
            m_prioritys.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_prioritys;
}

void Global::setUserName(const QString &userName)
{
    m_userName = userName;
}

