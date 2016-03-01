#ifndef SYSTEMUNITDEATILS_H
#define SYSTEMUNITDEATILS_H

#include <QWidget>

namespace Ui {
class SystemUnitDeatils;
}
class SystemUnitData;
class SystemUnitDeatils : public QWidget
{
    Q_OBJECT

public:
    explicit SystemUnitDeatils(QWidget *parent = 0);
    ~SystemUnitDeatils();

    SystemUnitData *systemUnitData() const;
    void setSystemUnitData(SystemUnitData *systemUnitData);

protected slots:
    void onTestStateChanged(int index);
    void newTicket();

signals:
    void unitDataChanged();
    void ticketCreated();

private:
    Ui::SystemUnitDeatils *ui;
    SystemUnitData *m_systemUnitData;
    bool m_blockUpdate;
};

#endif // SYSTEMUNITDEATILS_H
