#ifndef NEWSYSTEMUNIT_H
#define NEWSYSTEMUNIT_H

#include <QDialog>

namespace Ui {
class NewSystemUnit;
}

class NewSystemUnit : public QDialog
{
    Q_OBJECT

public:
    explicit NewSystemUnit(QWidget *parent = 0);
    ~NewSystemUnit();

protected slots:
    void onSave();

    void onSystemChanged(int index);
private:
    Ui::NewSystemUnit *ui;
};

#endif // NEWSYSTEMUNIT_H
