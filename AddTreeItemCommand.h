#ifndef ADDTREEITEMCOMMAND_H
#define ADDTREEITEMCOMMAND_H

#include <QUndoCommand>
class TreeModel;
class TreeItem;

class AddTreeItemCommand : public QUndoCommand
{
public:
    explicit AddTreeItemCommand(TreeModel *model, TreeItem *item, TreeItem *parentItem, int position);
    virtual int id() const;
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand *other);

private:
    TreeModel *m_model;
    TreeItem  *m_parentItem;
    TreeItem  *m_item;
    int m_position;
};

#endif // ADDTREEITEMCOMMAND_H
