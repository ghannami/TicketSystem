#ifndef NEWPTASK_H
#define NEWPTASK_H

#include <QDialog>
#include <QDate>

namespace Ui {
class NewPTask;
}

class NewPTask : public QDialog
{
    Q_OBJECT

public:
    explicit NewPTask(QWidget *parent = 0);
    ~NewPTask();

protected slots:
    void onSave();
    void onStartDateChanged(const QDate &date);

private:
    Ui::NewPTask *ui;
};

#endif // NEWPTASK_H
