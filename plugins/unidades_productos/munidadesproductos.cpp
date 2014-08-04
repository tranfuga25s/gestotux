#include "munidadesproductos.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

MUnidadesProductos::MUnidadesProductos(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable( "unidades_productos" );
    setHeaderData( 0, Qt::Horizontal, tr( "#ID"      ) );
    setHeaderData( 1, Qt::Horizontal, tr( "Nombre"   ) );
    setHeaderData( 2, Qt::Horizontal, tr( "Padre"    ) );
    setHeaderData( 3, Qt::Horizontal, tr( "Multiple" ) );
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
