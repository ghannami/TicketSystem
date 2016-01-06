#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QSqlDatabase>

class Global
{
private:
    Global();
public:
    static Global *instance();
    static Global *i();
    bool login();
    void setUserName(const QString &userName);
    QString userName() const;
    void connectDB();
    QSqlDatabase db();


    int userID() const;
    void setUserID(int userID);

private:
    static Global *m_instance;
    QString m_userName;
    QSqlDatabase m_db;
    int m_userID;
};

#endif // GLOBAL_H
