#include "resumencomprastotales.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QDebug>

#include "mcompra.h"

ResumenComprasTotales::ResumenComprasTotales(QObject *parent) :
QSqlTableModel( parent )
{
    _metodo_temporal = PorMes;
    setTable( "resumen_compra" );
    setHeaderData( 0, Qt::Horizontal, "Cantidad Total" );
    setHeaderData( 1, Qt::Horizontal, "Fecha" );
    setHeaderData( 2, Qt::Horizontal, "Mes" );
    setHeaderData( 3, Qt::Horizontal, QString::fromUtf8( "Año" ) );
    setHeaderData( 4, Qt::Horizontal, "Total" );
    setHeaderData( 5, Qt::Horizontal, "Proveedor" );
    setHeaderData( 6, Qt::Horizontal, "Contado" );
    this->removeColumn( this->fieldIndex( "id_proveedor" ) );
    /*this->removeColumn( this->fieldIndex( "contado" ) );
    this->removeColumn( this->fieldIndex( "fecha" ) );*/
}

/*!
 * \brief ResumenComprasTotales::setearMetodoTemporal
 * \param metodo
 */
void ResumenComprasTotales::setearMetodoTemporal( DivisionTemporal metodo )
{
    this->_metodo_temporal = metodo;
    this->actualizarDatos();
    this->select();
}

/*!
 * \brief ResumenComprasTotales::data
 * \param idx
 * \param role
 * \return
 */
QVariant ResumenComprasTotales::data(const QModelIndex &idx, int role) const
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
        default: {
            return QSqlTableModel::data( idx, role );
            break;
        }
    }
}

/*!
 * \brief ResumenComprasTotales::actualizarDatos
 */
void ResumenComprasTotales::actualizarDatos()
{
    QString groupBy;
    groupBy.clear();
    if( _metodo_temporal == ResumenComprasTotales::SinDivision ) {
        // No separo nada
    } else if( _metodo_temporal == ResumenComprasTotales::PorMes ||
               _metodo_temporal == ResumenComprasTotales::PorBimestre ||
               _metodo_temporal == ResumenComprasTotales::PorCuatrimestre ||
               _metodo_temporal == ResumenComprasTotales::PorSeximestre ) {
        groupBy = " GROUP BY MONTH( fecha ), YEAR( fecha ) ";
    } else if( _metodo_temporal == ResumenComprasTotales::PorAno ) {
        groupBy = " GROUP BY YEAR( fecha ) ";
    }
    this->setFilter( groupBy );
}
