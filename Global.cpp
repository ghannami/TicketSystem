#include "Global.h"
#include "LoginWidget.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QVariant>

Global *Global::m_instance = 0;

Global::Global()
{
    connectDB();
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

void Global::setUserName(const QString &userName)
{
    m_userName = userName;
}

