#ifndef TICKETMODEL_H
#define TICKETMODEL_H
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QSqlDatabase>

class TicketItem;
class FilterObject;

class TicketModel : public QAbstractItemModel
{
public:
    TicketModel(QObject *parent = 0);
    virtual ~TicketModel();

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant data ( TicketItem * item, int column, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    bool setData (TicketItem * item, int column, const QVariant & value, int role = Qt::EditRole );

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    void setHeaders(QStringList headers);
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    TicketItem *item(QModelIndex index) const;
    void addItem(TicketItem *child);
    void removeItem(TicketItem *item);

    QModelIndex parent(const QModelIndex &index) const;

    void updateModel();
    std::shared_ptr<FilterObject> filterObject() const;

    QModelIndex index(int ticketID) const;

private:
    QList<TicketItem *> m_items;
    QStringList m_hHeaders;     /// Horizontal Header Liste
    std::shared_ptr<FilterObject> m_filterObject;
};

#endif // TICKETMODEL_H

