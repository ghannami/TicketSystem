#include "Global.h"
#include "LoginWidget.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QVariant>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QSqlError>
#include <QDir>
#include <QApplication>

Global *Global::m_instance = 0;

Global::Global()
{
    connectDB();

    loadDBValues();
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
    if(w.loggedIn())
        return true;

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
    if(m_db.open())
        return;
    QMessageBox msg;

    QDomDocument doc("DBSettings");
    QFile file(settingsFile());

    if (!file.open(QIODevice::ReadOnly))
    {
        msg.setText(qApp->applicationDirPath());
        msg.exec();
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        msg.setText("!doc.setContent(&file)");
        msg.exec();
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
    if(!m_db.open())
    {
        msg.setText(m_db.lastError().text());
        msg.exec();
    }
}

QSqlDatabase Global::db()
{
    return m_db;
}

void Global::loadDBValues()
{
    users(true);
    ticketTypes(true);
    projects(true);
    stats(true);
    taskTypes(true);
    prioritys(true);
    systems(true);
    systemVersions(true);
    unitCategories(true);
    customers(true);
    trackingTypes(true);
    trackingBehavior(true);
    departments(true);
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

QMap<int, QString> Global::ticketTypes(bool reload)
{
    if(reload)
    {
        m_ticketTypes.clear();
        QSqlQuery query("SELECT * FROM tickettype order by id asc", db());
        while(query.next())
        {
            m_ticketTypes.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_ticketTypes;
}

QMap<int, QString> Global::taskTypes(bool reload)
{
    if(reload)
    {
        m_taskeTypes.clear();
        QSqlQuery query("SELECT * FROM project_task_type order by id asc", db());
        while(query.next())
        {
            m_taskeTypes.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_taskeTypes;
}

QMap<int, QString> Global::systems(bool reload)
{
    if(reload)
    {
        m_systems.clear();
        QSqlQuery query("SELECT * from system", db());
        while(query.next())
        {
            m_systems.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_systems;
}

QMap<int, QString> Global::systemVersions(bool reload)
{
    if(reload)
    {
        m_systemsVersions.clear();
        QSqlQuery query("select sv.id as id, s.name as system_name, v.version as version, sv.revision as revision from system s, system_version sv, version v where s.id = sv.system and v.id = sv.version order by sv.date desc ", db());
        while(query.next())
        {
            m_systemsVersions.insert(query.value("id").toInt(), query.value("system_name").toString() + " " + query.value("version").toString()+" rev." + query.value("revision").toString());
        }
    }
    return m_systemsVersions;
}

QMap<int, QString> Global::systemUnitCategories(int systemId)
{
    m_systemsUnitCategories.clear();
    QSqlQuery query("SELECT * FROM system_unit_categorie WHERE system = "+QString::number(systemId) + " order by position asc;", db());
    while(query.next())
    {
        m_systemsUnitCategories.insert(query.value("id").toInt(), query.value("name").toString());
    }
    return m_systemsUnitCategories;
}

QMap<int, QString> Global::unitCategories(bool reload)
{
    if(reload)
    {
        m_unitCategories.clear();
        QSqlQuery query("SELECT * FROM system_unit_categorie ", db());
        while(query.next())
        {
            m_unitCategories.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_unitCategories;
}

QString Global::appPath()
{
    return qApp->applicationDirPath();
}

QString Global::settingsFile()
{
    QDir dir(appPath());
    return dir.filePath("settings.xml");
}

QString Global::dateTimeFormat() const
{
    return "yyyy-MM-dd HH:mm:ss";
}

QMap<int, QString> Global::departments(bool reload)
{
    if(reload)
    {
        m_department.clear();
        QSqlQuery query("SELECT * FROM department", db());
        while(query.next())
        {
            m_department.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_department;
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

QMap<int, QString> Global::customers(bool reload)
{
    if(reload)
    {
        m_customers.clear();
        QSqlQuery query("SELECT * FROM customer ", db());
        while(query.next())
        {
            m_customers.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_customers;
}

QMap<int, QString> Global::trackingTypes(bool reload)
{
    if(reload)
    {
        m_trackingTypes.clear();
        QSqlQuery query("SELECT * FROM tracking_type order by id asc", db());
        while(query.next())
        {
            m_trackingTypes.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_trackingTypes;
}

QMap<int, QString> Global::trackingBehavior(bool reload)
{
    if(reload)
    {
        m_trackingBehavior.clear();
        QSqlQuery query("SELECT * FROM tracking_behavior order by id asc", db());
        while(query.next())
        {
            m_trackingBehavior.insert(query.value("id").toInt(), query.value("name").toString());
        }
    }
    return m_trackingBehavior;
}

void Global::setUserName(const QString &userName)
{
    m_userName = userName;
}
