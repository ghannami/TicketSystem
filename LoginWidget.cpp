#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "Global.h"
#include <QCryptographicHash>
#include <QVariant>
#include <QSqlQuery>
#include <QDomDocument>
#include <QtWidgets>
#include <QSqlError>

LoginWidget::LoginWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    m_loggedIn = false;
    connect(ui->login, SIGNAL(clicked(bool)), this, SLOT(onLogin()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(reject()));

    QDomDocument doc("DBSettings");
    QFile file(Global::i()->settingsFile());
    file.open(QIODevice::ReadOnly);
    doc.setContent(&file);
    file.close();
    QDomElement root = doc.documentElement().firstChildElement("login");
    onLogin(root.firstChildElement("user").text(), root.firstChildElement("user").text());
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

QString LoginWidget::userName()
{
    return ui->username->text();
}

QString LoginWidget::password()
{
    return ui->password->text();
}

void LoginWidget::onLogin(QString user, QString pwd)
{
    if(user.isEmpty())
        user = userName();
    if(pwd.isEmpty())
        pwd = password();

    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(pwd.toLatin1());
    QString hashPwd = hash.result().toHex();
    QSqlQuery query("SELECT * FROM user WHERE name = '" + user + "';", Global::i()->db());
    if(query.next())
    {
        if(hashPwd.compare(query.value("pwd").toString(), Qt::CaseSensitive ) == 0)
        {
            Global::i()->setUserName(query.value("name").toString());
            Global::i()->setUserID(query.value("id").toInt());
            m_loggedIn = true;

            QDomDocument doc("DBSettings");
            QFile file("settings.xml");
            file.open(QIODevice::ReadOnly);
            doc.setContent(&file);
            file.close();
            QDomElement root = doc.documentElement().firstChildElement("login");
            if(root.isNull())
            {
                root = doc.createElement("login");
                doc.documentElement().appendChild(root);
            }
            QDomElement userElm = root.firstChildElement("user");
            if(userElm.isNull())
            {
                userElm = doc.createElement("user");
                root.appendChild(userElm);
            }
            QDomText uTxt = userElm.firstChild().toText();
            if(uTxt.isNull())
            {
                uTxt = doc.createTextNode(user);
                userElm.appendChild(uTxt);
            }
            else
                uTxt.setNodeValue(user);
            QDomElement pwdElm = root.firstChildElement("pwd");
            if(pwdElm.isNull())
            {
                pwdElm = doc.createElement("pwd");
                root.appendChild(pwdElm);
            }
            QDomText pTxt = pwdElm.firstChild().toText();
            if(pTxt.isNull())
            {
                pTxt = doc.createTextNode(user);
                pwdElm.appendChild(pTxt);
            }
            else
                pTxt.setNodeValue(pwd);

            QFile outFile ("settings.xml");
            outFile.open(QIODevice::WriteOnly);
            QTextStream out(&outFile);
            out<<doc.toString();

            return accept();
        }
    }
    else
    {
        ui->error->setText(query.lastError().text());
    }
    ui->userNameLabel->setStyleSheet("color:red");
    ui->pwdLabel->setStyleSheet("color:red");
}

bool LoginWidget::loggedIn() const
{
    return m_loggedIn;
}
