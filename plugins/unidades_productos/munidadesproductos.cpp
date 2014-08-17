#include "munidadesproductos.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "mproductosunidades.h"

MUnidadesProductos::MUnidadesProductos(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "unidades_productos" );
    setHeaderData( 0, Qt::Horizontal, tr( "#ID"      ) );
    setHeaderData( 1, Qt::Horizontal, tr( "Nombre"   ) );
    setHeaderData( 2, Qt::Horizontal, tr( "Padre"    ) );
    setHeaderData( 3, Qt::Horizontal, tr( "Multiplo" ) );
}

/**
 * @brief MUnidadesProductos::data
 * @param idx
 * @param role
 * @return
 */
QVariant MUnidadesProductos::data(const QModelIndex &idx, int role) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( idx.column() ) {
                case 3:
                {
                    return QString( "%L1" ).arg( QSqlTableModel::data( idx, role ).toDouble() );
                    break;
                }
                default: { break; }
            }
            break;
        }
        default: { break; }
    }
    return QSqlTableModel::data( idx, role );
}

/**
 * @brief MUnidadesProductos::getValorNominal
 * Devuelve el valor multiplicado por el multiplo correcto
 *
 * @param id_unidad Unidad de referencia
 * @param cantidad Cantidad a transformar
 * @return Valor transforado
 */
double MUnidadesProductos::getValorNominal( const int id_unidad, const double cantidad )
{
    // Veo si la unidad tiene un padre
    if( this->tienePadre( id_unidad ) ) {
        double multiplo = this->getMultiplo( id_unidad );
        return cantidad * multiplo;
    } else {
        return cantidad;
    }
}

/**
 * @brief MUnidadesProductos::tienePadre
 * Devuelve verdadero si existe un padre para el item
 * @param id_unidad Identificador de la unidad
 * @return booleano
 */
bool MUnidadesProductos::tienePadre( const int id_unidad )
{
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT id_padre FROM unidades_productos WHERE id_unidad = %1" ).arg( id_unidad ) ) ) {
        qDebug() << "Error al ejecutar la cola de averiguacion de padre";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    if( cola.next() ) {
        if( !cola.record().value(0).isNull()
          && cola.record().value(0).toInt() > 0 ) {
            return true;
        }
    }
    return false;
}

/**
 * @brief MUnidadesProductos::getHijos
 * Devuelve el listado de hijos que poseea la unidad definida
 * @param id_unidad Identificador de la unidad
 * @return
 */
QVector<int> MUnidadesProductos::getHijos( const int id_unidad )
{
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT id FROM unidades_productos WHERE id_padre = %1" ).arg( id_unidad ) ) ) {
        qDebug() << "Error al ejecutar la cola de averiguacion de hijos";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    QVector<int> ret;
    while( cola.next() ) {
        ret << cola.record().value(0).toInt();
    }
    return ret;
}

/**
 * @brief MUnidadesProductos::getMultiplo
 * Obtiene el multiplo que corresponde para llevarlo a la unidad padre
 * @param id_unidad Identificador de la unidad
 * @return double
 */
double MUnidadesProductos::getMultiplo( const int id_unidad )
{
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT multiplo FROM unidades_productos WHERE id_unidad = %1" ).arg( id_unidad ) ) ) {
        qDebug() << "Error al ejecutar la cola de averiguacion de multiplo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    if( cola.next() ) {
        double multiplo = cola.record().value(0).toDouble();
        if( multiplo != 0.0 ) {
            return multiplo;
        }
    }
    return 0.0;
}

/**
 * @brief MUnidadesProductos::eliminar
 * Elimina una unidad de productos
 * @param id_unidad Identificador de unidad
 * @param eliminar_hijos Elimina los hijos si posee
 * @param tiene_transaccion La llamada anterior ya inicio una transaccion
 * @return resultado de la operacion
 */
bool MUnidadesProductos::eliminar( const int id_unidad, const bool eliminar_hijos, const bool tiene_transaccion )
{
    return false;
    bool es_padre = !this->tienePadre( id_unidad );
    if( es_padre && !eliminar_hijos ) {
        // No elimino padres si no puedo eliminar hijos
        return false;
    }
    if( !tiene_transaccion ) { QSqlDatabase::database().transaction(); }
    QSqlQuery cola;

    // Veo si tiene hijos
    if( es_padre && eliminar_hijos ) {

        QVector<int> hijos = this->getHijos( id_unidad );

        foreach( int id_hijo, hijos ) {
            if( !this->eliminar( id_hijo, false, true ) ) {
                 if( !tiene_transaccion ) { QSqlDatabase::database().rollback(); }
                 return false;
            }
        }

    }

    if( cola.exec( QString( "DELETE FROM %1 WHERE id_unidad = %2" ).arg( this->tableName() ).arg( id_unidad ) ) ) {
        // Elimino los datos relacionados
        MProductosUnidades *mpu = new MProductosUnidades();
        if( mpu->eliminarSegunUnidad( id_unidad, true ) ) {
            if( !tiene_transaccion ) { QSqlDatabase::database().commit(); }
            delete mpu;
            return true;
        } else {

        }
        delete mpu;
    } else {
        qDebug() << "Error al intentar eliminar una unidad";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }

    if( !tiene_transaccion ) { QSqlDatabase::database().rollback(); }
    return false;
}
