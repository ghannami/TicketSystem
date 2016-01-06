#include "FilterObject.h"

FilterObject::FilterObject()
{
    setToUserID(0);
    setStateID(0);
    setTypeID(0);
    setPriorityID(0);
}

int FilterObject::toUserID() const
{
    return m_toUserID;
}

void FilterObject::setToUserID(int toUserID)
{
    m_toUserID = toUserID;
}

int FilterObject::stateID() const
{
    return m_stateID;
}

void FilterObject::setStateID(int stateID)
{
    m_stateID = stateID;
}

int FilterObject::typeID() const
{
    return m_typeID;
}

void FilterObject::setTypeID(int typeID)
{
    m_typeID = typeID;
}

int FilterObject::priorityID() const
{
    return m_priorityID;
}

void FilterObject::setPriorityID(int priorityID)
{
    m_priorityID = priorityID;
}

int FilterObject::projectID() const
{
    return m_projectID;
}

void FilterObject::setProjectID(int projectID)
{
    m_projectID = projectID;
}

int FilterObject::categorieID() const
{
    return m_categorieID;
}

void FilterObject::setCategorieID(int categorieID)
{
    m_categorieID = categorieID;
}

