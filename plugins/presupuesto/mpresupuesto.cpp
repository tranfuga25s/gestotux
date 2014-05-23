/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller                                  *
 *   tranfuga25s@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mpresupuesto.h"

#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include "mdescuentos.h"

MPresupuesto::MPresupuesto(QObject *parent )
 : QSqlRelationalTableModel(parent)
{
    inicializar();
    relacionar();
}

void MPresupuesto::inicializar()
{
    setTable( "presupuestos" );
    // Descripciondes de las cabeceras
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "Cliente" );
    setHeaderData( 2, Qt::Horizontal, "Destinatario" );
    setHeaderData( 3, Qt::Horizontal, "Fecha" );
    setHeaderData( 4, Qt::Horizontal, "Total" );
    setHeaderData( 5, Qt::Horizontal, "Contenido" );
}

void MPresupuesto::relacionar()
{
  setRelation( 1, QSqlRelation( "clientes", "id", "razon_social" ) );
}



/*
CREATE TABLE IF NOT EXISTS `presupuestos` (
  `id_presupuesto` bigint(20) NOT NULL AUTO_INCREMENT,
  `id_cliente` bigint(20) DEFAULT NULL,
  `destinatario` text COLLATE utf8_spanish_ci,
  `direccion` text COLLATE utf8_spanish_ci,
  `fecha` date NOT NULL,
  `total` decimal(20,3) NOT NULL,
  `serie` bigint(20) NOT NULL,
  `numero` bigint(20) NOT NULL,
  PRIMARY KEY (`id_presupuesto`),
  UNIQUE KEY `presupuesto-serie-numero` (`serie`,`numero`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci AUTO_INCREMENT=1 ;
*/

/*!
 * \fn MPresupuesto::proximoComprobante()
 * Devuelve el proximo numero de comprobante, o el primer numero disponible si no se emitió ningun presupuesto anteriormente
 * \return NumeroComprobante conteniendo el numero y serie, o uno invalido si hubo un error
 */
NumeroComprobante &MPresupuesto::proximoComprobante() {
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT MAX( serie ) FROM presupuestos" ) ) ) {
      if( cola.next() ) {
          int serie = cola.record().value(0).toInt();
          if( cola.exec( QString( "SELECT MAX( numero ) FROM presupuestos WHERE serie = %1" ).arg( serie ) ) ) {
              if( cola.next() ) {
                  int numero = cola.record().value(0).toInt();
                  NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
                  num->siguienteNumero();
                  return *num;
              } else {
                  qDebug( "Error de cola al hacer next al obtener el numero de prespuesto maximo");
                  qDebug() <<  "Error: " << cola.lastError().number() << " - " << cola.lastError().text() << " - " << cola.lastQuery();
              }
          } else {
              qDebug( "Error de cola al hacer exec al obtener el numero de prespuesto maximo" );
              qDebug() <<  "Error: " << cola.lastError().number() << " - " << cola.lastError().text() << " - " << cola.lastQuery();
          }
      } else {
          qDebug( "Error de cola al hacer next al obtener el numero de serie de presupuesto maximo" );
          qDebug() <<  "Error: " << cola.lastError().number() << " - " << cola.lastError().text() << " - " << cola.lastQuery();
      }
  } else {
      NumeroComprobante *num = new NumeroComprobante( 0, 1, 0 );
      num->siguienteNumero();
      qDebug( "Error de cola al hacer exec al obtener el numero de serie de presupuesto maximo - Se inicio una nueva numeracion" );
      qDebug() <<  "Error: " << cola.lastError().number() << " - " << cola.lastError().text() << " - " << cola.lastQuery();
      return *num;
  }
  NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
  return *invalido;
}

/*!
 * \fn MPresupuesto::agregarPresupuesto( int id_cliente, QString texto_cliente, QString direccion, QDateTime fechahora, double total, const QString observaciones )
    Agrega un presupuesto con los datos pasados de parametros y devuelve el id del registro recien insertado para utilizar con los items
    @param id_cliente Identificador de cliente
    @param texto_cliente Nombre del cliente si id-cliente no es valido
    @param direccion Direccion del cliente o destinatario si el id-cliente no es valido
    @param fechahora Fecha y hora del presupeusto
    @param total Total del presupuesto
    @param observaciones Observaciones agregadas al presupuesto ( no include "Observaciones:" )
    @returns ID de insercion o -1 si hubo un error
 */
int MPresupuesto::agregarPresupuesto(int id_cliente, QString texto_cliente, QString direccion, QDateTime fechahora, double total, const QString observaciones ) {
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO presupuestos( id_cliente, destinatario, direccion, fecha, total, serie, numero, observaciones ) VALUES ( :id_cliente, :nombre, :direccion, :fecha, :total, :serie, :numero, :observaciones )") ) {
        qDebug( "Error al preparar la cola" );
        qDebug() << "Error: " << cola.lastError().number() << " - " << cola.lastError().text();
        return -1;
    }
    if( id_cliente < 0 ) {
        cola.bindValue( 0, QVariant( QVariant::Int ) );
        cola.bindValue( ":nombre", texto_cliente );
        cola.bindValue( ":direccion", direccion );
    } else {
        cola.bindValue( 0, id_cliente );
        cola.bindValue( ":nombre", QVariant( QVariant::String ) );
        cola.bindValue( ":direccion", QVariant( QVariant::String ) );
    }
    cola.bindValue( ":fecha", fechahora );
    cola.bindValue( ":total", total );
    // busco el proximo numero de serie
    NumeroComprobante num = this->proximoComprobante();
    cola.bindValue( ":serie", num.serie() );
    cola.bindValue( ":numero", num.numero() );
    if( observaciones.isNull() ) {
        cola.bindValue( ":observaciones", QVariant( QVariant::String ) );
    } else {
        cola.bindValue( ":observaciones", observaciones );
    }
    if( cola.exec() ) {
        // busco el ultimo id insetado
        QVariant ret = cola.lastInsertId();
        if( ret.isValid()){
            return ret.toInt();
        } else { return -1; }
    } else {
        qDebug( "Error al hacer exec para insertar un nuevo presupeusto.");
        qDebug() << "Error: " << cola.lastError().number() << " - " << cola.lastError().text() << " - " << cola.lastQuery();
        return -1;
    }
}

/*!
 * \fn MPresupuesto::modificarPresupuesto( int id_presupuesto, int id_cliente, QString texto_cliente, QString direccion, QDateTime fechahora, double total, const QString observaciones )
 * Modifica los datos del presupuesto pasado como parametro
 * \param id_presupuesto Identificar del presupuesto a modificar
 * \param id_cliente Identificador del cliente al cual está dirigido el presupuesto
 * \param texto_cliente Texto que debe aparecer en el item del presupuesto
 * \param direccion Dirección que aparecerá en el presupuesto
 * \param fechahora Fecha y Hora en que se generó el presupuesto
 * \param total Total del presupuesto
 * \param observaciones Observaciones agregadas al presupuesto
 * \returns Verdadero si no hubo error al guardar los datos del presupuesto.
 */
bool MPresupuesto::modificarPresupuesto( int id_presupuesto, int id_cliente, QString texto_cliente, QString direccion, QDateTime fechahora, double total, const QString observaciones )
{
    QSqlQuery cola;
    if( !cola.exec( QString(
                        "UPDATE presupuestos "
                        "SET `id_cliente` = %2,  "
                        "`destinatario` = '%3',  "
                        "`direccion` = '%4',  "
                        "`fecha` = '%5',  "
                        "`total` = %6,  "
                        "`observaciones` = '%7'  "
                        "WHERE id_presupuesto = %1 "
                        )
                    .arg( id_presupuesto )
                    .arg( id_cliente )
                    .arg( texto_cliente )
                    .arg( direccion )
                    .arg( fechahora.toString( Qt::ISODate ) )
                    .arg( total )
                    .arg( observaciones ) ) ) {
        qWarning( "Hubo un error al guardar los datos del presupuesto" );
        qDebug() << "Error de exec al actualizar los datos del presupuesto.";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    } else {
        return true;
    }
}

/*!
 * \fn MPresupuesto::eliminarPresupuesto( int id_presupuesto )
 *
 */
bool MPresupuesto::eliminarPresupuesto(int id_presupuesto)
{
    if( id_presupuesto <= 0 )
        return false;

    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
    QSqlQuery cola;
    // Elimino sus items
    if( !cola.exec( QString( "DELETE FROM item_presupuesto WHERE id_presupuesto = %1 LIMIT 1" ).arg( id_presupuesto ) ) ) {
        qWarning( ) << "No se pudo ejecutar la cola para eliminar los items de presupuesto";
        qDebug() << "Error al ejectuar la cola para eliminar el presupuesto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
        return false;
    }
    // Elimino sus descuentos
    // Busco los que correspondan
    if( cola.exec( QString( "SELECT id_descuento FROM descuento_comprobante WHERE id_comprobante = %1 AND tipo = %2" ).arg( id_presupuesto ).arg( MDescuentos::Presupuesto ) ) ) {
        QSqlQuery cola2;
        while( cola.next() ) {
            if( !cola2.exec( QString( "DELETE FROM descuento WHERE id_descuento = %1 LIMIT 1" ).arg( cola.record().value(0).toInt() ) ) ) {
                qWarning() << "No se pudo ejecutar la cola para eliminar los descuentos del presupuesto";
                qDebug() << "Error al ejectuar la cola para eliminar el presupuesto";
                qDebug() << cola2.lastError().text();
                qDebug() << cola2.lastQuery();
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                return false;
            }
        }
    }

    if( !cola.exec( QString( "DELETE FROM descuento_comprobante WHERE id_comprobante = %1 AND tipo = %2 LIMIT 1" ).arg( id_presupuesto ).arg( MDescuentos::Presupuesto ) ) ) {
        qWarning() << "No se pudo ejecutar la cola para eliminar los descuentos del presupuesto";
        qDebug() << "Error al ejectuar la cola para eliminar el presupuesto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
        return false;
    }
    // Elimino el presupuesto
    if( cola.exec( QString( "DELETE FROM presupuestos WHERE id_presupuesto = %1 LIMIT 1" ).arg( id_presupuesto ) ) ) {
        if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit() ) {
            return true;
        } else {
            qWarning( "Se pudo eliminar los datos pero no confirmarlos" );
            return false;
        }
    } else {
        qWarning() << "No se pudo ejecutar la cola para eliminar el presupuesto";
        qDebug() << "Error al ejectuar la cola para eliminar el presupuesto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
        return false;
    }

}

/*!
 * \fn MPresupuesto::obtenerFecha( int id_presupuesto )
 * Obtiene la fecha del presuupesto pasado como parametro. Si es incorrecto o hay error, la fecha será invalida.
 * \param id_presupuesto Identificador del presupuesto
 */
QDate MPresupuesto::obtenerFecha( int id_presupuesto )
{
    if( id_presupuesto < 1 ) {
        return QDate();
    }

    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha FROM presupuestos WHERE id_presupuesto = %1" ).arg( id_presupuesto ) ) ) {
        cola.next();
        return cola.record().value(0).toDate();
    } else {
        qDebug() << "Error al obtener la fecha del presupuesto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return QDate();
    }
}

/*!
 * \fn MPresupuesto::obtenerIdCliente( int id_presupuesto )
 * Obtiene el id de cliente del presuupesto pasado como parametro. Si es incorrecto o hay error, el id será menor que cero
 * \param id_presupuesto Identificador del presupuesto
 */
int MPresupuesto::obtenerIdCliente( int id_presupuesto )
{
    if( id_presupuesto < 1 )
        return -1;

    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_cliente FROM presupuestos WHERE id_presupuesto = %1" ).arg( id_presupuesto ) ) ) {
        cola.next();
        return cola.record().value(0).toInt();
    } else {
        qDebug() << "Error al obtener la fecha del presupuesto";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
}

