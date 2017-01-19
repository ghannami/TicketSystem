/*!
 *##########################################################
 * \class   NewTracking
 * \package
 * \author  Aziz Ghannami
 * \date    16.1.2017
 * \brief
 *
 *##########################################################
 */
#ifndef NEWTRACKING_H
#define NEWTRACKING_H

#include <QDialog>

namespace Ui {
class NewTracking;
}

class NewTracking : public QDialog
{
    Q_OBJECT

public:
    explicit NewTracking(QWidget *parent = 0);
    ~NewTracking();

protected slots:
    void onSave();

private:
    Ui::NewTracking *ui;
};

#endif // NEWTRACKING_H
