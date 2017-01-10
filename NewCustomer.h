/*!
 *##########################################################
 * \class   NewCustomer
 * \package
 * \author  Aziz Ghannami
 * \date    10.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef NEWCUSTOMER_H
#define NEWCUSTOMER_H

#include <QDialog>

namespace Ui {
class NewCustomer;
}

class NewCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit NewCustomer(QWidget *parent = 0);
    ~NewCustomer();

protected slots:
    void onSave();

private:
    Ui::NewCustomer *ui;
};

#endif // NEWCUSTOMER_H
