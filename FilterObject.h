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

    int systemVersionID() const;
    void setSystemVersionID(int systemVersionID);

    int unitCategorieID() const;
    void setUnitCategorieID(int unitCategorieID);

    int customerID() const;
    void setCustomerID(int customerID);

private:
    int m_toUserID;
    int m_stateID;
    int m_typeID;
    int m_priorityID;
    int m_systemVersionID;
    int m_unitCategorieID;
    int m_customerID;
};

#endif // FILTEROBJECT_H
