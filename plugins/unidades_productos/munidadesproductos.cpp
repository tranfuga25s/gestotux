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
    _parent_index = new QHash<int,QModelIndex>();
    _parent_index->clear();
}

/**
 * @brief MUnidadesProductos::data
 * Devuelve los datos del elemento formateado para que se vean correctamente
 * @param idx Indice del elemento a mostrar
 * @param role Que se desea mostrar
 * @return Representación deseada
 */
QVariant MUnidadesProductos::data(const QModelIndex &idx, int role) const
{
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( idx.column() ) {
                case 3:
                {
                    return QString( "%L1" ).arg( QSqlTableModel::data( idx, role ).toDouble(), 20, 'f', 10 );
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
 * @brief MUnidadesProductos::parent
 * @param child
 * @return
 */
QModelIndex MUnidadesProductos::parent( const QModelIndex &child ) const
{
    int id_hijo = this->data( this->index( child.row(), 0 ), Qt::DisplayRole ).toInt();
    if( this->tienePadre( id_hijo ) ) {
        int id_padre = this->obtenerPadre( id_hijo );
        if( _parent_index->contains( id_padre ) ) {
            return _parent_index->value( id_padre );
        }
    }
    return QModelIndex();
}

/**
 * @brief MUnidadesProductos::flags
 * @param index
 * @return
 */
Qt::ItemFlags MUnidadesProductos::flags( const QModelIndex & ) const
{ return QFlag( Qt::ItemIsEnabled | !Qt::ItemIsEditable ); }

/**
 * @brief MUnidadesProductos::index
 *
 *
 * @param row rowID
 * @param column
 * @param parent
 * @return
 */
QModelIndex MUnidadesProductos::index( int row, int column, const QModelIndex &par ) const
{
    // Check if is a parent or not
    QModelIndex original = QSqlTableModel::index( row, column, par );
    int id_actual = data( QSqlTableModel::index( row, 0 ), Qt::EditRole ).toInt();
    if( !tienePadre( id_actual ) ) {
        _parent_index->insert( id_actual, original );
    }
    return original;
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
bool MUnidadesProductos::tienePadre( const int id_unidad ) const
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
    if( !cola.exec( QString( "SELECT id_unidad FROM unidades_productos WHERE id_padre = %1" ).arg( id_unidad ) ) ) {
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
 * @brief MUnidadesProductos::obtenerPadre
 * @param id_unidad
 * @return
 */
int MUnidadesProductos::obtenerPadre( const int id_unidad ) const
{
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT id_padre FROM unidades_productos WHERE id_unidad = %1" ).arg( id_unidad ) ) ) {
        qDebug() << "Error al ejecutar la cola de averiguacion de padre";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();

    } else {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        }
    }
    return 0;
}

/**
 * @brief MUnidadesProductos::agregarUnidad
 * @param id_padre Identificador del padre. Si es 0, entonces es padre.
 * @param nombre Nombre a utilizar
 * @param multiplicador Multiplicador respecto al padre.
 * @return resultado de la operación. Falso si hubo algún error.
 */
bool MUnidadesProductos::agregarUnidad( const int id_padre, const QString nombre, double multiplicador )
{
    if( nombre.isEmpty() ) { return false; }

    if( multiplicador == 0.0 ) { return false; }

    // Condicion predeterminada
    if( id_padre == 0 ) { multiplicador = 1.0; }

    QSqlQuery cola;

    if( !cola.prepare( QString( "INSERT INTO %1 ( id_padre, nombre, multiplo ) "
                                " VALUES ( :id_padre, :nombre, :multiplicador )" ).arg( this->tableName() ) ) ) {
        qDebug() << "Error al intentar preparar la cola de inserción de unidades";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }

    cola.bindValue( ":id_padre", id_padre );
    cola.bindValue( ":nombre", nombre );
    cola.bindValue( ":multiplicador", multiplicador );
    if( cola.exec() ) {
        return true;
    }
    qDebug() << "Error al intentar ejecutar la cola de inserción de unidades";
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return false;
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
                 qDebug() << "Error al eliminar un hijo";
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
            qDebug() << "Error al eliminar según unidad";
        }
        delete mpu;
    } else {
        qDebug() << "Error al intentar eliminar una unidad - exec";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }

    if( !tiene_transaccion ) { QSqlDatabase::database().rollback(); }
    return false;
}
