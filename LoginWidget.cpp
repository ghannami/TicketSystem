#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "Global.h"
#include <QCryptographicHash>
#include <QVariant>
#include <QSqlQuery>

LoginWidget::LoginWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    connect(ui->login, SIGNAL(clicked(bool)), this, SLOT(onLogin()));
    connect(ui->abort, SIGNAL(clicked(bool)), this, SLOT(reject()));
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

void LoginWidget::onLogin()
{
    QString user = userName();
    QString pwd = password();
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
            return accept();
        }
    }
    ui->userNameLabel->setStyleSheet("color:red");
    ui->pwdLabel->setStyleSheet("color:red");
}
