#include "resumencomprasproveedor.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QDebug>

#include "mcompra.h"

/*
 * SELECT COUNT( id ) as cantidad,
 *        MAX( fecha ) as fecha,
 *        strftime("%m" , max( fecha ) ) as mes,
 *        strftime( "%Y", MAX( fecha ) ) as ano,
 *        SUM( total ) as total,
 *        id_proveedor,
 *        contado
 * FROM compras
 */
ResumenComprasProveedor::ResumenComprasProveedor(QObject *parent) :
QSqlTableModel( parent )
{
    _metodo_temporal = PorMes;
    setTable( "resumen_compra" );
    setHeaderData( 0, Qt::Horizontal, "Cantidad Total" );
    setHeaderData( 1, Qt::Horizontal, QString::fromUtf8( "Fecha última" ) );
    setHeaderData( 2, Qt::Horizontal, "Mes" );
    setHeaderData( 3, Qt::Horizontal, QString::fromUtf8( "Año" ) );
    setHeaderData( 4, Qt::Horizontal, "Total" );
    setHeaderData( 5, Qt::Horizontal, "Proveedor" );
    setHeaderData( 6, Qt::Horizontal, "Contado" );
}

/*!
 * \brief ResumenComprasProveedor::setearMetodoTemporal
 * Permite cambiar el metodo de división temporal elegido
 * \param metodo Metodo de división temporal elegido
 */
void ResumenComprasProveedor::setearMetodoTemporal( DivisionTemporal metodo )
{
    this->_metodo_temporal = metodo;
    this->actualizarDatos();
    this->select();
}

/*!
 * \brief ResumenComprasProveedor::data
 * \param idx
 * \param role
 * \return
 */
QVariant ResumenComprasProveedor::data(const QModelIndex &idx, int role) const
{
    switch( role ) {
        case Qt::DisplayRole: {
            switch( idx.column() ) {
                case 0: {
                    return QSqlTableModel::data( idx, role ).toInt();
                    break;
                }
                case 1: {
                    return QSqlTableModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                case 2: {
                    // Lo convierto al mes que corresponde
                    return QDate::longMonthName( QSqlTableModel::data( idx, role ).toInt() );
                    break;
                }
                case 4: {
                    return QString( "$ %L1" ).arg( QSqlTableModel::data( idx, role ).toDouble(), 10, 'f', 2 );
                    break;
                }
                default: {
                    return QSqlTableModel::data( idx, role );
                    break;
                }
            }
            break;
        }
        case Qt::TextAlignmentRole: {
            switch( idx.column() ) {
                case 0:
                case 1:
                case 2:
                case 3: {
                    return int( Qt::AlignCenter | Qt::AlignVCenter );
                    break;
                }
                case 4: {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                    break;
                }
                default: {
                    return QSqlTableModel::data( idx, role );
                    break;
                }
            }
            break;
        }
        default: {
            return QSqlTableModel::data( idx, role );
            break;
        }
    }
}

/**
 * @brief ResumenComprasProveedor::cambioProveedor
 * Permite filtrar los elementos según un proveedor elegido
 * @param id_proveedor ID del proveedor
 */
void ResumenComprasProveedor::cambiarProveedor(int id_proveedor)
{
    if( id_proveedor > 0 ) {
        _filtro.clear();
        _filtro.append( QString( " WHERE id_proveedor = %1" ).arg( id_proveedor ) );
        actualizarDatos();
    }
    this->setFilter( _filtro );
    this->select();
    qDebug() << this->filter();
}

/*!
 * \brief ResumenComprasProveedor::actualizarDatos
 * Actualiza la consulta para que agrupe según el modelo indicado
 */
void ResumenComprasProveedor::actualizarDatos()
{
    QString groupBy;
    groupBy.clear();
    groupBy.append( " GROUP BY id_proveedor" );
    if( _metodo_temporal == SinDivision ) {
        // No separo nada
    } else if( _metodo_temporal == PorMes ||
               _metodo_temporal == PorBimestre ||
               _metodo_temporal == PorCuatrimestre ||
               _metodo_temporal == PorSeximestre ) {
        groupBy.append( ", mes, ano " );
    } else if( _metodo_temporal == PorAno ) {
        groupBy.append( ", ano " );
    }
    _filtro.append( groupBy );
}

