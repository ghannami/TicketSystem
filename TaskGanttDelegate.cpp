#include "TaskGanttDelegate.h"
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDate>
#include "PTaskItemData.h"
#include "TreeItem.h"
#include "ProjectItemData.h"

TaskGanttDelegate::TaskGanttDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void TaskGanttDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QStyleOptionViewItem opt = option;
    QRect rec1 = option.rect.adjusted(0,20,0,0);
    QRect rec2 = option.rect.adjusted(0,0,0,-20);
    opt.rect = rec1;
    TreeItem * item = static_cast<TreeItem*>(index.internalPointer());

    QColor bg(192, 179, 188, 20);
    if(dynamic_cast<ProjectItemData*>(item->itemData()))
        bg.setAlpha(40);

    AbstractTaskData *data = dynamic_cast<AbstractTaskData*>(item->itemData());

    painter->fillRect(rec1, bg);
    if(index.column() == 0)
    {
        if(data)
            if(data->taskType() == 10)
                opt.font.setBold(true);
        QStyledItemDelegate::paint(painter, opt, index);
    }
    else if(index.column() > 0)
    {
        if (option.state & QStyle::State_Selected)
            painter->fillRect(rec1, option.palette.highlight());
        if(data)
        {
            painter->setPen(Qt::NoPen);
            int colWeek = index.column();
            int startWeek = data->taskStart().weekNumber();
            int endWeek = data->taskEnd().weekNumber();
            if(data->taskType() == 2 /*Planung*/ || data->taskType() == 10 /*Projekt*/)
            {
                if(colWeek >= startWeek && colWeek <= endWeek)
                {
                    if(data->taskType() == 2)
                        painter->setBrush(QColor(231, 76, 60, 150));
                    else
                        painter->setBrush(QColor(231, 76, 60));

                    painter->drawRect(rec1.adjusted(0,3,0,-3));
                    if(startWeek == colWeek)
                    {
                        painter->setPen(Qt::white);
                        painter->setBrush(Qt::white);
                        QString perc = QString::number(data->percent())+"%";
                        painter->drawText(rec1.adjusted(3,3,0,-3),perc);

                        painter->setPen(QColor(231, 76, 60));
                        painter->setBrush(QColor(231, 76, 60));
                        painter->drawText(rec2.adjusted(3,3,0,-3), data->taskStart().toString("dd.MM"));

                    }
                    if(endWeek == colWeek)
                    {
                        painter->setPen(QColor(231, 76, 60));
                        painter->setBrush(QColor(231, 76, 60));
                        painter->drawText(rec2.adjusted(3,3,0,-3), data->taskEnd().toString("dd.MM"));
                    }
                }
            }
            else if(data->taskType() == 1)
            {
                if(startWeek == colWeek)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(Qt::NoBrush);
                    painter->drawPixmap(rec1, QPixmap(":/icon/icons/milestone.png"));
                    painter->setPen(QColor(46, 96, 148));
                    painter->setBrush(QColor(46, 96, 148));
                    painter->drawText(rec2.adjusted(3,3,0,-3), data->taskStart().toString("dd.MM"));
                }
            }
            else if(data->taskType() == 3)
            {
                if(startWeek == colWeek)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(Qt::NoBrush);
                    painter->drawPixmap(rec1, QPixmap(":/icon/icons/termin.png"));
                    painter->setPen(QColor(117,207,182));
                    painter->setBrush(QColor(117,207,182));
                    painter->drawText(rec2.adjusted(3,3,0,-3), data->taskStart().toString("dd.MM"));
                }
            }
            int currWeek = QDate::currentDate().weekNumber();
            if(currWeek == colWeek)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(142, 68, 173, 150));
                painter->drawRect(option.rect.adjusted(10,0,-10,0));
            }
        }
    }
    painter->restore();
}

QSize TaskGanttDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 0)
    {
        QStyleOptionViewItem opt = option;
        opt.font.setBold(true);
        return QStyledItemDelegate::sizeHint(opt, index);
    }
    else
        return QSize(30,40);
}
