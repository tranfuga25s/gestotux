#include "mequipamiento.h"

#include "mgarantias.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QDebug>
#include <QSqlField>

MEquipamiento::MEquipamiento(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable( "equipamiento" );
    setHeaderData(  0, Qt::Horizontal, "#ID"               );
    setHeaderData(  1, Qt::Horizontal, "Descripcion"       );
    setHeaderData(  2, Qt::Horizontal, "Marca"             );
    setHeaderData(  3, Qt::Horizontal, "Modelo"            );
    setHeaderData(  4, Qt::Horizontal, "#Serie"            );
    setHeaderData(  5, Qt::Horizontal, "Fecha de Compra"   );
    setHeaderData(  6, Qt::Horizontal, "Fecha Vencimiento" );
    setHeaderData(  7, Qt::Horizontal, "#Factura"          );
    setHeaderData(  8, Qt::Horizontal, "#Cliente"          );
    setHeaderData(  9, Qt::Horizontal, "Bultos"            );
    setHeaderData( 10, Qt::Horizontal, "Fecha de baja"     );
    setHeaderData( 11, Qt::Horizontal, "Razon de baja"     );
}

/*!
 * \brief MEquipamiento::data
 * \param item
 * \param role
 * \return
 */
QVariant MEquipamiento::data(const QModelIndex &item, int role) const
{
    switch( item.column() ) {
        case 5:
        case 6:
        case 10:
        {
            if( role == Qt::DisplayRole ) {
                return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::LocaleDate );
                break;
            } else if ( role == Qt::UserRole && item.column() == 10 ) {
                QDate temp = QSqlRelationalTableModel::data( item, role ).toDate();
                if( temp.isValid() ) { return true;  } else { return false;  }
            }
        }
    }
    return QSqlRelationalTableModel::data( item, role );
}

/*!
 * \brief MEquipamiento::tieneDatosRelacionados
 * Busca si el elemento posee alguna relación con los otros datos de las tablas.
 * \param id_equipamiento Identificador del equipamiento
 * \return Verdadero si hay alguna relacion
 */
bool MEquipamiento::tieneDatosRelacionados( const int /* id_equipamiento */ )
{ return false; }

// Los datos relacionados deberán ser eliminados si se elimina un equipamiento
/*!
 * \brief MEquipamiento::eliminarConRelacionados
 * \param id_equipamiento
 * \return
 */
bool MEquipamiento::eliminarConRelacionados( const int /*id_equipamiento*/ )
{ return false; }

/*!
 * \brief MEquipamiento::darBaja
 * \param id_equipamiento Identificador del equipamiento
 * \param razon Razon de baja del equipamiento
 * \param fecha Fecha de baja
 * \return
 */
bool MEquipamiento::darDeBaja( const int id_equipamiento, const QString razon, QDateTime fechahora )
{
    QSqlQuery cola;
    if( !cola.prepare( "UPDATE equipamientos SET razon = :razon, fecha_baja = :fecha_baja WHERE id_equipamiento = :id_equipamiento" ) ) {
        qWarning( "No se pudo preparar la cola de baja de equipamiento" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    cola.bindValue( ":razon", razon );
    cola.bindValue( ":fecha_baja", fechahora );
    cola.bindValue( ":id_equipamiento", id_equipamiento );
    if( cola.exec() ) {
        return true;
    } else {
        qWarning( "No se pudo ejecutar la cola de baja de equipamiento" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MEquipamiento::darReAlta
 * Coloca los datos de razon y fecha de baja en nulos para que el equipamiento esté como dado de alta nuevamente.
 * \param id_equipamiento Identificador de equipamiento
 * \return
 */
bool MEquipamiento::darReAlta( const int id_equipamiento )
{
    QSqlQuery cola;
    if( !cola.prepare( "UPDATE equipamientos SET razon = :razon, fecha_baja = :fecha_baja WHERE id_equipamiento = :id_equipamiento" ) ) {
        qWarning( "No se pudo preparar la cola de re alta de equipamiento" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    cola.bindValue( ":razon", QVariant::String );
    cola.bindValue( ":fecha_baja", QVariant::Date );
    cola.bindValue( ":id_equipamiento", id_equipamiento );
    if( cola.exec() ) {
        return true;
    } else {
        qWarning( "No se pudo ejecutar la cola de re alta de equipamiento" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MEquipamiento::existeEquipamientoParaCliente
 * Averigua si existe algún equipamiento para el cliente pasado como parametro.
 * \param id_cliente Identificador del cliente
 * \return Verdadero si existe al menos uno
 */
bool MEquipamiento::existeEquipamientoParaCliente( const int id_cliente )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id_equipamiento) FROM equipamiento WHERE id_cliente = %1" ).arg( id_cliente ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            } else {
                return false;
            }
        } else {
            qDebug() << "Error al hacer next en la cola de averiguación de existencia de equipamiento para un cliente";
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguación de existencia de equipamiento para un cliente";
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return false;
}

/*!
 * \brief MEquipamiento::agregarEquipamiento
 * \param descripcion
 * \param marca
 * \param modelo
 * \param num_serie
 * \param observaciones
 * \return
 */
int MEquipamiento::agregarEquipamiento( const int id_cliente,
                                        const QString descripcion,
                                        const QString marca,
                                        const QString modelo,
                                        const QString num_serie,
                                        const QString observaciones )
{
    QSqlQuery cola;
    QString tcola = "INSERT INTO equipamiento( id_cliente , descripcion , marca , modelo , numero_serie , observaciones  ) "
                                   "  VALUES ( :id_cliente, :descripcion, :marca, :modelo, :numero_serie, :observaciones ) ";
    if( !cola.prepare( tcola ) ) {
        qDebug() << "Error al preparar la cola para insertar un nuevo equipamiento";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
    cola.bindValue( ":id_cliente"   , id_cliente    );
    cola.bindValue( ":descripcion"  , descripcion   );
    cola.bindValue( ":marca"        , marca         );
    if( !modelo.isEmpty() ) {
        cola.bindValue( ":modelo", modelo );
    } else {
        cola.bindValue( ":modelo", QVariant(QVariant::String) );
    }
    if( !num_serie.isEmpty() ) {
        cola.bindValue( ":numero_serie", num_serie );
    } else {
        cola.bindValue( ":numero_serie", QVariant(QVariant::String) );
    }
    if( !observaciones.isEmpty()  ) {
        cola.bindValue( ":observaciones", observaciones );
    } else {
        cola.bindValue( ":observaciones", QVariant(QVariant::String) );

    }
    if( !cola.exec() ) {
        qDebug() << "Error al ejecutar la cola de inserción de nueva orden de trabajo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    } else {
        if( QSqlDatabase::database().driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
            return cola.lastInsertId().toInt();
        } else {
            /// @TODO: Agregada devolución de ID de equipamiento cuando el driver no lo soporta
        }
    }
    return -1;
}

/*!
 * \brief MEquipamiento::cantidadEquipamientoParaCliente
 * Devuelve la cantidad de equipamientos designados para un cliente
 * \param id_cliente Identificador del cliente
 * \return cantidad de equipamientos declarados o 0 si hubo error
 */
int MEquipamiento::cantidadEquipamientoParaCliente( const int id_cliente )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id_equipamiento) FROM equipamiento WHERE id_cliente = %1" ).arg( id_cliente ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qDebug() << "Error al hacer next en la cola de averiguación de existencia de equipamiento para un cliente";
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguación de existencia de equipamiento para un cliente";
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return 0;
}

/*!
 * \brief MEquipamiento::buscarIdEquipamientoSegunCliente
 * Devuelve el identificador del primer equipamiento dado para un cliente específico.
 * \param id_cliente Identificador del cliente
 * \return Identificador del equipamiento o -1 en caso de error
 */
int MEquipamiento::buscarIdEquipamientoSegunCliente(const int id_cliente)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_equipamiento FROM equipamiento WHERE id_cliente = %1 LIMIT 1" ).arg( id_cliente ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qDebug() << "Error al hacer next en la cola de averiguación de existencia de equipamiento para un cliente";
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguación de existencia de equipamiento para un cliente";
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return -1;
}


/*!
 * \brief MEquipamiento::flags
 * \return
 */
Qt::ItemFlags MEquipamiento::flags( const QModelIndex & ) const
{
    return Qt::ItemFlags( !Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );
}

/*!
 * \brief MEquipamiento::existe
 * \param id_equipamiento
 * \return
 */
bool MEquipamiento::existe( const int id_equipamiento )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id_equipamiento) FROM equipamiento WHERE id_equipamiento = %1" ).arg( id_equipamiento ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            } else {
                return false;
            }
        } else {
            qDebug() << "Error al hacer next en la cola de averiguación de existencia de equipamiento";
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguación de existencia de equipamiento";
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return false;
}

/*!
 * \brief MEquipamiento::cargarDatos
 * Carga los datos del equipamiento en el registro interno.
 * \param id_equipamiento Identificador del equipamiento
 */
bool MEquipamiento::cargarDatos( const int id_equipamiento )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT * FROM equipamiento WHERE id_equipamiento = %1" ).arg( id_equipamiento ) ) ) {
        if( cola.next() ) {
            this->_datos = cola.record();
            return true;
        } else {
            qDebug() << "Error al obtener los datos del ID de equipamiento indicado";
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguación de datos de equipamiento";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MEquipamiento::dadoDeBaja
 * \return
 */
bool MEquipamiento::dadoDeBaja()
{
    if( _datos.contains( "fecha_baja" ) ) {
        if( !_datos.field("fecha_baja").isNull() ) {
            if( _datos.field( "fecha_baja" ).value().toDate().isValid() ) {
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief MEquipamiento::enGarantia
 * Devuelve verdadero si el equipamiento está en garantía o no.
 * \return
 */
bool MEquipamiento::enGarantia()
{
    if( _datos.contains( "id_equipamiento" ) ) {
        int id_equipamiento = _datos.value( "id_equipamiento" ).toInt();
        bool estado = false;
        MGarantias *mgarantia = new MGarantias();
        estado = mgarantia->estaActiva( mgarantia->obtenerIdSegunEquipamiento( id_equipamiento ) );
        delete mgarantia;
        return estado;
    }
}