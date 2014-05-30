#ifndef ECBPRODUCTOSFILTER_H
#define ECBPRODUCTOSFILTER_H

#include <QSortFilterProxyModel>

#include "ecbproductosmodel.h"

class ECBProductosFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ECBProductosFilter(QObject *parent = 0);
    void setearNoMostrarProductosSinStock( bool estado );
    void setearNoMostrarProductosDeshabilitados( bool estado );
    void setearIdProveedor( int id_proveedor );
    bool mostrarProductosDeshabilitados() { return _mostrar_deshabilitados; }
    bool mostrarProductosSinStock() { return _mostrar_sin_stock; }

    int buscarPorCodigo( const QString buscar );
    QList<int> *getListaIDs();

    int agregarItem( const QString texto, double stock = 1.0, bool habilitado = true, int proveedor = 0 );
    ECBProductosModel *modeloFuente();

protected:
    bool filterAcceptsRow( int source_row, const QModelIndex &source_parent = QModelIndex() ) const;

private:
    bool _mostrar_sin_stock;
    bool _mostrar_deshabilitados;
    int _id_proveedor;
        
};

#endif // ECBPRODUCTOSFILTER_H
