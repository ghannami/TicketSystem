#ifndef NEWTICKET_H
#define NEWTICKET_H

#include <QDialog>

namespace Ui {
class NewTicket;
}

class NewTicket : public QDialog
{
    Q_OBJECT

public:
    explicit NewTicket(QWidget *parent = 0);
    ~NewTicket();

protected slots:
    void onSave();

protected:
    void setupeBoxes();

private:
    Ui::NewTicket *ui;
};

#endif // NEWTICKET_H
