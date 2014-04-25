#include "mgarantias.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlDriver>

MGarantias::MGarantias(QObject *parent) :
QSqlRelationalTableModel(parent)
{
    setTable("garantias");
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "#Serie" );
    setHeaderData( 2, Qt::Horizontal, "#Numero" );
    setHeaderData( 3, Qt::Horizontal, "Texto" );
    setHeaderData( 4, Qt::Horizontal, "Fecha de Inicio" );
    setHeaderData( 5, Qt::Horizontal, "Fecha de Fin" );
    setHeaderData( 6, Qt::Horizontal, "#Cliente" );
    setHeaderData( 7, Qt::Horizontal, "#Equipamiento" );
    setHeaderData( 8, Qt::Horizontal, "#Factura" );
    setHeaderData( 9, Qt::Horizontal, "#Codigo" );
    setHeaderData( 10, Qt::Horizontal, "Fecha de Baja" );
    setHeaderData( 11, Qt::Horizontal, "Razón de Baja" );
    setHeaderData( 12, Qt::Horizontal, "#Producto" );

}

void MGarantias::relacionar()
{
    setRelation( 4, QSqlRelation( "cliente", "id", "razon_social" ) );
    setRelation( 5, QSqlRelation( "equipamiento", "id_equipamiento", "Descripcion" ) );
}

QVariant MGarantias::data( const QModelIndex &item, int role ) const
{
    switch( item.column() ) {
        case 5:
        case 4:
        case 9:
        {
            switch( role )
            {
                case Qt::DisplayRole:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::SystemLocaleShortDate );
                    break;
                }
                default: { break; }
            }
            break;
        }
        default:
        { break; }
    }
    return QSqlRelationalTableModel::data( item, role );
}

/*!
 * \brief MGarantias::darDeBaja
 * \param id_garantia Identificador de la garantía
 * \param razon Razon de la baja
 * \param fecha Fecha de la baja
 * \return verdadero si se pudo dar de baja correctamente
 */
bool MGarantias::darDeBaja( const int id_garantia, const QString razon, const QDate fecha )
{
    QSqlQuery cola;
    if( !cola.prepare( "UPDATE garantias SET razon_baja = :razon, fecha_baja = :fecha WHERE id_garantia = :id_garantia" ) ) {
        qWarning() << "Error al preparar la cola de baja de garantía";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    cola.bindValue( ":id_garantia", id_garantia );
    cola.bindValue( ":razon", razon );
    cola.bindValue( ":fecha", fecha );
    if( cola.exec() ) {
        return true;
    } else {
        qWarning() << "Error al ejecutar la cola de baja de garantia";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    return false;
}

/*!
 * \brief MGarantias::estaActiva
 * \param id_garantia
 * \return
 */
bool MGarantias::estaActiva( const int id_garantia )
{
    if( id_garantia <= 0 ) {
        return false;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha_inicio, fecha_fin, fecha_baja FROM %1 WHERE id_garantia = %2" ).arg( this->tableName() ).arg( id_garantia ) ) ) {
        if( cola.next() ) {
            QDate inicio, fin, baja;
            inicio = cola.record().value(0).toDate();
            fin    = cola.record().value(1).toDate();
            baja   = cola.record().value(2).toDate();
            if( baja.isValid() ) {
                return false; // La garantía esta dada de baja
            } else if( inicio.isValid() && fin.isValid() ) {
                if( inicio <= QDate::currentDate() &&
                       fin >= QDate::currentDate() ) {
                    return true;
                } else {
                    return false;
                }
            }
        }else {
            qWarning() << "Error al hacer next en la cola de obtencion de datos para averiguacion de garantia activa";
            qDebug() << cola.lastQuery();
        }
    } else {
        qWarning() << "Error al ejecutar la cola de obtencion de datos para averiguación de garantias activa";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MGarantias::eliminar
 * \param id_garantia
 * \return
 */
bool MGarantias::eliminar( const int id_garantia )
{   QSqlQuery cola;
    if( cola.exec( QString( "DELETE FROM garantias WHERE id_garantia = %1" ).arg( id_garantia ) ) ) {
        return true;
    } else {
        qWarning() << "Error al ejecutar la cola de baja de garantia";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    return false;
}

/*!
 * \brief MGarantias::obtenerProximoComprobante
 * \return
 */
NumeroComprobante MGarantias::obtenerProximoComprobante()
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT MAX( serie ) FROM garantias" ) ) ) {
        if( cola.next() ) {
            int serie = cola.record().value(0).toInt();
            if( serie == 0 ) {
                serie = 1;
            }
            if( cola.exec( QString( "SELECT MAX( numero ) FROM garantias WHERE serie = %1" ).arg( serie ) ) ) {
                if( cola.next() ) {
                    int numero = cola.record().value(0).toInt();
                    NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
                    num->siguienteNumero();
                    return *num;
                } else {
                    qDebug( "Error de cola al hacer next al obtener el numero de garantia maximo");
                }
            } else {
                qDebug( "Error de cola al hacer exec al obtener el numero de garantia maximo" );
            }
        } else {
            qDebug( "Error de cola al hacer next al obtener el numero de serie de garantia maximo -  Se inicio una nueva numeracion" );
        }
    } else {
        NumeroComprobante *num = new NumeroComprobante( 0, 1, 0 );
        num->siguienteNumero();
        qDebug( "Error de cola al hacer exec al obtener el numero de serie de garantia maximo - Se inicio una nueva numeracion" );
        return *num;
    }
    NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
    return *invalido;
}

/*!
 * \brief MGarantias::obtenerIdSegunEquipamiento
 * \param id_equipamiento
 * \return
 */
int MGarantias::obtenerIdSegunEquipamiento( const int id_equipamiento )
{
    if( id_equipamiento <= 0 ) {
        return -1;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_garantia FROM %1 WHERE id_equipamiento = %2" ).arg( this->tableName() ).arg( id_equipamiento ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        }
    } else {
        qWarning() << "Error al ejecutar la cola de obtencion de datos garantia <-> equipamiento";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}

/*!
 * \brief MGarantias::obtenerIdSegunProducto
 * \param id_producto
 * \return
 */
int MGarantias::obtenerIdSegunProducto( const int id_producto )
{
    if( id_producto <= 0 ) {
        return -1;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_garantia FROM %1 WHERE id_producto = %2" ).arg( this->tableName() ).arg( id_producto ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qWarning() << "Error al hacer next en la cola de obtencion de datos garantia <-> producto";
            qDebug() << cola.lastQuery();
        }
    } else {
        qWarning() << "Error al ejecutar la cola de obtencion de datos garantia <-> producto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}

/*!
 * \brief MGarantias::agregarGarantia
 * Agrega una nueva garantía al sistema
 * \param id_cliente Identificación del cliente
 * \param texto_producto Texto del producto o elemento de la garantía
 * \param fecha_inicio Fecha de inicio de la garantia
 * \param fecha_fin Fecha de fin de la garantia
 * \param id_equipamiento Identificador del equipamiento al que pertenece
 * \param id_producto Identificador del producto elegido
 * \param id_factura Identificador de la factura/recibo de emision
 * \return
 */
int MGarantias::agregarGarantia( NumeroComprobante codigo_garantia, const int id_cliente, const QString nombre_producto, const QDate fecha_inicio, const QDate fecha_fin, const int id_equipamiento, const int id_producto, const int id_factura)
{
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO garantias( serie, numero, id_cliente, nombre_producto, fecha_inicio, fecha_fin, id_equipamiento, id_factura, id_producto ) "
                       " VALUES ( :serie, :numero, :id_cliente, :texto_producto, :fecha_inicio, :fecha_fin, :id_equipamiento, :id_factura, :id_producto ) " ) ) {
        qWarning() << "Error al preparar la cola de alta de garantía";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
    cola.bindValue( ":serie"          , codigo_garantia.serie()  );
    cola.bindValue( ":numero"         , codigo_garantia.numero() );
    cola.bindValue( ":id_cliente"     , id_cliente               );
    cola.bindValue( ":nombre_producto", nombre_producto          );
    cola.bindValue( ":fecha_inicio"   , fecha_inicio             );
    cola.bindValue( ":fecha_fin"      , fecha_fin                );
    cola.bindValue( ":id_equipamiento", id_equipamiento          );
    cola.bindValue( ":id_factura"     , id_factura               );
    cola.bindValue( ":id_producto"    , id_producto              );
    if( cola.exec() ) {
        if( QSqlDatabase::database().driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
            return cola.lastInsertId().toInt();
        } else {
            qDebug() << "No agregado elemento para cuando el driver no tiene LastInsertedId";
            /// @TODO: Falta agregar esto para cuando el driver no tiene LastInsertedId
            return -1;
        }
    } else {
        qWarning() << "Error al ejecutar la cola de baja de garantia";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}


