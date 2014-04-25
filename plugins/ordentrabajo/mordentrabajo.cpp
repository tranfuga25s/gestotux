#include "mordentrabajo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>


MOrdenTrabajo::MOrdenTrabajo( QObject *parent ) :
QSqlRelationalTableModel( parent )
{
    setTable( "orden_trabajo" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "#Cliente" );
    setHeaderData( 2, Qt::Horizontal, "Fecha de Ingreso" );
    setHeaderData( 3, Qt::Horizontal, "Requerente" );
    setHeaderData( 4, Qt::Horizontal, "Ingresante" );
    setHeaderData( 5, Qt::Horizontal, "Fecha de Vencimiento" );
    setHeaderData( 6, Qt::Horizontal, "Fecha de Devolucion" );
    setHeaderData( 7, Qt::Horizontal, "#Equipamiento" );
    setHeaderData( 8, Qt::Horizontal, "#Factura" );
    setHeaderData( 9, Qt::Horizontal, "#Tecnico" );
    setHeaderData( 10, Qt::Horizontal, "Causa de Ingreso" );
    _id_orden_trabajo = -1;
    _data = QSqlRecord();
}

/*!
 * \brief MOrdenTrabajo::relacionarDatos
 */
void MOrdenTrabajo::relacionarDatos()
{
    setRelation( 1, QSqlRelation( "clientes"    , "id"             , "razon_social" ) );
    setRelation( 7, QSqlRelation( "equipamiento", "id_equipamiento", "descripcion"  ) );
    setRelation( 9, QSqlRelation( "tecnico"     , "id_tecnico"     , "razon_social" ) );
}

/*!
 * \brief MOrdenTrabajo::data
 * \param item
 * \param role
 * \return
 */
QVariant MOrdenTrabajo::data( const QModelIndex &item, int role ) const
{
    switch( item.column() ) {
        case 2:
        case 5:
        case 6:
        {
            return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::SystemLocaleShortDate );
            break;
        }
        default:
        {
            return QSqlRelationalTableModel::data( item, role );
            break;
        }
    }
}

/*!
 * \brief MOrdenTrabajo::tieneDatosRelacionados
 * \return
 */
bool MOrdenTrabajo::tieneDatosRelacionados( const int /* id_orden_trabajo */ )
{
    // Las ordenes de trabajo implican la eliminacion de todos los datos relacionados ( operacion_orden_trabajo )
    return false;
}

/*!
 * \brief MOrdenTrabajo::numeroComprobanteProximo
 * \return
 */
NumeroComprobante MOrdenTrabajo::numeroComprobanteProximo()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT MAX(id_orden_trabajo) FROM orden_trabajo" ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                NumeroComprobante temp( 0, 0, 1 );
                temp.deNumero( cola.record().value(0).toInt() );
                temp.siguienteNumero();
                return temp;
            }
        } else {
            return NumeroComprobante( 0, 0, 1 );
        }
    } else {
        qDebug() << "Error al intentar ejecutar la cola de averiguación de numero de comprobante proximo de una orden de trabajo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return NumeroComprobante( 0, -1, -1 );
}

/*!
 * \brief MOrdenTrabajo::numeroComprobanteSegunId
 * \param id_orden
 * \return
 */
NumeroComprobante MOrdenTrabajo::numeroComprobanteSegunId( const int id_orden )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_orden_trabajo FROM orden_trabajo WHERE id_orden_trabajo = %1" ).arg( id_orden ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                NumeroComprobante temp( 0, 0, 1 );
                temp.deNumero( cola.record().value(0).toInt() );
                return temp;
            }
        } else {
            qDebug() << "Error al intentar hacer next de la cola de averiguación de numero de comprobante de una orden de trabajo";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al intentar ejecutar la cola de averiguación de numero de comprobante segun id de una orden de trabajo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return NumeroComprobante( 0, -1, -1 );
}

/*!
 * \brief MOrdenTrabajo::idSegunNumeroComprobante
 * \param num
 * \return
 */
int MOrdenTrabajo::idSegunNumeroComprobante( NumeroComprobante num )
{
    return num.aNumero();
}

/*!
 * \brief MOrdenTrabajo::obtenerIdEquipamientoSegunId
 * Se obtiene el identificador de equiapamiento asociado con la orde pasada como parametro
 * \param id_orden Identificador de la orden
 * \return Identificador del equipamiento o -1 si hubo error
 */
int MOrdenTrabajo::obtenerIdEquipamientoSegunId( const int id_orden )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_equipamiento FROM orden_trabajo WHERE id_orden_trabajo = %1" ).arg( id_orden ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return cola.record().value(0).toInt();
            }
        } else {
            qDebug() << "Error al intentar cargar los datos de una orden de trabajo";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al intentar ejecutar la cola de averiguación de datos de una orden de trabajo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}

/*!
 * \brief MOrdenTrabajo::cambiarTecnico
 * Cambia el tecnico de una orden de trabajo.
 * \param id_orden_trabajo Identificador de orden de trabajo
 * \param id_tecnico identificador del técnico.
 * \return Verdadero si el cambio fue correcto.
 */
bool MOrdenTrabajo::cambiarTecnico(const int id_orden_trabajo, const int id_tecnico)
{
    QSqlQuery cola;
    if( cola.exec( QString( "UPDATE orden_trabajo SET id_tecnico = %1 WHERE id_orden_trabajo = %2" ).arg( id_tecnico ).arg( id_orden_trabajo ) ) ) {
        return true;
    } else {
        qDebug() << "MOrdenTrabajo::cambioTecnico(): Error al intentar ejecutar la cola de averiguación de datos de una orden de trabajo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MOrdenTrabajo::cargarDatos
 * Carga los datos de una orden de trabajo pasada como parametro.
 * \param id_orden IDentificador de la orden de trabajo a cargar.
 */
bool MOrdenTrabajo::cargarDatos( const int id_orden )
{
    if( id_orden <= 0 ) {
        qDebug() << "MOrdenTrabajo::cargarDatos(): Orden de trabajo invalida: "<< id_orden;
        return false;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT * FROM orden_trabajo WHERE id_orden_trabajo = %1" ).arg( id_orden ) ) ) {
        if( cola.next() ) {
            this->_data = cola.record();
            _id_orden_trabajo = id_orden;
            return true;
        } else {
            qDebug() << "MOrdenTrabajo::cargarDatos(): Error al intentar cargar los datos de una orden de trabajo";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "MOrdenTrabajo::cargarDatos(): Error al intentar ejecutar la cola de averiguación de datos de una orden de trabajo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MOrdenTrabajo::agregarOrdenTrabajo
 * Agrega una nueva orden de trabajo con los parametros indicados
 * \param id_cliente Identificador del cliente
 * \param id_equipamiento Identificador del equipamiento
 * \param id_tecnico Identificador del técnico
 * \param requerente Nombre del requerente
 * \param ingresante Nombre del ingresante
 * \param fecha_ingreso Fecha de ingreso de la orden
 * \param fecha_devolucion Fecha de posible devolucion
 * \param causa_ingreso Causa del ingreso
 * \return ID de al nueva orden de trabajo o -1 si hubo algún error
 */
int MOrdenTrabajo::agregarOrdenTrabajo( const int id_cliente, const int id_equipamiento, const int id_tecnico,
                                        QString requerente,   QString ingresante, QDateTime fecha_ingreso,
                                        QDateTime fecha_devolucion, QDateTime fecha_vencimiento,
                                        QString causa_ingreso )
{
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO orden_trabajo(  cliente_id,  id_equipamiento,  id_tecnico,  requerente,  ingresante,  fecha_ingreso,  fecha_devolucion, fecha_vencimiento,  causa_ingreso ) "
                       "               VALUES    ( :id_cliente, :id_equipamiento, :id_tecnico, :requerente, :ingresante, :fecha_ingreso, :fecha_devolucion, :fecha_vencimiento, :causa_ingreso ) " ) ) {
        qDebug() << "Error al preparar la cola de inserción";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
    cola.bindValue( ":id_cliente"       , id_cliente        );
    cola.bindValue( ":id_equipamiento"  , id_equipamiento   );
    cola.bindValue( ":id_tecnico"       , id_tecnico        );
    cola.bindValue( ":requerente"       , requerente        );
    cola.bindValue( ":ingresante"       , ingresante        );
    cola.bindValue( ":fecha_ingreso"    , fecha_ingreso     );
    cola.bindValue( ":fecha_vencimiento", fecha_vencimiento );
    cola.bindValue( ":fecha_devolucion" , fecha_devolucion  );
    cola.bindValue( ":causa_ingreso"    , causa_ingreso     );
    if( cola.exec() ) {
        // Busco y devuelvo el ID
        if( QSqlDatabase::database().driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
            return cola.lastInsertId().toInt();
        } else {
            /// @TODO: Agregar codigo para esta condicion de ID
        }
    } else {
        qDebug() <<  "Error al insertar los datos de la orden de trabajo";
        qDebug() <<  cola.lastError().text();
        qDebug() <<  cola.lastQuery();
    }
    return -1;
}


