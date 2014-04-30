/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
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

#include "MFactura.h"
#include <QDate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QMessageBox>
#include <QSqlDriver>
#include <QDebug>

#include "NumeroComprobante.h"
#include "mitemfactura.h"
#include "mproductos.h"
#include "mproductostotales.h"
#include "eregistroplugins.h"
#include "mcuentacorriente.h"
#include "mitemcuentacorriente.h"
#include "mcajas.h"
#include "mmovimientoscaja.h"
#include "mdescuentos.h"

MFactura::MFactura(QObject *parent) :
QSqlRelationalTableModel(parent) {
    inicializar();
    relacionar();
}

void MFactura::inicializar() {
 setTable("factura");
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "#Cliente" );
 setHeaderData( 2, Qt::Horizontal, "Fecha Emision" );
 setHeaderData( 3, Qt::Horizontal, "Forma de Pago" );
 setHeaderData( 4, Qt::Horizontal, "#Serie" );
 setHeaderData( 5, Qt::Horizontal, "#Numero" );
}

void MFactura::relacionar() {
    setRelation( 1, QSqlRelation( "clientes", "id", "razon_social" ) );
}

/*
CREATE TABLE IF NOT EXISTS `factura` (
    `id_factura` bigint(20) NOT NULL AUTO_INCREMENT,
    `id_cliente` bigint(20) NULL,
    `fecha` datetime NOT NULL,
    `id_forma_pago` int(11) NOT NULL,
    `serie` int(5) NOT NULL,
    `numero` int(5) NOT NULL,
    `total` Decimal(20,3),
    `id_ctacte` bigint(20) NULL,
    `id_mov_caja` bigint(20) NULL,
    `anulada` boolean NOT NULL,
    `razon` text COLLATE utf8_spanish_ci,
    `fecha_cancelada` datetime DEFAULT NULL,
    `observaciones` tinytext DEFAULT NULL,
    PRIMARY KEY (`id_factura`),
    UNIQUE KEY `factura-serie-numero` (`serie`,`numero`),
    FOREIGN KEY (`id_cliente`) REFERENCES `clientes`( `id` ),
    FOREIGN KEY (`id_ctacte`) REFERENCES `item_ctacte`( `id_op_ctacte` ),
    FOREIGN KEY (`id_mov_caja`) REFERENCES `movimiento_caja`( `id_movimiento` )
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;
*/

/*!
 * \fn MFactura::agregarVenta( QDateTime fecha, int id_cliente, MFactura::FormaPago id_forma_pago, MProductosTotales *mcp )
 * Agrega el registro para una factura y devuelve el id o -1 si hubo un error.
 * \param fecha Fecha en que se registrará la venta
 * \param id_cliente ID del cliente al cual se le hace la venta
 * \param id_forma_pago Identificativo de la forma de pago
 * \param mcp Modelo de productos o items a facturar
 * \param observaciones Observaciones para agregar a la factura
 * \returns ID de la operaciòn o -1 si hubo un error.
 */
int MFactura::agregarVenta( QDateTime fecha, int id_cliente, MFactura::FormaPago id_forma_pago, MProductosTotales *mcp, QString observaciones )
{
 QSqlQuery cola;
 cola.prepare( "INSERT INTO factura( fecha, id_cliente, id_forma_pago, serie, numero, total, observaciones ) VALUES ( :fecha, :id_cliente, :id_forma_pago, :serie, :numero, :total, :observaciones  )" );
 cola.bindValue(":fecha", fecha );
 cola.bindValue( ":id_cliente", id_cliente );
 cola.bindValue( ":id_forma_pago", id_forma_pago );
 NumeroComprobante num = this->proximoComprobante();
 cola.bindValue( ":serie", num.serie() );
 cola.bindValue( ":numero", num.numero() );
 cola.bindValue( ":total", mcp->total() );
 if( observaciones.isNull() ) {
     cola.bindValue( ":observaciones", QVariant( QVariant::String ) );
 } else {
     cola.bindValue( ":observaciones", observaciones );
 }
 if( !cola.exec() )
 {
  qDebug() << "Error de insercion de registro de venta";
  qDebug() << QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( cola.lastError().type() ).arg( cola.lastError().number() ).arg( cola.lastError().text() );
  qDebug() << cola.lastQuery();
  return -1;
 }
 else
 {
  int id_venta = -1;
  if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
    id_venta = cola.lastInsertId().toInt();
  } else {
      if( cola.exec( "SELECT MAX( id_factura ) FROM factura" ) ) {
          cola.next();
          id_venta = cola.record().value(0).toInt();
      } else {
          qDebug() << "Error de obtencion del id del registro de venta";
          qDebug() << QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( cola.lastError().type() ).arg( cola.lastError().number() ).arg( cola.lastError().text() );
          qDebug() << cola.lastQuery();
          return -1;
      }
  }
  if( id_venta <= 0 ) {
      qWarning( "Identificador de factura incorrecto!" );
      QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
      return -1;
  }

  // Guardo los datos de el modelo

  // recorro el modelo y guardo los datos
  MItemFactura *mi = new MItemFactura();
  // Tengo que utilizar siempre el mismo objeto para evitar errores de indice de id_item_factura
  for( int i= 0; i<mcp->conteoItems(); i++ )
  {
   if( mi->agregarItemFactura( id_venta,
                               mcp->data( mcp->index( i, 0 ), Qt::EditRole ).toDouble(),
                               mcp->data( mcp->index( i, 1 ), Qt::DisplayRole ).toString(),
                               mcp->data( mcp->index( i, 2 ), Qt::EditRole ).toDouble(),
                               mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt() // Intento de que funcione el "id_producto"
                             ) ) {
        // veo si tengo que disminuir el stock
        if( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt() > 0 ) {
             if( !MProductos::modificarStock( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt(),
                                              mcp->data( mcp->index( i, 0 ), Qt::EditRole ).toDouble()*(-1.0) ) ) {
                 qDebug( "Error al disminuir el stock del producto" );
                 return -1;
             } else {
                 //qDebug( "Stock del producto disminuido correctamente" );
             }
        } else {
             //qDebug( "El item no es un producto" );
        }
   } else {
       qDebug( "Error - No se pudo agregar un item." );
       return -1;
   } // Fin if agregarItemFactura
  } // Fin del for items
  if( mcp->conteoDescuentos() > 0 ) {
     MDescuentos *md = new MDescuentos();
     for( int fila = mcp->conteoItems()-1; fila < mcp->conteoDescuentos(); fila ++ ) {
          if( !md->agregarDescuento( MDescuentos::Factura,
                                     id_venta,
                                     mcp->data( mcp->index( fila+1, 1 ), Qt::EditRole ).toString(),
                                     mcp->data( mcp->index( fila+1, 2 ), Qt::EditRole ).toDouble()
                                    ) ) {
              qDebug( "Error al ingresar el descuento en la base de datos" );
              QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
              return -1;
         }
     }
     delete md;
  }
  delete mi;
  mi = 0;
  double total_calculado = mcp->total();
  // Si la operación es a cuenta corriente, guardo los datos si esta activo el plugin de ctacte
  if( ERegistroPlugins::getInstancia()->existePluginExterno( "ctacte" ) && id_forma_pago == MFactura::CuentaCorriente )
  {
   //qDebug( "Venta a cuenta corriente" );
   // Si se ingresa aqui el cliente tiene cuenta corriente
   QString num_comprobante = this->obtenerComprobante().aCadena();
   // Busco el numero de cuenta
   QString num_ctacte = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
   switch( MCuentaCorriente::verificarSaldo( num_ctacte, total_calculado ) )
   {
         case MCuentaCorriente::LimiteExcedido:
         {
                 QMessageBox::information( 0, "Limite de Saldo Excedido", "El limite de saldo para este cliente ha sido excedido. No se hara la factura" );
                 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                 return -1;
                 break;
         }
         case MCuentaCorriente::EnLimite:
         {
                 QMessageBox::information( 0, "Limite de Saldo Alcanzado", "El limite de saldo para este cliente ha sido alcanzado." );
                 break;
         }
         case MCuentaCorriente::ErrorBuscarLimite:
         {
                 QMessageBox::information( 0, "Error", "No se pudo encontrar la cuenta corriente para el cliente buscado. No se registrará la venta." );
                 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                 return -1;
                 break;
         }
         case MCuentaCorriente::LimiteCorrecto:
         {
                 break;
         }
         default:
         {
                 QMessageBox::information( 0, "Error", "Error desconocido al verificar el saldo. No se registrará la venta." );
                 return -1;
                 break;
         }
   }
   if( MItemCuentaCorriente::agregarOperacion(    num_ctacte,
                                                  num_comprobante,
                                                  id_venta,
                                                  MItemCuentaCorriente::Factura,
                                                  fecha.date(),
                                                  QString("Venta a Cuenta Corriente - Factura %1" ).arg( num_comprobante ),
                                                  total_calculado ) == -1 )
   {
       qWarning( "Error al actualizar la cuenta corriente - inserccion de item" );
       return -1;
   } else  {
       //qDebug( "Se actualizó correctamente la cuenta corriente del cliente" );
   }
  } else {
      qDebug( "La factura no fue a ctacte o no existe el plugin de ctacte cargado" );
  }
  // Veo si fue en efectivo
  if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) && id_forma_pago == MFactura::Contado ) {
      // Agrego el item de caja
      MMovimientosCaja *m = new MMovimientosCaja();
      if( !m->agregarMovimiento( MCajas::cajaPredeterminada(), QString( "Pago de factura %1" ).arg( num.aCadena() ), QString(), total_calculado ) ) {
          qDebug( "Error al agregar el movimiento de caja cuando se pago una factura en contado.");
          return -1;
      }
      delete m;
  }
  qDebug() << QString( "Factura ingresada correctamente: id = %1" ).arg( id_venta );
  return id_venta;
 }
 return -1;
}

/*!
 * \fn MFactura::agregarFactura( const int id_cliente, const QDateTime fecha, MFactura::FormaPago id_forma_pago, const double total, bool registrar_operacion )
 * Agrega un registro en la tabla de facturas para las facturas de servicios.
 * \param id_cliente Identificador de cliente.
 * \param fecha Fecha de la factura.
 * \param id_forma_pago Forma de pago de la factura
 * \param total Total a facturar
 * \param registrar_operacion muestra si se debe registrar o no la operacion ( util si no se desea actualiza ral cuenta corriente )
 * \returns Identificador de la factura o -1 si hubo un error.
 */
int MFactura::agregarFactura( const int id_cliente, const QDateTime fecha, MFactura::FormaPago id_forma_pago, const double total, bool registrar_operacion ) {
    if( total <= 0.0 ) { qDebug( "El total no puede ser <= 0" ); return -1; }
    QSqlQuery cola;
    cola.prepare( "INSERT INTO factura( fecha, id_cliente, id_forma_pago, serie, numero, total ) VALUES ( :fecha, :id_cliente, :id_forma_pago, :serie, :numero, :total )" );
    cola.bindValue(":fecha", fecha );
    cola.bindValue( ":id_cliente", id_cliente );
    cola.bindValue( ":id_forma_pago", id_forma_pago );
    NumeroComprobante num = this->proximoComprobante();
    cola.bindValue( ":serie", num.serie() );
    cola.bindValue( ":numero", num.numero() );
    cola.bindValue( ":total", total );
    if( !cola.exec() )
    {
     qDebug( "Error de insercion de registro de venta" );
     qDebug() << QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( cola.lastError().type() ).arg( cola.lastError().number() ).arg( cola.lastError().text() );
     return -1;
    }
    else
    {
     qDebug( "Registro de factura agregado correctamente" );
     int id_venta = cola.lastInsertId().toInt();
     qDebug( "ID obtenido" );
     // Si la operación es a cuenta corriente, guardo los datos si esta activo el plugin de ctacte
     if( ERegistroPlugins::getInstancia()->existePluginExterno( "ctacte" ) && id_forma_pago == MFactura::CuentaCorriente && registrar_operacion )
     {
      // Si se ingresa aqui el cliente tiene cuenta corriente
      QString num_comprobante = this->obtenerComprobante().aCadena();
      // Busco el numero de cuenta
      QString num_ctacte = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
      switch( MCuentaCorriente::verificarSaldo( num_ctacte, total ) )
      {
            case MCuentaCorriente::LimiteCorrecto:
            {
                    qDebug( "Cuenta Corriente reporta limite correcto." );
                    break;
            }
            case MCuentaCorriente::LimiteExcedido:
            {
                    QMessageBox::information( 0, "Limite de Saldo Excedido", "El limite de saldo para este cliente ha sido excedido. No se hara la factura" );
                    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                    return -1;
                    break;
            }
            case MCuentaCorriente::EnLimite:
            {
                    QMessageBox::information( 0, "Limite de Saldo Alcanzado", "El limite de saldo para este cliente ha sido alcanzado." );
                    break;
            }
            case MCuentaCorriente::ErrorBuscarLimite:
            {
                    QMessageBox::information( 0, "Error", "No se pudo encontrar la cuenta corriente para el cliente buscado. No se registrará la venta." );
                    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                    return -1;
                    break;
            }
            default:
            {
                    QMessageBox::information( 0, "Error", "Error desconocido al verificar el saldo. No se registrara la venta." );
                    qDebug( "El metodo MCuentaCorriente::verificarSaldo() retorno un valor desconocido" );
                    return -1;
                    break;
            }
      }
      if( MItemCuentaCorriente::agregarOperacion(   num_ctacte,
                                                     num_comprobante,
                                                     id_venta,
                                                     MItemCuentaCorriente::Factura,
                                                     fecha.date(),
                                                     "Venta a Cuenta Corriente",
                                                     total ) == -1 )
      { qWarning( "Error al actualizar la cuenta corriente - inserccion de item" ); return -1; }

      // Veo si fue en efectivo
     } else if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja " ) && id_forma_pago == MFactura::Contado && registrar_operacion ) {
         // Agrego el item de caja
         MMovimientosCaja *m = new MMovimientosCaja();
         if( !m->agregarMovimiento( MCajas::cajaPredeterminada(), "Pago de factura %1", QString(), total ) ) {
             qDebug( "Error al agregar el movimiento de caja cuando se pago una factura en contado.");
             return -1;
         }
         delete m;
     }
     return id_venta;
    }
}

/*!
 * \fn MFactura::proximoComprobante()
 * Devuelve un objeto NumeroComprobante conteniendo el proximo numero de serie y comprobante que corresponde.
 */
NumeroComprobante &MFactura::proximoComprobante() {
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT MAX( serie ) FROM factura" ) ) ) {
      if( cola.next() ) {
          int serie = cola.record().value(0).toInt();
          if( serie == 0 ) {
              serie = 1;
          }
          if( cola.exec( QString( "SELECT MAX( numero ) FROM factura WHERE serie = %1" ).arg( serie ) ) ) {
              if( cola.next() ) {
                  int numero = cola.record().value(0).toInt();
                  NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
                  num->siguienteNumero();
                  return *num;
              } else {
                  qDebug( "Error de cola al hacer next al obtener el numero de factura maximo");
              }
          } else {
              qDebug( "Error de cola al hacer exec al obtener el numero de factura maximo" );
          }
      } else {
          qDebug( "Error de cola al hacer next al obtener el numero de serie de factura maximo -  Se inicio una nueva numeracion" );
      }
  } else {
      NumeroComprobante *num = new NumeroComprobante( 0, 1, 0 );
      num->siguienteNumero();
      qDebug( "Error de cola al hacer exec al obtener el numero de serie de factura maximo - Se inicio una nueva numeracion" );
      return *num;
  }
  NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
  return *invalido;
}

/*!
 * \brief MFactura::obtenerComprobante
 * \return
 */
NumeroComprobante & MFactura::obtenerComprobante() {
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT MAX( serie ) FROM factura" ) ) ) {
      if( cola.next() ) {
          int serie = cola.record().value(0).toInt();
          if( cola.exec( QString( "SELECT MAX( numero ) FROM factura WHERE serie = %1" ).arg( serie ) ) ) {
              if( cola.next() ) {
                  int numero = cola.record().value(0).toInt();
                  NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
                  return *num;
              } else {
                  qDebug( "Error de cola al hacer next al obtener el numero de factura maximo");
              }
          } else {
              qDebug( "Error de cola al hacer exec al obtener el numero de factura maximo" );
          }
      } else {
          qDebug( "Error de cola al hacer next al obtener el numero de serie de factura maximo -  Se inicio una nueva numeracion" );
      }
  } else {
      NumeroComprobante *num = new NumeroComprobante( 0, 1, 0 );
      num->siguienteNumero();
      qDebug( "Error de cola al hacer exec al obtener el numero de serie de factura maximo - Se inicio una nueva numeracion" );
      return *num;
  }
  NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
  return *invalido;
}

/*!
 * \brief MFactura::obtenerComprobante
 * \param id_factura
 * \return
 */
NumeroComprobante & MFactura::obtenerComprobante( const int id_factura ) {
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT serie, numero FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
    if( cola.next() ) {
        int serie = cola.record().value(0).toInt();
        int numero = cola.record().value(1).toInt();
        NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
        return *num;
    } else {
        qDebug( "Error de cola al hacer next para obtener el numero de comprobante de factura");
    }
  } else {
    qDebug( "Error de cola al hacer exec para obtener el numero de comprobante maximo" );
 }
  NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
  return *invalido;
}

/*!
 * \fn MFactura::anularFactura( const int id_factura, QString razon, QDateTime fechahora )
 * Metodo para anular una factura emitida.
 * \param id_factura Identificador de la factura que se desea anular
 * \param razon Razon de facturación.
 * \param fechahora Fecha y hora en que se anulo la factura.
 * \return Verdadero si se pudo eliminar, falso en caso de error.
 */
bool MFactura::anularFactura( const int id_factura, QString razon, QDateTime fechahora ) {
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
    QSqlQuery cola;
    // Busco los datos de la factura.
    if( cola.exec( QString( "SELECT id_forma_pago, total, serie, numero, id_cliente FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
        if( cola.next() ) {
            int forma_pago = cola.record().value(0).toInt();
            double total = cola.record().value(1).toDouble();
            NumeroComprobante num( 0, cola.record().value(2).toInt(), cola.record().value(3).toInt() );
            int id_cliente = cola.record().value(4).toInt();
            // Segun lo que sea la forma de pago actuo
            switch( forma_pago ) {
                case MFactura::Contado: {
                    if( ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) ) {
                        // Genero la operación de caja que contraarreste
                        MMovimientosCaja *n = new MMovimientosCaja(0);
                        if( ! n->agregarMovimiento( MCajas::cajaPredeterminada(),
                                                    QString( "Cancelacion de la factura %1" ).arg( num.aCadena() ),
                                                    QString(),
                                                    total ) ) {
                            qDebug( "MFactura::anularFactura::No se pudo agregar el movimiento de caja" );
                            delete n;
                            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                            return false;
                        }
                        delete n;
                    } else {
                        qDebug() << "La factura era a contado pero no está cargado el plugin -> se jode";
                    }
                    break;
                }
                case MFactura::CuentaCorriente: {
                    if( ERegistroPlugins::getInstancia()->existePluginExterno( "ctacte" ) ) {
                        // Genero la entrada para contraarrestar
                        QString num_cuenta = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
                        if( num_cuenta == QString::number( MCuentaCorriente::ErrorClienteInvalido ) ) {
                            qDebug( "Error, el cliente es invalido" );
                            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                            return false;
                        } else if( num_cuenta == QString::number( MCuentaCorriente::ErrorNumeroCuenta ) ) {
                            qDebug( "El cliente no posee cuenta corriente" );
                        } else {
                            if( ! MItemCuentaCorriente::agregarOperacion( num_cuenta,
                                                                          num.aCadena(),
                                                                          id_factura,
                                                                          MItemCuentaCorriente::AnulacionFactura,
                                                                          fechahora.date(),
                                                                          QString::fromUtf8( "Anulación de la factura %1" ).arg( num.aCadena() ),
                                                                          (-1.0) * total ) ) {
                                qDebug( "MFactura::anularFactura::No se pudo agregar el movimiento de cuenta corriente" );
                                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                                return false;
                            }
                        }
                    } else {
                        qDebug() << "La factura era a cuenta corriente pero no está cargado y no s descuenta de la cuenta corriente.";
                    }
                    break;
                }
            }
            // cancelo la factura
            cola.prepare( QString( "UPDATE factura SET anulada = :anulada, razon = :razon, fecha_cancelada = :fechahora WHERE id_factura = %1 " ).arg( id_factura ) );
            cola.bindValue( ":anulada", true );
            cola.bindValue( ":razon", razon );
            cola.bindValue( ":fechahora", fechahora );
            if( cola.exec()) {
                qDebug( "Factura cancelada correctamente" );
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit();
                return true;
            } else {
                qDebug( "MFactura::anularFactura::Error al ejecutar la cola de anulación de la factura seleccionada." );
                qDebug() << cola.lastError().text();
                qDebug() << cola.lastQuery();
            }
        } else {
            qDebug( "MFactura::anularFactura::Error al hacer next al buscar informacion de la factura" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug( "MFactura::anularFactura::Error al hacer next al buscar informacion de la factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
    return false;
}

/*!
 * \fn MFactura::idFacturaPorComprobante( const QString numero )
 * Devuleve el id de la factura que corresponde al numero pasado como parametro o -1 si hubo un error o no existe.
 * \param numero Numero de factura de la forma 99999-99999
 * \return -1 en caso de error, id en caso correcto
 */
int MFactura::idFacturaPorComprobante( const QString numero )
{
 QSqlQuery cola;
 NumeroComprobante *n = new NumeroComprobante( 0, -1, -1 );
 n->desdeString( numero );
 if( !n->esValido() )  {
     qDebug( "El numero de comprobante es invalido" );
     delete n;
     return -1;
 }
 // Numero de comprobante valido
 if( cola.exec( QString( "SELECT id_factura FROM factura WHERE serie = %1 AND numero = %2" ).arg( n->serie() ).arg( n->numero() ) ) ) {
     if( cola.next() ) {
         int valor = cola.record().value(0).toInt();
         if( valor > 0 ) {
             delete n;
             return valor;
         } else {
             qDebug( "Error de numero de id al buscar factura por numero de comprobante" );
         }
     } else {
         qDebug( "Error al hacer next en el id de factura según comprobante" );
     }
 } else {
     qDebug( "Error al hacer exec en el id de factura según comprobante" );
     qDebug() << cola.lastError().text();
     qDebug() << cola.lastQuery();
 }
 delete n;
 return -1;
}

/*!
 * \fn MFactura::fechaUltimaVenta()
 * Devuelve la fecha de la ultima factura emitida
 * \returns QDate con la fecha o un QDate invalido si existió un error
 */
QDate MFactura::fechaUltimaVenta()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT fecha FROM factura ORDER BY id_factura DESC LIMIT 1" ) ) {
        cola.next();
        return cola.record().value(0).toDate();
    } else {
        qWarning( "Error al buscar la fecha de la ultima factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}

/*!
 * \fn MFacura::obtenerFecha( const int id_factura )
 * Devuelve la fecha de la factura pasada con parametro
 * \param id_factura identificador de factura
 */
QDate MFactura::obtenerFecha(const int id_factura)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
        cola.next();
        return cola.record().value(0).toDate();
    } else {
        qWarning( "Error al buscar la fecha de la factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}

/*!
 * \fn MFactura::obtenerTotal( const int id_factura )
 * Devuelve el total de la factura pasada como parametro
 * \param id_factura Identificador de la factura
 */
double MFactura::obtenerTotal( const int id_factura )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT total FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
        cola.next();
        return cola.record().value(0).toDouble();
    } else {
        qWarning( "Error al buscar la fecha de la factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1.0;
}

/*!
 * \brief MFactura::obtenerIdCliente
 * Devuelve el identificador del cliente que tiene asociada la factura
 * \param id_factura Identificador de la factura a buscar
 * \return Identificador del cliente o -1 si hubo error
 */
int MFactura::obtenerIdCliente(const int id_factura)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_cliente, id_factura FROM factura WHERE id_factura = %1" ).arg( id_factura ) ) ) {
        cola.next();
        return cola.record().value(0).toInt();
    } else {
        qWarning() << "Error al buscar el cliente de la factura";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}

/*!
 * \brief MFactura::obtenerFechaMinimaVenta
 * Devuelve la fecha de la primera factura
 * \return Fecha de la primera factura
 */
QDate MFactura::obtenerFechaMinimaVenta()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT fecha FROM factura ORDER BY fecha ASC LIMIT 1" ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        }
    } else {
        qWarning( "Error al buscar la fecha de la factura minima" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate( 1900, 1, 1);
}
