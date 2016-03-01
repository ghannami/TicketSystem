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
    void setType(int typeID);
    void setUnitCategorie(int categorieID);
    void setSystemVersion(int systemVersion);
    void setTitle(const QString &title);

protected slots:
    void onSave();
    void onSystemChanged(int index);

protected:
    void setupeBoxes();

private:
    Ui::NewTicket *ui;
};

#endif // NEWTICKET_H
