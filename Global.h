#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QSqlDatabase>
#include <QMap>

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

    QMap<int, QString> stats() const;

    QMap<int, QString> users() const;

    QMap<int, QString> projects() const;

    QMap<int, QString> types() const;

    QMap<int, QString> categories() const;

    QMap<int, QString> prioritys() const;

private:
    static Global *m_instance;
    QString m_userName;
    QSqlDatabase m_db;
    int m_userID;
    QMap<int, QString> m_stats;
    QMap<int, QString> m_users;
    QMap<int, QString> m_projects;
    QMap<int, QString> m_types;
    QMap<int, QString> m_categories;
    QMap<int, QString> m_prioritys;
};

#endif // GLOBAL_H
