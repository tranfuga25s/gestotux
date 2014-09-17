#ifndef MUNIDADESPRODUCTOSARBOL_H
#define MUNIDADESPRODUCTOSARBOL_H

#include <QProxyModel>
class MUnidadesProductos;

class MUnidadesProductosArbol : public QProxyModel
{
    Q_OBJECT
public:
    MUnidadesProductosArbol( QObject *parent = 0 );

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;

    bool hasChildren(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    void fetchMore(const QModelIndex &) {}

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    /**
     * @brief _adb
     * Mapa del elemento con la base de datos
     */
    QHash<int, QHash<int, int> > _adb;

    MUnidadesProductos *mup;
    
};

#endif // MUNIDADESPRODUCTOSARBOL_H
