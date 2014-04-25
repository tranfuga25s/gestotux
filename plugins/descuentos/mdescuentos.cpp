#include "mdescuentos.h"

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

MDescuentos::MDescuentos(QObject *parent) :
QSqlTableModel(parent)
{
    setTable( "descuentos" );
    _seleccionado = false;
    _id_comprobante = -1;
    _tipo = Invalido;
}

void MDescuentos::inicializar() {
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "Texto" );
    setHeaderData( 2, Qt::Horizontal, "Porcentaje" );
}

bool MDescuentos::agregarDescuento( Tipo t, int id_comprobante, QString texto, double porcentaje ) {
    if( t == Invalido )
        return false;
    if( id_comprobante == 0 || texto.isEmpty() || porcentaje <= 0.0 )
        return false;

    QSqlRecord r = this->record();
    r.setNull( "id_descuento" );
    r.setValue( "texto", texto );
    r.setValue( "cantidad", porcentaje );
    if( this->insertRecord( -1, r ) ) {
        // Busco el ID
        int id_descuento = this->query().lastInsertId().toInt();
        if( id_descuento <= 0 ) {
            qWarning( "Error al obtener el id del descuento agregado" );
            return false;
        }
        submit();
        this->clear();
        this->setTable( "descuento_comprobante" );
        QSqlRecord dc = this->record();
        dc.setValue( "id_descuento", id_descuento );
        dc.setValue( "id_comprobante", id_comprobante );
        dc.setValue( "tipo", t );
        if( !this->insertRecord( -1, dc ) ) {
            qWarning( "Error al insertar el registro de asociacion con el comprobante de un descuento" );
            return false;
        } else {
            this->clear();
            this->setTable( "descuentos" );
            inicializar();
        }
    } else {
        qWarning( "Error al insertar el registro de descuento" );
        return false;
    }
    return true;
}

/*!
 * \fn MDescuentos::existenDatos()
 * Devuelve verdadero si existen datos para los valores de tipo y el id de comprobantes que se seteo anteriormente.
 */
bool MDescuentos::existenDatos()
{
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT COUNT(id_comprobante) FROM descuento_comprobante WHERE id_comprobante = %1 AND tipo = %2" ).arg( _id_comprobante ).arg( _tipo ) ) ) {
      cola.next();
      if( cola.record().value(0).toInt() > 0 ) {
          return true;
      }
  } else {
      qDebug( "Error al buscar la cantidad de descuentos relacionados a un comprobante" );
      qDebug( cola.lastError().text().toLocal8Bit() );
      qDebug( cola.lastQuery().toLocal8Bit() );
  }
  return false;
}

/*!
 * \fn MDescuentos::seleccionarDatos()
 * Genera el select si no fue echo ya
 */
bool MDescuentos::seleccionarDatos()
{
    // Ya se que existen datos asi que genero el filtro y listo
    QString ids;
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_descuento FROM descuento_comprobante WHERE id_comprobante = %1 AND tipo = %2" ).arg( _id_comprobante ).arg( _tipo ) ) ) {
        while( cola.next() ) {
            ids.append( QString( " , %1").arg( cola.record().value(0).toInt() ) );
        }
        this->setFilter( QString( " id_descuento IN ( %1 )" ).arg( ids.remove( 0 , 2 ) ) );
        this->select();
        return true;
    } else {
        qDebug( "Error al buscar la cantidad de descuentos relacionados a un comprobante" );
        qDebug( cola.lastError().text().toLocal8Bit() );
        qDebug( cola.lastQuery().toLocal8Bit() );
    }
    return false;
}
