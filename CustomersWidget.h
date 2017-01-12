/*!
 *##########################################################
 * \class   CustomersWidget
 * \package
 * \author  Aziz Ghannami
 * \date    10.1.2017
 * \brief
 *
 *##########################################################
 * Copyright (C) 2017 CAM CONCEPT. All rights reserved.
 *##########################################################
 */
#ifndef CUSTOMERSWIDGET_H
#define CUSTOMERSWIDGET_H

#include <QWidget>

namespace Ui {
class CustomersWidget;
}

class CustomersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomersWidget(QWidget *parent = 0);
    ~CustomersWidget();

protected slots:
    void newCustomer();

signals:
    void customerAdded();

private:
    Ui::CustomersWidget *ui;
};

#endif // CUSTOMERSWIDGET_H
