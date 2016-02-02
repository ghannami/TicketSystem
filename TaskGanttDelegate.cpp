#include "TaskGanttDelegate.h"
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDate>

TaskGanttDelegate::TaskGanttDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void TaskGanttDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    if(index.column() == 0)
        QStyledItemDelegate::paint(painter, option, index);
    else if(index.column() > 0)
    {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, option.palette.highlight());
        int weekDay = index.data().toInt();
        painter->setPen(Qt::NoPen);
        if(weekDay > 0)
        {
            painter->setBrush(QColor(231, 76, 60, 200));
            painter->drawRect(option.rect.adjusted(0,3,0,-3));

        }
        int currWeek = QDate::currentDate().weekNumber();
        if(currWeek == index.column())
        {
            painter->setBrush(QColor(142, 68, 173, 150));
            painter->drawRect(option.rect.adjusted(10,0,-10,0));
        }
    }
    painter->restore();
}

QSize TaskGanttDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 0)
        return QStyledItemDelegate::sizeHint(option, index);
    else
        return QSize(30,30);
}
