#ifndef FILTEROBJECT_H
#define FILTEROBJECT_H


class FilterObject
{
public:
    FilterObject();

    int toUserID() const;
    void setToUserID(int toUserID);

    int stateID() const;
    void setStateID(int stateID);

    int typeID() const;
    void setTypeID(int typeID);

    int priorityID() const;
    void setPriorityID(int priorityID);

    int projectID() const;
    void setProjectID(int projectID);

    int categorieID() const;
    void setCategorieID(int categorieID);

private:
    int m_toUserID;
    int m_stateID;
    int m_typeID;
    int m_priorityID;
    int m_projectID;
    int m_categorieID;
};

#endif // FILTEROBJECT_H
