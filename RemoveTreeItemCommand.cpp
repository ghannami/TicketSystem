#include "RemoveTreeItemCommand.h"
#include "TreeModel.h"
#include "TreeItem.h"

RemoveTreeItemCommand::RemoveTreeItemCommand(TreeModel *model, TreeItem *item, TreeItem *parentItem) :
    QUndoCommand()
{
    m_model = model;
    m_item = item;
    m_parentItem = parentItem;
    m_position = parentItem->childRow(item);
}

int RemoveTreeItemCommand::id() const
{
    return 2;
}

void RemoveTreeItemCommand::undo()
{
    m_model->insertItem(m_item, m_parentItem, m_position);
}

void RemoveTreeItemCommand::redo()
{
    m_model->detachItem(m_item, m_parentItem);
}

bool RemoveTreeItemCommand::mergeWith(const QUndoCommand *other)
{
    return false;
}
