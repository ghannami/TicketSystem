#ifndef TASKGANTTDELEGATE_H
#define TASKGANTTDELEGATE_H

#include <QStyledItemDelegate>

class TaskGanttDelegate : public QStyledItemDelegate
{
public:
    TaskGanttDelegate(QObject *parent = 0);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

};

#endif // TASKGANTTDELEGATE_H
