#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QDialog>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    QString userName();
    QString password();

    bool loggedIn() const;

protected slots:
    void onLogin(QString user = QString(""), QString pwd = QString(""));

private:
    Ui::LoginWidget *ui;
    bool m_loggedIn;
};

#endif // LOGINWIDGET_H
