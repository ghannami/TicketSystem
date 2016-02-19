#ifndef TREEITEMDATA_H
#define TREEITEMDATA_H

#include <QVariant>

class TreeItemData
{
public:
    void setData(int column, const QVariant & value, int role);
    QVariant data(int column, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index);

protected:
    /*!
     * \brief setValue hier wird das setzen der Werte von den Unterklasse implementiert
     * \param column
     * \param value
     * \param role
     * \return true wenn das setzen erfolgreich war, sonst übernimmt diese Klasse das setzen
     */
    virtual bool setValue(int column, const QVariant & value, int role) = 0;
    /*!
     * \brief value hier wird das Zurückgeben der Werte von den Unterklasse implementiert
     * \param column
     * \param role
     * \return
     */
    virtual QVariant value(int column, int role) = 0;

private:
    QMap<QPair<int, int>, QVariant> m_additionalData;   /// < <column, role>, value> für zusätzliche roles ausser EditRole und DisplayRole
};

#endif // TREEITEMDATA_H
