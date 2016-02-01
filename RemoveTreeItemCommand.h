#ifndef REMOVETREEITEMCOMMAND_H
#define REMOVETREEITEMCOMMAND_H

#include <QUndoCommand>
class TreeModel;
class TreeItem;

class RemoveTreeItemCommand : public QUndoCommand
{
public:
    explicit RemoveTreeItemCommand(TreeModel *model, TreeItem *item, TreeItem *parentItem);
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

#endif // REMOVETREEITEMCOMMAND_H
