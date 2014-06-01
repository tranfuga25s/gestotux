#include "resumencomprasporproductos.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QDebug>

#include "mcompra.h"

ResumenComprasPorProductos::ResumenComprasPorProductos(QObject *parent) :
    QSqlQueryModel(parent)
{
    _metodo_temporal = PorMes;
    consultas.insert( "QSQLITE", "SELECT cp.id_producto as id_producto, SUM( cp.cantidad ), p.nombre AS proveedor, MAX( c.fecha ) as ultima_compra FROM compras_productos AS cp, compras AS c, proveedor AS p WHERE p.id = c.id_proveedor AND cp.id_compra = c.id " );
    //consultas.insert( "QMYSQL", "SELECT COUNT( id ) as cantidad, MAX( fecha ) as fecha, strftime( \"%m\" , fecha ) as mes, strftime( \"%Y\", fecha ) as ano, SUM( total ) as total, id_proveedor FROM compras " );
    setQuery( consultas.value( QSqlDatabase::database().driverName() ) );
    setHeaderData( 0, Qt::Horizontal, "Producto" );
    setHeaderData( 1, Qt::Horizontal, "Cantidad Total" );
    setHeaderData( 2, Qt::Horizontal, "Proveedor" );
    setHeaderData( 3, Qt::Horizontal, QString::fromUtf8( "Fecha última" ) );
    _id_producto = -1;
}

/*!
 * \brief ResumenComprasPorProductos::setearMetodoTemporal
 * \param metodo
 */
void ResumenComprasPorProductos::setearMetodoTemporal( DivisionTemporal metodo )
{
    this->_metodo_temporal = metodo;
    this->actualizarDatos();
}

/*!
 * \brief ResumenComprasPorProductos::setearIdProducto
 * \param id_producto
 */
void ResumenComprasPorProductos::setearIdProducto(const int id_producto)
{
    if( id_producto > 0 ) {
        this->_id_producto = id_producto;
    } else {
        this->_id_producto = -1;
    }
    actualizarDatos();
}

/*!
 * \brief ResumenComprasPorProductos::data
 * \param idx
 * \param role
 * \return
 */
QVariant ResumenComprasPorProductos::data(const QModelIndex &idx, int role) const
{
    switch( role ) {
        case Qt::DisplayRole: {
            switch( idx.column() ) {
                case 1: {
                    return QSqlQueryModel::data( idx, role ).toInt();
                    break;
                }
                case 3: {
                    return QSqlQueryModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                default: {
                    return QSqlQueryModel::data( idx, role );
                    break;
                }
            }
            break;
        }
        case Qt::TextAlignmentRole: {
            switch( idx.column() ) {
                case 1:
                case 2:
                case 3:
                case 4: {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
                case 5: {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                    break;
                }
                default: {
                    return QSqlQueryModel::data( idx, role );
                    break;
                }
            }
            break;
        }
        default: {
            return QSqlQueryModel::data( idx, role );
            break;
        }
    }
}

/*!
 * \brief ResumenComprasPorProductos::actualizarDatos
 */
void ResumenComprasPorProductos::actualizarDatos()
{
    QString filtro;
    if( _id_producto > 0 ) {
        filtro.append( QString( " AND cp.id_producto = %1 " ).arg( _id_producto ) );
    }
    filtro.append( " GROUP BY 1 " ); // Agrupa por numero de producto
    qDebug() << consultas.value( QSqlDatabase::database().driverName() ) + filtro;
    setQuery( consultas.value( QSqlDatabase::database().driverName() ) + filtro );
    if( this->lastError().isValid() ) {
        qDebug() << "Error al ejecutar la cola de resumen: " << this->query().lastQuery();
    }
}
