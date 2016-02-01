#include "AddTreeItemCommand.h"
#include "TreeModel.h"
#include "TreeItem.h"

AddTreeItemCommand::AddTreeItemCommand(TreeModel *model, TreeItem *item, TreeItem *parentItem, int position) :
    QUndoCommand()
{
    m_model = model;
    m_item = item;
    m_parentItem = parentItem;
    m_position = position;
}

int AddTreeItemCommand::id() const
{
    return 1;
}

void AddTreeItemCommand::undo()
{
    m_model->detachItem(m_item, m_parentItem);
}

void AddTreeItemCommand::redo()
{
    m_model->insertItem(m_item, m_parentItem, m_position);
}

bool AddTreeItemCommand::mergeWith(const QUndoCommand *other)
{
    // kein mergen erlaubt
    return false;
}
