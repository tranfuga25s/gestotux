#include "utilreportedeudoresservicios.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>

UtilReporteDeudoresServicios::UtilReporteDeudoresServicios(QObject *parent) :
    QObject(parent)
{
}


/*!
 * \fn MPeriodoServicio::existeFacturacion( const int id_servicio )
 * Devuelve verdadero si existe alguna facturación para el servicio elegido como parametro
 * \param id_servicio Identificador del servicio
 **/
bool UtilReporteDeudoresServicios::existeFacturacionServicio(const int id_servicio)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT( id_servicio ) FROM periodo_servicio WHERE id_servicio = %1" ).arg( id_servicio ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        } else {
            return false;
        }
    } else {
        qDebug() << "Resumenes::UtilReporteDeudoresServicios::existeFacturacion: Error en el exec de la cola actual";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
}

/*!
 * \fn UtilReporteDeudoresServicios::getNombreServicio( int id_servicio )
 * Devuelve el nombre de un servicio
 * @param id_servicio Identificador del servicio al cual se le quiere saber el nombre.
 * @return Nombre del servicio o una cadena vacÃ­a si existio un error al obtener el nombre
 */
QString UtilReporteDeudoresServicios::getNombreServicio( const int id_servicio )
{
  QSqlQuery cola( QString( "SELECT nombre FROM %2 WHERE id_servicio = %1" ).arg( id_servicio ).arg( "servicios") );
  if( cola.exec()) {
      if( cola.next() ) {
          return cola.record().value(0).toString();
      } else { qDebug( "Servicios:MServicios:Error al hacer el next en la cola de nombre del servicio" ); return ""; }
  } else { qDebug( "Servicios:MServicios:Error al hacer el exec en la cola del nombre del servicio" ); return ""; }
}


/**
 * @brief UtilReporteDeudoresServicios::periodoMinimo
 * @return
 */
QDate UtilReporteDeudoresServicios::periodoMinimo()
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT MIN(fecha_inicio) FROM periodo_servicio" ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        }
    } else {
        qDebug( "Servicios::UtilReporteDeudoresServicios::periodoMinimo: Error en el exec de la cola actual" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}

/**
 * @brief UtilReporteDeudoresServicios::periodoMinimo
 * @return
 */
QDate UtilReporteDeudoresServicios::periodoMaximo()
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT MAX(fecha_inicio) FROM periodo_servicio" ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        }
    } else {
        qDebug( "Servicios::UtilReporteDeudoresServicios::periodoMaximo: Error en el exec de la cola actual" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}

QString UtilReporteDeudoresServicios::getRazonSocial(const int id_cliente) {

    QSqlQuery cola;
    if( cola.exec( QString( "SELECT razon_social FROM clientes WHERE id = %1" ).arg( id_cliente ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toString();
        } else {
            qDebug() << "Error al hacer next en la cola de averiguacion de razonsocial de clientes";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug( "Error al ejecutar  la cola de averiguacion de razonsocial de clientes" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return "Desconocido";
}

/**
 *
 */
QList<int> UtilReporteDeudoresServicios::listaDeServicios()
{
    QList<int> lista;
    QSqlQuery cola;
    if( cola.exec( "SELECT id_servicio FROM servicios WHERE fecha_baja IS NULL" ) ) {
        while( cola.next() ) {
            lista.append( cola.record().value(0).toInt() );
        }
    }
    return lista;
}
