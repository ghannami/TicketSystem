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

    int departmentID() const;
    void setDepartmentID(int departmentID);

    int fromUserID() const;
    void setFromUserID(int fromUserID);

private:
    int m_toUserID;
    int m_fromUserID;
    int m_stateID;
    int m_typeID;
    int m_priorityID;
    int m_systemVersionID;
    int m_unitCategorieID;
    int m_customerID;
    int m_departmentID;
};

#endif // FILTEROBJECT_H
