#include "mfacturacionemitida.h"

#include "eregistroplugins.h"
#include <QDebug>

MFacturacionEmitida::MFacturacionEmitida(bool deudor, QObject *parent) :
    QSqlQueryModel(parent)
{
    this->setHeaderData( 0, Qt::Horizontal, "Razon Social" );
    this->setHeaderData( 1, Qt::Horizontal, "#Comprobante" );
    this->setHeaderData( 2, Qt::Horizontal, "Total" );
    _id_servicio = -1;
    _id_periodo_servicio = -1;
    _deudor = deudor;
}

void MFacturacionEmitida::setearServicio( int s ) { _id_servicio = s; }
void MFacturacionEmitida::setearPeriodo( int p ) { _id_periodo_servicio = p; }

QVariant MFacturacionEmitida::data( const QModelIndex &item, int role ) const
{
    switch( role )
    {
        case Qt::DisplayRole:
        {
            switch ( item.column() )
            {
                case 2:
                {
                    return QString( "$ %L1" ).arg( QSqlQueryModel::data( item, role ).toDouble(), 10, 'f', 2 );
                    break;
                }
            }
        }
    }
    return QSqlQueryModel::data( item, role );
}

/*!
 * \fn MFacturacionEmitida::generarDatos()
 * Genera la consulta correcta según el plugin de hicomp cargado o no mas los parametros pasados
 */
void MFacturacionEmitida::generarDatos()
{
    QString cola = "SELECT c.razon_social,";
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
        if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {
            cola.append( "  CONCAT( CONCAT( LPAD( r.serie, 5, 0 ), '-' ), LPAD( r.numero, 5, 0 ) ) AS NumComp, " );
        } else if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {
            cola.append( "  substr( '0000000000' || r.serie, -5, 5 ) || '-' || substr( '0000000000' || r.numero, -5, 5 ) AS NumComp, " );
        } else {
            qDebug( "Driver desconocido para formar campo concatenado" );
        }
        cola.append( " r.precio "
                     " FROM `cobro_servicio_cliente_periodo` AS cscp, `clientes` AS c, `recibos` AS r "
                     " WHERE cscp.id_cliente = c.id "
                     " AND cscp.id_factura = r.id_recibo " );
    } else {
        if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {
            cola.append( "  CONCAT( CONCAT( LPAD( f.serie, 5, 0 ), '-' ), LPAD( f.numero, 5, 0 ) ) AS NumComp, " );
        } else if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {
            cola.append( "  substr( '0000000000' || f.serie, -5, 5 ) || '-' || substr( '0000000000' || f.numero, -5, 5 ) AS NumComp, " );
        } else {
            qDebug( "Driver desconocido para formar campo concatenado" );
        }
        cola.append( " f.total "
                     "FROM `cobro_servicio_cliente_periodo` AS cscp, `clientes` AS c, `factura` AS f "
                     "WHERE cscp.id_cliente = c.id "
                     " AND cscp.id_factura = f.id_factura " );
    }
    if( _id_servicio > 0 ) {
        cola.append( QString( " AND cscp.id_servicio = %1 " ).arg( _id_servicio ) );
    }
    if( _id_periodo_servicio > 0 ) {
        cola.append( QString( " AND cscp.id_periodo_servicio = %1 " ).arg( _id_periodo_servicio ) );
    }
    if( _deudor ) {
        cola.append( " AND cscp.id_recibo IS NULL" );
    } else {
        cola.append( " AND cscp.id_recibo IS NOT NULL " );
    }
    cola.append( "  ORDER BY razon_social" );
    setQuery( cola );
    qDebug() << cola;
    this->setHeaderData( 0, Qt::Horizontal, "Razon Social" );
    this->setHeaderData( 1, Qt::Horizontal, "#Comprobante" );
    this->setHeaderData( 2, Qt::Horizontal, "Total" );
}
