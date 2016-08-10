/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   *
 *   juiraze@yahoo.com.ar   *
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
#include "mclientes.h"

#include "util.h"
#include "preferencias.h"

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QSqlDriver>

MClientes::MClientes( QObject *parent )
 : QSqlRelationalTableModel( parent )
{
 setTable( "clientes" );
/*
-- Describe CLIENTES
CREATE TABLE IF NOT EXISTS `clientes` (
    `id` bigint(10) NOT NULL auto_increment,
    `razon_social` tinytext NOT NULL,
    `nombre` tinytext,
    `apellido` tinytext,
    `calle` tinytext,
    `numero` int(2) default NULL,
    `piso` int(2) default NULL,
    `depto` tinytext,
    `ciudad` tinytext,
    `codigo_postal` tinytext,
    `provincia` tinytext,
    `pais` tinytext,
    `tel_fijo` tinytext,
    `tel_celular` tinytext,
    `fax` tinytext,
    `email` tinytext,
    `comprobante_email` tinyint(1) default '1',
    `ctacte` tinyint(1) default NULL,
    `CUIT/CUIL` tinytext default null,
    `id_estado_fiscal` INT NULL
);
*/
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Razon Social" );
 setHeaderData( 2, Qt::Horizontal, "Nombre" );
 setHeaderData( 3, Qt::Horizontal, "Apellido" );
 setHeaderData( 4, Qt::Horizontal, "Calle" );
 setHeaderData( 5, Qt::Horizontal, "Numero" );
 setHeaderData( 6, Qt::Horizontal, "Piso" );
 setHeaderData( 7, Qt::Horizontal, "Depto" );
 setHeaderData( 8, Qt::Horizontal, "Ciudad" );
 setHeaderData( 9, Qt::Horizontal, "Codigo Postal" );
 setHeaderData( 10, Qt::Horizontal, "Provincia" );
 setHeaderData( 11, Qt::Horizontal, "Pais" );
 setHeaderData( 12, Qt::Horizontal, "Telefono" );
 setHeaderData( 13, Qt::Horizontal, "Celular" );
 setHeaderData( 14, Qt::Horizontal, "Fax" );
 setHeaderData( 15, Qt::Horizontal, "Em@il" );
 setHeaderData( 16, Qt::Horizontal, "EnviarComprobantexEmail" );
 setHeaderData( 17, Qt::Horizontal, "CUIT/CUIL" );
 setHeaderData( 18, Qt::Horizontal, "Estado Fiscal" );
}

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
QString MClientes::direccionEntera(int id_cliente) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT calle, numero, piso, depto, ciudad, codigo_postal, provincia FROM clientes WHERE id = %1 ").arg( id_cliente ) ) ) {
        if( cola.next() ) {
            QString ret;
            if( !cola.record().value(0).isNull() ) {
                ret.append( cola.record().value(0).toString() );
            }
            if( !cola.record().value(1).isNull() && !cola.record().value(1).toString().isEmpty() ) {
                ret.append( " " + cola.record().value(1).toString() );
            }
            if( !cola.record().value(2).isNull() & !cola.record().value(2).toString().isEmpty() ) {
                ret.append( " P " + cola.record().value(2).toString() );
            }
            if( !cola.record().value(3).isNull() && !cola.record().value(3).toString().isEmpty() ) {
                ret.append( " dept " + cola.record().value(3).toString() );
            }
            if( !cola.record().value(4).isNull() && !cola.record().value(4).toString().isEmpty() ) {
                ret.append( " - " + cola.record().value(4).toString() );
            }
            /*if( !cola.record().value(5).isNull()) {
                ret.append( " - CP " + cola.record().value(5).toString() );
            }
            if( !cola.record().value(6).isNull()) {
                ret.append( " - " + cola.record().value(6).toString() );
            }*/
            return ret;
        } else {
            qDebug() << "Error haciendo next en la cola para obtener la direcicon de un cliente";
            qDebug() << cola.lastError().text();
            return "Desconocido";
        }
    } else {
        qDebug() << "Error haciendo exec en la cola para obtener la direccion de un cliente";
        qDebug() << cola.lastError().text();
        return "Desconocido";
    }
}

Qt::ItemFlags MClientes::flags( const QModelIndex &/*index*/ ) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/*!
 * \fn MClientes::tieneDatosRelacionados( const int id_cliente )
 * Verifica si existe alguna asociacion en alguna tabla que corresponda con el cliente pasado como paraemtro
 * \param id_cliente ID de cliente
 * \return Verdadero si hay alguna asociación
 */
bool MClientes::tieneDatosRelacionados( const int id_cliente )
{
 /// Busca si posee relaciones
 /* Elementos Posibles:
   - Presupuestos.
   - Cuentas corrientes.
   - Facturas.
   - Recibos.
   - Servicios Asociados.
  */
  // Veo si es consumidor final, no se puede eliminar
  if (id_cliente == 0) {
      return true;
  }
  QSqlQuery cola;
  /////////////////////////////////////////////////////
  // Cuenta corriente
  if( cola.exec( QString( "SELECT COUNT(id_cliente) FROM ctacte WHERE id_cliente = %1" ).arg( id_cliente ) ) ) {
      cola.next();
      if( cola.record().value(0).toInt() > 0 ) {
         qWarning( "Este cliente posee una cuenta corriente activa." );
         return true;
      }
  } else {
      qDebug() << "Error al ejecutar la cola de contar la cantidad de clientes que hay en las cuentas corrientes";
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  /////////////////////////////////////////////////////
  // Presupuestos
  if( cola.exec( QString( "SELECT COUNT(id_cliente) FROM presupuestos WHERE id_cliente = %1" ).arg( id_cliente ) ) ) {
      cola.next();
      if( cola.record().value(0).toInt() > 0 ) {
          qWarning( "Este cliente posee presupuestos echos a su nombre" );
          return true;
      }
  } else {
      qDebug() << "Error al ejecutar la cola de contar la cantidad de clientes que hay en los presupuestos";
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  /////////////////////////////////////////////////////
  // Factura
  if( cola.exec( QString( "SELECT COUNT(id_cliente) FROM factura WHERE id_cliente = %1" ).arg( id_cliente ) ) ) {
      cola.next();
      if( cola.record().value(0).toInt() > 0 ) {
          qWarning( "Este cliente posee facturas emitidas a su nombre" );
          return true;
      }
  } else {
      qDebug() << "Error al ejecutar la cola de contar la cantidad de clientes que hay en las facturas";
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  /////////////////////////////////////////////////////
  // Recibos
  if( cola.exec( QString( "SELECT COUNT(id_cliente) FROM recibos WHERE id_cliente = %1" ).arg( id_cliente ) ) ) {
      cola.next();
      if( cola.record().value(0).toInt() > 0 ) {
          qWarning( "Este cliente posee recibos a su nombre" );
          return true;
      }
  } else {
      qDebug() << "Error al ejecutar la cola de contar la cantidad de clientes que hay en las cuentas corrientes";
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  /////////////////////////////////////////////////////
  // Servicios Asociados
  if( cola.exec( QString( "SELECT COUNT(id_cliente) FROM servicios_clientes WHERE id_cliente = %1").arg( id_cliente ) ) ) {
      cola.next();
      if( cola.record().value(0).toInt() > 0 ) {
          qWarning( "Este cliente posee algún servicio asociado" );
          return true;
      }
  } else {
      qDebug() << "Error al ejecutar la cola de contar la cantidad de clientes que hay en las cuentas corrientes";
      qDebug() << cola.lastError().text();
      qDebug() << cola.lastQuery();
  }
  // Si llego hasta aca no hay datos relacionados
  return false;
}

/*!
 * \brief MClientes::agregarClientePredeterminado
 * Agrega un cliente predeterminado sacando sus propiedades de las preferencias y extrapolando el nombre desde el parametro
 * \param texto
 * \return
 */
int MClientes::agregarClientePredeterminado(const QString razon_social , QString nombre, QString apellido, QString direccion, QString telefono, int id_estado_fiscal )
{
    if( razon_social.isEmpty() || razon_social.isNull() ) {
        qWarning() << "El parametro de texto no puede ser nulo";
        return -1;
    }
    // Busco las preferencias de usuario
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Clientes" );
    int id_provincia = p->value( "provincia", 0 ).toInt();
    int id_pais = p->value( "pais", 0 ).toInt();
    if( id_estado_fiscal == -1 ) {
        id_estado_fiscal = p->value( "estado-fiscal", 0 ).toInt();
    }
    p->endGroup(); p->endGroup(); p=0;

    // Intento extrapolar el texto
    if( apellido.isEmpty() && nombre.isEmpty() && razon_social.contains( ',' ) ) {
        QStringList salida = razon_social.split( ',', QString::KeepEmptyParts );
        // Supongo que el apellido está primero
        apellido = salida.takeFirst();
        nombre = salida.takeLast();
    }

    // Intento agregar el cliente con los datos
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO clientes( razon_social , nombre , apellido , direccion , telefono , provincia    , pais    , id_estado_fiscal  ) "
                       "             VALUES ( :razon_social, :nombre, :apellido, :direccion, :telefono, :id_provincia, :id_pais, :id_estado_fiscal )" ) ) {
        qWarning() << "No se pudo preparar la cola para insertar el cliente predeterminado";
        return -1;
    }
    cola.bindValue( ":razon_social", razon_social );
    cola.bindValue( ":nombre", nombre );
    cola.bindValue( ":apellido", apellido );
    cola.bindValue( ":direccion", direccion );
    cola.bindValue( ":telefono", telefono );
    cola.bindValue( ":id_provincia", id_provincia );
    cola.bindValue( ":id_pais", id_pais );
    cola.bindValue( ":id_estado_fiscal", id_estado_fiscal );
    if( cola.exec() ) {
        // Busco el ID que se inserto
        if( cola.driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
            return cola.lastInsertId().toInt();
        } else {
            if( cola.exec( "SELECT MAX(id_cliente) FROM clientes" ) ) {
                if( cola.next() ) {
                    return cola.record().value(0).toInt();
                } else {
                    qDebug() << "Error al hacer next para averiguar el id del cliente";
                }
            } else {
                qDebug() << "Error al ejecutar la cola para averiguar el id del cliente insertado";
            }
        }
    } else {
        qDebug() << "Error al intentar ejecutar la cola de inserción de datos de cliente predeterminado";
    }
    qDebug() << "Error al intentar ejecutar la cola de inserción de datos de cliente predeterminado";
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return -1;
}

/*!
 * \brief MClientes::getRazonSocial
 * Devuelve la razón social del cliente pasado como parametro
 * \param id_cliente Identificador del cliente
 * \return Razon social del cliente o Desconocido
 */
QString MClientes::getRazonSocial( const int id_cliente )
{
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


/*!
 * \fn MClientes::existe( QString razon_social, QString nombre )
 * Devuelve verdadero si ya se encuentra un cliente con la razon social pasada como parametro
 * \param razon_social Razon Social a buscar
 * \param nombre Nombre del cliente
 * \returns verdadero si existe un cliente, falso en caso de error o que no exista
 */
bool MClientes::existe( QString razon_social, QString /*nombre*/ ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id) FROM clientes WHERE %1 = \"%2\"" ).arg( Util::funcionComparacionSQL( "razon_social" ) ).arg( Util::comparacionSQL( razon_social ) ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() != 0 )
                return true;
    } else {
        qDebug() << "Error al ejecutar la cola de contar la cantidad de clientes que hay en las cuentas corrientes";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}
