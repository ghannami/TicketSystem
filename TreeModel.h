#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>

class TreeItem;
class AddTreeItemCommand;
class QUndoStack;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class AddTreeItemCommand;
    friend class RemoveTreeItemCommand;

public:
    TreeModel(QObject *parent = 0);
    virtual ~TreeModel();

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    QVariant data ( TreeItem * treeItem, int column, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    bool setData (TreeItem * treeItem, int column, const QVariant & value, int role = Qt::EditRole );

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(TreeItem * item);
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setRoot(TreeItem * root);
    TreeItem * rootItem();
    void setRootView(TreeItem * root); /// !< \brief Das root Element für die Anzeige,
                                        /// ab welchem Knoten das Model den Baum darstellen soll
    TreeItem * rootItemView() const;

    TreeItem * item(QModelIndex index) const;

    void addItem(TreeItem *child, TreeItem *parentItem, int position, bool undoable = false);
    void removeItem(TreeItem *treeItem, bool undoable = false);
    void removeItems(QList<TreeItem *> treeItems, bool undoable = false);
    void deleteItem(TreeItem *treeItem);
    void clearModel();              /// !< \brief löscht alle TreeItem s unter dem rootItem
    void changeParent(TreeItem * item, TreeItem *newParent, int position);

    void setHeaders(QStringList headers, Qt::Orientation orientation = Qt::Horizontal);
    void setColumnCount(int count);

    TreeItem * findItem(QMap<int, QVariant> keys, int role);   /// gibt den ersten gefundenen Item mit dem Wert value zurück,
    /// daher muss dieser Wert eindeutig sein wie z. B ID

    QUndoStack *undoStack() const;
    void updateLayout();

protected slots:
    void onBeginInsertRows(const QModelIndex &parent, int first, int last);
    void onEndInsertRows();

    void onBeginRemoveRows(const QModelIndex &parent, int first, int last);
    void onEndRemoveRows();

protected:
    void insertItem(TreeItem *child, TreeItem *parentItem, int position);
    void detachItem(TreeItem *treeItem, TreeItem *parentItem);

private:
    TreeItem * m_rootItem;         /// Das root Element für die Baumstruktur des Models
    TreeItem * m_rootItemView;     /// Das root Element für die Anzeige,
                                    /// ab welchem Knoten das Model den Baum darstellen soll
    QStringList m_hHeaders;     /// Horizontal Header Liste
    QStringList m_vHeaders;     /// Vertical Header Liste
    int m_columnCount;
    QUndoStack *m_undoStack;
};
#endif // TREEMODEL_H
