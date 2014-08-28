#include "mproductosunidades.h"

#include <QVector>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

/**
 * @brief MProductosUnidades::MProductosUnidades
 * @param parent
 */
MProductosUnidades::MProductosUnidades(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "productos_unidades" );
    setHeaderData( 0, Qt::Horizontal, "ID Producto" );
    setHeaderData( 1, Qt::Horizontal, "ID Unidad" );
}

/**
 * @brief MProductosUnidades::getUnidadesSegunProducto
 * Obtiene el listado completo
 * @param id_producto Identificador del producto
 * @return
 */
QVector<int> MProductosUnidades::getUnidadesSegunProducto( const int id_producto )
{
    QSqlQuery cola;
    QString consulta = QString( "SELECT id_unidad FROM %1 WHERE id_producto = %2" ).arg( this->tableName() ).arg( id_producto );
    if( cola.exec( consulta ) ) {
        QVector<int> ret;
        while( cola.next() ) {
            ret << cola.record().value(0).toInt();
        }
        if( ret.count() == 0 ) {
            // Agrego la unidad predeterminada
            ret << MProductosUnidades::predeterminada;
        }
        return ret;
    } else {
        qDebug() << "Error al ejecutar la cola de averiguacion de unidades de productos";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    QVector<int> ret;
    ret << MProductosUnidades::predeterminada;
    return ret;
}

/**
 * @brief MProductosUnidades::getProductosSegunUnidad
 * @param id_unidad
 * @return
 */
QVector<int> MProductosUnidades::getProductosSegunUnidad(const int id_unidad)
{
    QSqlQuery cola;
    QString consulta = QString( "SELECT id_producto FROM %1 WHERE id_unidad = %2" ).arg( this->tableName() ).arg( id_unidad );
    if( cola.exec( consulta ) ) {
        QVector<int> ret;
        while( cola.next() ) {
            ret << cola.record().value(0).toInt();
        }
        return ret;
    } else {
        qDebug() << "Error al ejecutar la cola de averiguacion de productos segun unidades";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QVector<int>();
}

/**
 * @brief MProductosUnidades::eliminarSegunUnidad
 * Elimina todos los datos realacionados con una unidad en la tabla de relaciones
 * @param id_unidad Identificador de la unidad
 * @param tiene_transaccion se ha iniciado una trasaccion antes
 * @return estado de la operacion
 */
bool MProductosUnidades::eliminarSegunUnidad(const int id_unidad, bool tiene_transaccion )
{
    if( !tiene_transaccion ) { QSqlDatabase::database().transaction(); }

    QSqlQuery cola;
    if( cola.exec( QString( "DELETE FROM %1 WHERE id_unidad = %2" ).arg( this->tableName() ).arg( id_unidad ) ) ) {
        if( !tiene_transaccion ) { QSqlDatabase::database().commit(); }
        return true;
    } else {
        qDebug() << "Error al intentar eliminar las relaciones unidad producto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    if( !tiene_transaccion ) { QSqlDatabase::database().rollback(); }
    return false;
}
