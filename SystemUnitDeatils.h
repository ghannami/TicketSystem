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

private:
    Ui::SystemUnitDeatils *ui;
    SystemUnitData *m_systemUnitData;
};

#endif // SYSTEMUNITDEATILS_H
