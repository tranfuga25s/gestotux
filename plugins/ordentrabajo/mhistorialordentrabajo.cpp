#include "mhistorialordentrabajo.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlDriver>

MHistorialOrdenTrabajo::MHistorialOrdenTrabajo(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable( "operacion_orden_trabajo" );
    setHeaderData( 0, Qt::Horizontal, "#ID Operacion" );
    setHeaderData( 1, Qt::Horizontal, "#ID Orden Trabajo" );
    setHeaderData( 2, Qt::Horizontal, "Fecha de operacion" );
    setHeaderData( 3, Qt::Horizontal, "Descripcion" );
    setHeaderData( 4, Qt::Horizontal, "Tipo de operacion" );
    setHeaderData( 5, Qt::Horizontal, "Tecnico" );
    setHeaderData( 6, Qt::Horizontal, "Costo" );
    setHeaderData( 7, Qt::Horizontal, "Sumatoria" );
    _id_orden_trabajo = 0;
    _mostrar_suma_costos = false;
    _tiene_relaciones = false;
    _sumas = new QMap<int, double>();
}

/*!
 * \brief MHistorialOrdenTrabajo::~MHistorialOrdenTrabajo
 */
MHistorialOrdenTrabajo::~MHistorialOrdenTrabajo()
{
    delete _sumas;
    _sumas=0;
}

/*!
 * \brief MHistorialOrdenTrabajo::setearRelacionTecnico
 */
void MHistorialOrdenTrabajo::setearRelacionTecnico()
{
    setRelation( 5, QSqlRelation( "tecnico", "id_tecnico", "razon_social" ) );
    _tiene_relaciones = true;
}

/*!
 * \brief MHistorialOrdenTrabajo::setearRelacionTipo
 */
void MHistorialOrdenTrabajo::setearRelacionTipo()
{
    setRelation( 4, QSqlRelation( "tipo_operacion_orden_trabajo", "id_tipo_operacion_orden_trabajo", "nombre" ) );
    _tiene_relaciones = true;
}

/*!
 * \brief MHistorialOrdenTrabajo::data
 * \param item
 * \param role
 * \return
 */
QVariant MHistorialOrdenTrabajo::data(const QModelIndex &item, int role) const
{
    if( _mostrar_suma_costos ) {
        switch( item.column() ) {
            case 7: {
                if( role == Qt::DisplayRole ) {
                    if( item.row() != 0 ) {
                        double nuevo = _sumas->value( item.row() - 1 );
                        nuevo += data( index( item.row(), 6 ), Qt::EditRole ).toDouble();
                        _sumas->insert( item.row(), nuevo );
                    } else {
                        _sumas->insert( item.row(), data( index( item.row(), 6 ), Qt::EditRole ).toDouble() );
                    }
                    return QString( "$ %L1" ).arg( _sumas->value( item.row() ), 10, 'f', 2 );
                } else if( role == Qt::EditRole ) {
                    return _sumas->value( item.row() );
                }
                break;
            }
            default:
            { break; }
        }
    }
    switch( item.column() ) {
        case 2: {
            switch( role ) {
                case Qt::DisplayRole:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::LocaleDate );
                    break;
                }
                default: { break; }
            }
            break;
        }
        case 6: // Costo
        {
            switch( role ) {
                case Qt::DisplayRole:
                {
                    return QString( "$ %L1" ).arg( QSqlRelationalTableModel::data( item, role ).toDouble(), 10, 'f', 2 );
                    break;
                }
                default:
                {
                    return QSqlRelationalTableModel::data( item, role );
                    break;
                }
            }
            break;
        }
        default:
        { break; }
    }
    return QSqlRelationalTableModel::data( item, role );
}

/*!
 * \brief MHistorialOrdenTrabajo::columnCount
 * \param parent
 * \return
 */
int MHistorialOrdenTrabajo::columnCount(const QModelIndex &parent) const
{
    if( _mostrar_suma_costos ) {
        return QSqlRelationalTableModel::columnCount( parent ) + 1;
    } else {
        return QSqlRelationalTableModel::columnCount( parent );
    }
}

/*!
 * \brief MHistorialOrdenTrabajo::agregarHistorial
 * Agrega un elemento al historial de una orden de trabajo
 * \param id_orden_trabajo Identificador de la orden de trabajo
 * \param fecha_hora Fecha y hora en que se realizó el trabajo
 * \param descripcion Descripción del trabajo
 * \param costo Costo asociado
 * \param tipo_operacion Identificador del tipo de operacion
 * \param id_tecnico Identificador del técnico responsable
 * \return -1 en caso de error, o el identificador (<0) si la agregado fue correcto
 */
int MHistorialOrdenTrabajo::agregarHistorial( const int id_orden_trabajo, QDateTime fecha_hora, QString descripcion, double costo, const int tipo_operacion, const int id_tecnico )
{
    QSqlRecord r = this->record();
    r.setGenerated( "id_historial_orden_trabajo", true );
    r.setValue( "id_orden_trabajo", id_orden_trabajo );
    r.setValue( "fecha_hora", fecha_hora );
    r.setValue( "descripcion", descripcion );
    r.setValue( "costo", costo );
    r.setValue( "tipo_operacion", tipo_operacion );
    r.setValue( "id_tecnico", id_tecnico );
    if( insertRecord( -1, r ) ) {
        // busco la ultima insercion
        if( QSqlDatabase::database().driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
            return this->query().lastInsertId().toInt();
        } else {
            /// @TODO: Agregar retorno cuando no está la feature
        }
    } else {
        qDebug() << "Error al ingresar el nuevo registro";
        qDebug() << this->query().lastError().text();
        qDebug() << this->query().lastQuery();
    }
    return -1;
    /// @TODO: Agregar implementación de agregado de item de historial
}

/*!
 * \brief MHistorialOrdenTrabajo::eliminarHistorial
 * \param id_historial
 * \return
 */
bool MHistorialOrdenTrabajo::eliminarHistorial(const int id_historial)
{
    return false;
    /// @TODO: Agregar implementación de eliminación de historial.
}

/*!
 * \brief MHistorialOrdenTrabajo::setearOrdenTrabajo
 * \param id_orden_trabajo
 */
void MHistorialOrdenTrabajo::setearOrdenTrabajo( const int id_orden_trabajo )
{
    if( id_orden_trabajo <= 0 ) {
        return;
    }
    _id_orden_trabajo = id_orden_trabajo;
    setFilter( QString( " id_orden_trabajo = %1 " ).arg( id_orden_trabajo ) );
}

/*!
 * \brief MHistorialOrdenTrabajo::mostrarCostosSumados
 */
void MHistorialOrdenTrabajo::mostrarCostosSumados( bool activar )
{
    _mostrar_suma_costos = activar;
}
