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

protected slots:
    void onLogin();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
