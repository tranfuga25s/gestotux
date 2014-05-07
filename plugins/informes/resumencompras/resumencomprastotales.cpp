#include "resumencomprastotales.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QDebug>

#include "mcompra.h"

ResumenComprasTotales::ResumenComprasTotales(QObject *parent) :
QSqlQueryModel( parent )
{
    _metodo_temporal = PorMes;
    consultas.insert( "QSQLITE", "SELECT COUNT( id ) as cantidad, MAX( fecha ) as fecha, strftime( \"%m\" , fecha ) as mes, strftime( \"%Y\", fecha ) as ano, SUM( total ) as total, id_proveedor FROM compras " );
    //consultas.insert( "QMYSQL", "SELECT COUNT( id ) as cantidad, MAX( fecha ) as fecha, strftime( \"%m\" , fecha ) as mes, strftime( \"%Y\", fecha ) as ano, SUM( total ) as total, id_proveedor FROM compras " );
    setQuery( consultas.value( QSqlDatabase::database().driverName() ) );
    setHeaderData( 0, Qt::Horizontal, "Cantidad Total" );
    setHeaderData( 1, Qt::Horizontal, QString::fromUtf8( "Fecha última" ) );
    setHeaderData( 2, Qt::Horizontal, "Mes" );
    setHeaderData( 3, Qt::Horizontal, QString::fromUtf8( "Año" ) );
    setHeaderData( 4, Qt::Horizontal, "Total" );
    setHeaderData( 5, Qt::Horizontal, "Proveedor" );
    setHeaderData( 6, Qt::Horizontal, "Contado" );
}

/*!
 * \brief ResumenComprasTotales::setearMetodoTemporal
 * \param metodo
 */
void ResumenComprasTotales::setearMetodoTemporal( DivisionTemporal metodo )
{
    this->_metodo_temporal = metodo;
    this->actualizarDatos();
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
                    return QSqlQueryModel::data( idx, role ).toInt();
                    break;
                }
                case 1: {
                    return QSqlQueryModel::data( idx, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                case 2: {
                    // Lo convierto al mes que corresponde
                    return QDate::longMonthName( QSqlQueryModel::data( idx, role ).toInt() );
                    break;
                }
                case 4: {
                    return QString( "$ %L1" ).arg( QSqlQueryModel::data( idx, role ).toDouble(), 10, 'f', 2 );
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
 * \brief ResumenComprasTotales::actualizarDatos
 */
void ResumenComprasTotales::actualizarDatos()
{
    QString groupBy;
    groupBy.clear();
    if( _metodo_temporal == SinDivision ) {
        // No separo nada
    } else if( _metodo_temporal == PorMes ||
               _metodo_temporal == PorBimestre ||
               _metodo_temporal == PorCuatrimestre ||
               _metodo_temporal == PorSeximestre ) {
        groupBy = " GROUP BY 3, 4 ";
    } else if( _metodo_temporal == PorAno ) {
        groupBy = " GROUP BY 4 ";
    }
    qDebug() << consultas.value( QSqlDatabase::database().driverName() ) + groupBy;
    setQuery( consultas.value( QSqlDatabase::database().driverName() ) + groupBy );
    if( this->lastError().isValid() ) {
        qDebug() << "Error al ejecutar la cola de resumen: " << this->query().lastQuery();
    }
}
