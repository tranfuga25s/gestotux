#include "ecbproductosfilter.h"

#include "ecbproductosmodel.h"

#include <QDebug>

ECBProductosFilter::ECBProductosFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    _mostrar_sin_stock = true;
    _mostrar_deshabilitados = true;
    _id_proveedor = -1;
}

/*!
 * \brief ECBProductosFilter::setearNoMostrarProductosSinStock
 * Define si se filtran o no los productos sin stock
 * \param estado estado deseado
 */
void ECBProductosFilter::setearNoMostrarProductosSinStock( bool estado )
{
    _mostrar_sin_stock = !estado;
    this->invalidate();
}

/*!
 * \brief ECBProductosFilter::setearNoMostrarProductosDeshabilitados
 * especifica si se mostrarán o ocultarán los productos deshabilitados
 * \param estado estado deseado
 */
void ECBProductosFilter::setearNoMostrarProductosDeshabilitados(bool estado)
{
    _mostrar_deshabilitados = !estado;
    this->invalidate();
}

/*!
 * \brief ECBProductosFilter::setearIdProveedor
 * Setea sobre que proveedor se filtrará
 * \param id_proveedor ID del proveedor o < 0 para cualquier proveedor
 */
void ECBProductosFilter::setearIdProveedor(int id_proveedor)
{
    _id_proveedor = id_proveedor;
    this->invalidate();
}

/*!
 * \brief ECBProductosFilter::buscarPorCodigo
 * \param buscar
 * \return
 */
int ECBProductosFilter::buscarPorCodigo(const QString buscar)
{
    QModelIndex indicem = mapFromSource( sourceModel()->index( modeloFuente()->buscarPorCodigo( buscar ), 0 ) );
    return indicem.row();
}

/*!
 * \brief ECBProductosFilter::getListaIDs
 * Devuelve la lista de IDs
 * \return QList<int> *
 */
QList<int> *ECBProductosFilter::getListaIDs()
{
    return modeloFuente()->getListaIDs();
}

/*!
 * \brief ECBProductosFilter::agregarItem
 * Agrega el elemento temporal mapeado al modelo actual
 * \param texto Nombre de producto
 * \param stock Stock actual
 * \param habilitado Habilitado o no
 * \param proveedor ID proveedor
 * \return Posición del elemento en el modelo actual
 */
int ECBProductosFilter::agregarItem(const QString texto, double stock, bool habilitado, int proveedor)
{
    int nuevo_id = modeloFuente()->agregarItem( texto, stock, habilitado, proveedor );
    QModelIndex indice = sourceModel()->index( modeloFuente()->obtenerPosicionSegunId( nuevo_id ), 0 );
    this->invalidateFilter(); // Es necesario ya que se reinicializa el modelo base
    QModelIndex indicem = mapFromSource( indice );
    return indicem.row();
}

/*!
 * \brief ECBProductosFilter::modeloFuente
 * \return
 */
ECBProductosModel *ECBProductosFilter::modeloFuente()
{
    return qobject_cast<ECBProductosModel *>(QSortFilterProxyModel::sourceModel());
}

/*!
 * \brief ECBProductosFilter::filterAcceptsRow
 * Permite definir si la columna utiliza se puede mostrar o no según los datos seleccionados de filtrado
 * \param source_row ID de columna de modelo de origen
 * \param source_parent Padre
 * \return verdadero o falso
 */
bool ECBProductosFilter::filterAcceptsRow( int source_row, const QModelIndex & ) const
{
    if( !_mostrar_sin_stock ) {
        if( sourceModel()->data( sourceModel()->index( source_row, ECBProductosModel::Stock ) ).toDouble() <= 0.0 )
        { return false; }
    }
    if( !_mostrar_deshabilitados ) {
        if( ! sourceModel()->data( sourceModel()->index( source_row, ECBProductosModel::Habilitado ) ).toBool() )
        { return false; }
    }
    if( _id_proveedor > 0 ) {
        if( sourceModel()->data( sourceModel()->index( source_row, ECBProductosModel::IdProveedor ) ).toInt() != _id_proveedor )
        { return false; }
    }
    return true;
}
