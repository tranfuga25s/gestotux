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
#include "mpagos.h"

#include <QDate>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QDebug>

#include "eregistroplugins.h"
#include "../caja/mmovimientoscaja.h"
#include "../caja/mcajas.h"
#include "../CtaCte/mcuentacorriente.h"
#include "../CtaCte/mitemcuentacorriente.h"
#include "../../src/mclientes.h"

MPagos::MPagos(QObject *parent, bool relaciones )
 : QSqlRelationalTableModel(parent)
{
 inicializar();
 if( relaciones ) { relacionar(); }
}

/*!
 * \fn MPagos::inicializar()
 * Inicializa el modelo
 */
void MPagos::inicializar()
{
    setTable( "recibos" );
    setHeaderData( 0, Qt::Horizontal, "#ID" );
    setHeaderData( 1, Qt::Horizontal, "Cliente" );
    setHeaderData( 2, Qt::Horizontal, "Fecha Pago" );
    setHeaderData( 3, Qt::Horizontal, "Texto" );
    setHeaderData( 4, Qt::Horizontal, "Cantidad" );
    setHeaderData( 5, Qt::Horizontal, "Forma Pago" );
    setHeaderData( 6, Qt::Horizontal, "Cancelado" );
    setHeaderData( 7, Qt::Horizontal, "Pagado" ); // Campo utilizado en hicomp
    setHeaderData( 8, Qt::Horizontal, "#Serie" );
    setHeaderData( 9, Qt::Horizontal, "#Numero" );
    setHeaderData( 10, Qt::Horizontal, "ID mov caja" );
}

/*!
 * \fn MPagos::relacionar()
 * Inicializa las relaciones del  modelo
 */
void MPagos::relacionar()
{
    setRelation( 1, QSqlRelation( "clientes", "id", "razon_social" ) );
    //setRelation( 9, QSqlRelation( "movimientos_caja", "id_movimiento", "descripcion" ) );
}

QVariant MPagos::data(const QModelIndex& item, int role) const
{
 switch( role )
 {
  case Qt::DisplayRole:
  {
   switch( item.column() )
   {
     case 0: // ID
     { return QString( "#%L1" ).arg( QSqlRelationalTableModel::data( item, role ).toInt() ); break;}
     case 1: // Cliente
     { return QSqlRelationalTableModel::data( item, role ).toString(); break; }
     case 2: // fecha de pago
     { return QSqlRelationalTableModel::data( item, role ).toDate(); break; }
     case 3: // Texto
     { return QSqlRelationalTableModel::data( item, role ).toString(); break; }
     case 4: // Cantidad pagada
     { return QString( "$ %L1" ).arg( QSqlRelationalTableModel::data( item, role ).toDouble() ); break; }
     case 5:
     {
       switch( QSqlRelationalTableModel::data( item, role ).toInt() )
       {
        case Efectivo:
        { return "Efectivo"; break; }
        case Otro:
        { return "Otro"; break; }
       }
       break;
     }
     case 6: // Cancelado ( anulado ) y pagado
     case 7:
     //{ return QSqlRelationalTableModel::data( item, role ).toBool(); break; }
     {
             if( QSqlRelationalTableModel::data( item, role ).toBool() ) {
                return "Si";
             } else {
                return "No";
             }
             break;
     }
     case 8:
     case 9:
     { return QSqlRelationalTableModel::data( item, role ).toInt(); }
     default:
     { return QSqlRelationalTableModel::data( item, role ); break; }
   }
   break;
  }
  case Qt::TextAlignmentRole:
  {
   switch( item.column() )
   {
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    { return int( Qt::AlignCenter | Qt::AlignVCenter ); break; }
    default:
    { return QSqlRelationalTableModel::data( item, role ); break; }
   }
   break;
  }
  default:
  {
   return QSqlRelationalTableModel::data( item, role );
   break;
  }
 }
 return QSqlRelationalTableModel::data(item, role);
}

/*!
   \fn MPagos::buscarUltimoNumeroRecibo() const
   Busca el maximo ID de la tabla de recibos
   \return ID del ultimo recibo o -1 si ocurrio un error
 */
int MPagos::buscarUltimoNumeroRecibo() const
{
  // Busco el maximo de los recibos
  QSqlQuery cola( QString( "SELECT MAX(id_recibo) FROM %1" ).arg( this->tableName() ) );
  if( cola.exec() ) {
      if( cola.next() ) {
          return cola.record().value(0).toInt();
      } else {
          qDebug() << "MPagos::buscarUltimoNumeroRecibo::Error al hacer next en la cola";
          qDebug() << this->lastError().text();
          return -1;
      }
  } else {
      qDebug() << "MPagos::buscarUltimoNumeroRecibo::Error al hacer exec de la cola";
      qDebug() << this->lastError().text();
      return -1;
  }
}

/*!
   \fn MPagos::buscarUltimoNumeroRecibo() const
   Busca la fecha mas tardia de cualquier recibo
   \return Objeto QDate valido con la fecha, o invalido si no se pudo ejecutar la consulta
 */
QDate MPagos::buscarFechaUltimoRecibo() const
{
  // Busco la fecha maxima de los recibos
  QSqlQuery cola( QString( "SELECT fecha_pago FROM recibos ORDER BY fecha_pago DESC LIMIT 1" ) );
  if( cola.exec() ) {
      if( cola.next() ) {
          return cola.record().value(0).toDate();
      } else {
          qDebug() << "MPagos::buscarFechaUltimoRecibo::Error al hacer next en la cola";
          qDebug() << this->lastError().text();
          return QDate();
      }
  } else {
      qDebug() << "MPagos::buscarFechaUltimoRecibo::Error al hacer exec de la cola";
      qDebug() << this->lastError().text();
      return QDate();
  }
}

/*!
  \fn MPagos::agregarRecibo( const int id_cliente, const QDate fecha, const QString contenido, const double total, const bool efectivo, const bool pagado )
  Agrega un nuevo recibo con los datos pasados como parametro y devuelve el numero de recibo insertado o -1 si ocurrio un error
  @param id_cliente ID de la tabla cliente del cliente en cuestion
  @param fecha Fecha de emisión del recibo
  @param contenido Texto o contenido html que ira en el recibo
  @param total Total pagado en este recibo
  @param pagado Coloca como pagado el recibo
  @param efectivo indica si el recibo fue pagado en efectivo
  @return Numero de recibo emitido o -1 en caso de cualquier error
*/
int MPagos::agregarRecibo(int id_cliente, QDate fecha, QString contenido, double total, bool efectivo, bool pagado , QString texto_ctacte )
{
    int id_caja = -1;
    if( efectivo && pagado ) {
        // Genero la entrada en la caja
        if( ERegistroPlugins::getInstancia()->existePluginExterno("caja") ) {
            int pid_caja = MCajas::cajaPredeterminada();
            MMovimientosCaja *mc = new MMovimientosCaja( this );
            if( mc->agregarMovimiento( pid_caja, "Pago en efectivo de recibo", QString(), total, 0.0 ) )
            {
                id_caja = mc->ultimoIdInsertado();
            } else {
                qDebug( "MPagos::agregarRecibo::MovimientoCaja:: Error al agregar el movimiento" );
                return -1;
            }
        }
    }
    // Elimino la asociación si existe
    int ret = -1;
    this->clear();
    this->inicializar();
    QSqlRecord rec = this->record();
    // Cliente
    rec.setValue( "id_cliente", id_cliente );
    rec.setValue( "fecha_pago", fecha );
    rec.setValue( "texto", contenido );
    rec.setValue( "precio", total );
     /// @TODO: Ver si esto andaría bien con el formulario de pago retrasado
    if( efectivo && pagado )  {
        rec.setValue( "forma_pago", MPagos::Efectivo );
        rec.setValue( "pagado", true );
        rec.setValue( "id_caja", id_caja );
    } else {
        if( pagado ) {
            rec.setValue( "forma_pago", MPagos::Otro );
            rec.setValue( "pagado", true );
            rec.setNull( "id_caja" );
        } else {
            rec.setValue( "forma_pago", MPagos::SinPagar );
            rec.setValue( "pagado", false );
            rec.setNull( "id_caja" );
        }
    }
    NumeroComprobante proximo = this->proximoSerieNumeroRecibo();
    // Ver por el tema de numero de caja
    if( proximo.serie() == -1 ) { qDebug( "Error de numeracion!" );  return -1; }
    rec.setValue( "serie", proximo.serie() );
    rec.setValue( "numero",proximo.numero() );
    rec.setValue( "cancelado", false );
    if( this->insertRecord( -1, rec ) ) {
        this->submitAll();
        int id_recibo = query().lastInsertId().toInt();
        if( id_recibo > 0 ) {
            ret = id_recibo;
            // Veo si es necesario actualizar la cuenta corriente.
            // Solo será necesario si fue fue pagado efectivamente
            if( efectivo || pagado ) {
                QString cuenta = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
                if( cuenta == QString::number( MCuentaCorriente::ErrorNumeroCuenta ) ) {
                    // no posee cuenta corriente
                    qDebug( "El cliente no posee cuenta corriente, se salteara la actualizaciòn de cuentas corrientes" );
                } else if( cuenta == QString::number( MCuentaCorriente::ErrorClienteInvalido ) ) {
                    // Error de numero de cliente
                    qDebug( "Id de cliente erroneo" );
                } else {
                    // Actualizo la cuenta corriente - El total es negativo para que vaya al haber
                    if( MItemCuentaCorriente::agregarOperacion( cuenta,
                                                                proximo.aCadena(),
                                                                ret,
                                                                MItemCuentaCorriente::Recibo,
                                                                fecha,
                                                                QString( "Pago de recibo %1" ).arg( proximo.aCadena() ),
                                                                total * (-1) ) ) {
                        //qDebug( "Item de cuenta corriente agregado correctamente." );
                    } else {
                        qWarning( "No se pudo agregar el item de la cuenta corriente" );
                    }
                }
            } else {
                if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
                    QString cuenta = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
                    if( cuenta == QString::number( MCuentaCorriente::ErrorNumeroCuenta ) ) {
                        // no posee cuenta corriente
                        qDebug( "El cliente no posee cuenta corriente, se salteara la actualizaciòn de cuentas corrientes" );
                    } else if( cuenta == QString::number( MCuentaCorriente::ErrorClienteInvalido ) ) {
                        // Error de numero de cliente
                        qDebug( "Id de cliente erroneo" );
                    } else {
                        // Actualizo la cuenta corriente - El total es positivo para que vaya al debe
                        if( MItemCuentaCorriente::agregarOperacion( cuenta,
                                                                    proximo.aCadena(),
                                                                    ret,
                                                                    MItemCuentaCorriente::Recibo,
                                                                    fecha,
                                                                    texto_ctacte,
                                                                    total ) ) {
                            //qDebug( "Item de cuenta corriente agregado correctamente." );
                        } else {
                            qWarning( "No se pudo agregar el item de la cuenta corriente" );
                        }
                    }
                } else {
                    qDebug( "No se agrego item de ctacte por no estar pagado el recibo" );
                }
            }

            // Actualizo la entrada de caja
            if( pagado && efectivo ) {
                if( ERegistroPlugins::getInstancia()->existePluginExterno("caja") ) {
                    QString nombre_cliente = MClientes::getRazonSocial( id_cliente );
                    MMovimientosCaja *mc = new MMovimientosCaja();
                    mc->actualizarMovimiento( id_caja, QString( "Pago en efectivo de recibo %1 - %2" ).arg( proximo.aCadena() ).arg( nombre_cliente ) );
                    delete mc;
                }
            }
        } else {
            // ¿no se lleno el campo ? - seguramente la base de datos no lo soporta
            qDebug( "MPagos::agregarRecibo::No se pudo llenar el id del recibo?" );
        }
    } else {
        // Error al insertar el registro
        qDebug() << "MPagos::agregarRecibo:: Error al insertar el registro";
        qDebug() << this->lastError().text();
    }
    this->relacionar();

    return ret;
}

/*!
 * \fn MPagos::numeroSerieActual()
 * Devuelve el numero de serie actual para el recibo
 * \return Numero de serie o -1 si hay error
 */
int MPagos::numeroSerieActual()
{
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT MAX(serie) FROM recibos") ) ) {
    if( cola.next() ) {
        return cola.record().value(0).toInt();
    } else {
        return -1;
    }
 } else {
    return -1;
 }
}

/*!
 * \fn MPagos::numeroReciboActual( const int serie )
 * Devuelve el numero de recibo de ultima emision
 * \param serie Numero de serie
 * \return Numero de recibo actual o -1 si hay error
 */
int MPagos::numeroReciboActual( const int serie )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT MAX(numero) FROM recibos WHERE serie = %1" ).arg( serie ) ) ) {
        if( cola.next() ) {
           return cola.record().value(0).toInt();
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

#include <QMessageBox>
/*!
 * \fn MPagos::proximoSerieNumeroRecibo()
 * Devuelve un par de numeros indicando la serie y numero de recibo que se debería guardar al agregar un recibo.
 * \return NumeroRecibo indicando <serie, numero> o <-1,-1> si hubo un error
 */
NumeroComprobante &MPagos::proximoSerieNumeroRecibo()
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT MAX( serie ) FROM recibos" ) ) ) {
        if( cola.next() ) {
            int serie = cola.record().value(0).toInt();
            if( serie == 0 ) { serie = 1; }
            if( cola.exec( QString( "SELECT MAX( numero ) FROM recibos WHERE serie = %1" ).arg( serie ) ) ) {
                if( cola.next() ) {
                    int numero = cola.record().value(0).toInt();
                    if( numero == 0 ) { numero = 1; }
                    NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
                    num->siguienteNumero();
                    return *num;
                } else {
                    qDebug() << "Error de cola al hacer next al obtener el numero de recibo maximo";
                    qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
                }
            } else {
                qDebug() << "Error de cola al hacer exec al obtener el numero de recibo maximo";
                qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
            }
        } else {
            qDebug() << "Error de cola al hacer next al obtener el numero de serie de recibo maximo";
            qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
        }
    } else {
        // por ley se empieza desde el numero de serie 1
        NumeroComprobante *num = new NumeroComprobante( 0, 1, 0 );
        num->siguienteNumero();
        qDebug() << "Error de cola al hacer exec al obtener el numero de serie de recibo maximo - Se inicio una nueva numeracion";
        qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
        return *num;
    }
    NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
    qDebug( "Devolviendo numero de Comprobante invalido" );
    return *invalido;
}

#include "../CtaCte/mitemcuentacorriente.h"
#include "../CtaCte/mcuentacorriente.h"
#include "../caja/mcajas.h"
#include "../caja/mmovimientoscaja.h"
/*!
 * \fn MPagos::setearComoPagado( const int id_recibo, const bool efectivo )
 * Setea como pago el recibo que se pasa como id. Si ya esta como pagado, no hace nada y devuelve true.
 * \param id_recibo ID del recibo
 * \param efectivo Define si la forma de pago es MPagos::Efectivo ( si el parametro es true ) o MPagos::Otro ( si es false )
 * \param ctacte Define si se debe actualizar o no la cuenta corriente.
 * \return Verdadero si pudo ser puesto como pagado o si ya estaba como pagado.
 */
bool MPagos::setearComoPagado( const int id_recibo, const bool efectivo, const bool ctacte )
{
 QSqlQuery cola;
 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
 if( cola.exec( QString( "UPDATE recibos SET pagado = 1 WHERE id_recibo = %1" ).arg( id_recibo ) ) ) {
    if( cola.exec( QString( "SELECT id_cliente, precio, serie, numero FROM recibos WHERE id_recibo = %1" ).arg( id_recibo ) ) )
    {
        cola.next();
        int id_cliente = cola.value(0).toInt();
        double precio = cola.value(1).toDouble();
        NumeroComprobante *num = new NumeroComprobante( this, cola.value(2).toInt(), cola.value(3).toInt() );
        // Coloco el recibo en la cuenta corriente del cliente si no es el consumidor final
        if( id_cliente > 0 && ctacte ) {
            MItemCuentaCorriente *m = new MItemCuentaCorriente();
            if( m->agregarOperacion( MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente ), // Numero de cuenta del cliente
                                     num->aCadena(), // Numero de recibo ( real )
                                     id_recibo, // Id de referencia
                                     MItemCuentaCorriente::Recibo,
                                     QDate::currentDate(),
                                     QString( "Pago mediante recibo %1" ).arg( num->aCadena() ),
                                     precio ) != -1 )
            {
                qDebug( "Operación de cuenta corriente guardada correctamente" );
            } else {
                qDebug( "Error al intentar agregar la operación de cuenta corriente cuando poniendo como pagado un recibo." );
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                return false;
            }
            delete m;
        } // El cliente es consumidor final sino
        // Si el recibo se pago en efectivo, hago su ingreso en caja
        if( efectivo )
        {
            QString nombre_cliente = MClientes::getRazonSocial( id_cliente );
            MMovimientosCaja *m = new MMovimientosCaja();
            if( m->agregarMovimiento( MCajas::cajaPredeterminada(),
                                      QString( "Pago del recibo %1 - %2" ).arg( num->aCadena() ).arg( nombre_cliente ),
                                      QString(),
                                      precio ) )
            {
                qDebug( "Operación de movimiento de caja agregado correctamente" );
            } else {
                qDebug( "Error al intentar registrar el movimiento de caja al pagar un recibo ya emitido." );
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                return false;
            }
            delete m;
            // El metodo de pago es efectivo
            if( cola.exec( QString( "UPDATE recibos SET forma_pago = %1 WHERE id_recibo = %2" ).arg( MPagos::Efectivo ).arg( id_recibo ) ) ) {
                // Todos los pasos guardados correctamente
            } else {
                qDebug() << "Error al intentar poner la forma de pago en efectivo al poner como pagado un recibo ya emitido";
                qDebug() << cola.lastError().text();
                qDebug() << cola.lastQuery();
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                return false;
            }
        } else {
            // El metodo de pago es otro ( desconocido )
            if( cola.exec( QString( "UPDATE recibos SET forma_pago = %1 WHERE id_recibo = %2" ).arg( MPagos::Otro ).arg( id_recibo ) ) ) {
                // Todos los pasos guardados correctamente
            } else {
                qDebug() << "Error al intentar poner la forma de pago en otro al poner como pagado un recibo ya emitido";
                qDebug() << cola.lastError().text();
                qDebug() << cola.lastQuery();
                QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
                return false;
            }
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de datos del recibo.";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
        return false;
    }
 } else {
    qDebug() << "Error al intentar colocar el recibo como pagado";
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
    return false;
 }
 if( !QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit() ) {
     qCritical() << "Error al hacer commit de la base de datos!";
     abort();
 } else {
     return true;
 }
}

/*!
 * \fn MPagos::buscarMenorSerieNumeroPagado()
 * Devuelve el par serie-numero del menor recibo pagado de la base de datos
 * \return NumeroComprobante indicando <serie,numero> o <0,0> si se produjo un error
 */
NumeroComprobante &MPagos::buscarMenorSerieNumeroPagado()
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT MAX( serie ) FROM recibos WHERE pagado = 0" ) ) ) {
        if( cola.next() ) {
            int serie = cola.record().value(0).toInt();
            if( cola.exec( QString( "SELECT MAX( numero ) FROM recibos WHERE serie = %1 AND pagado = 0" ).arg( serie ) ) ) {
                if( cola.next() ) {
                    int numero = cola.record().value(0).toInt();
                    NumeroComprobante *num = new NumeroComprobante( 0, serie, numero );
                    num->siguienteNumero();
                    return *num;
                } else {
                    qDebug() << "Error de cola al hacer next al obtener el numero de recibo maximo pagado";
                    qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
                }
            } else {
                qDebug() << "Error de cola al hacer exec al obtener el numero de recibo maximo pagado";
                qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
            }
        } else {
            qDebug() << "Error de cola al hacer next al obtener el numero de serie de recibo maximo pagado";
            qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error de cola al hacer exec al obtener el numero de serie de recibo maximo pagado";
        qDebug() << "Error: " << cola.lastError().number() << cola.lastError().text() << cola.lastQuery();
    }
    NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
    return *invalido;
}

/*!
 * \fn MPagos::buscarSiPagado( const int serie, const int numero )
 * Busca si un recibo con los datos del parametro esta pagado o no
 * \param serie Serie del recibo buscado
 * \param numero Numero del recibo buscado
 * \returns pagado o no
 */
bool MPagos::buscarSiPagado( const int serie, const int numero )
{
    if( serie < 0 && numero <= 0 ) {
        qDebug() << "Numeros de comprobante invalido";
        qDebug() << QString( "%1-%2" ).arg( serie ).arg( numero );
        return true;
    }
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT pagado FROM recibos WHERE serie = %1 AND numero = %2" ).arg( serie ).arg( numero ) ) ) {
     if( cola.next() ) {
         if( cola.record().value(0).toBool() ) {
             return true;
         } else {
             return false;
         }
     } else {
         qDebug() << "Error al hacer next en cola de si pagado serie-numero";
         qDebug() << cola.lastQuery();
         qDebug() << cola.lastError().text();
         return false;
     }
 } else {
     qDebug() << "error al hacer exec de cola de si pagado serie-numero";
     qDebug() << cola.lastQuery();
     qDebug() << cola.lastError().text();
     return false;
 }
}

/*!
 * \fn MPagos::buscarIdPorSerieNumero( const int serie, const int numero )
 * Devuelve el identificador de base de datos para una serie y numero de recibo
 * \param serie Serie del recibo buscado.
 * \param numero Numero del recibo buscado
 * \returns ID en la base de datos
 */
int MPagos::buscarIdPorSerieNumero( const int serie, const int numero )
{
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT id_recibo FROM recibos WHERE serie = %1 AND numero = %2" ).arg( serie ).arg( numero ) ) )
 {
     if( cola.next() )
     {
         return cola.record().value(0).toInt();
     } else {
         qDebug( "Error al hacer next en la cola de buscar id recibo x num y serie" );
         return -1;
     }
 } else {
     qDebug( "Error al hacer exec en la cola de buscar id recibo x num y serie" );
     return -2;
 }
}

/*!
 * \fn MPagos::buscarIdPorSerieNumero( const NumeroComprobante num )
 * Sobrecarga para tomar el parametro NumeroRecibo
 * \param num Numero y serie del recibo buscado
 * \returns ID en la base de datos
 */
int MPagos::buscarIdPorSerieNumero( const NumeroComprobante num )
{ return buscarIdPorSerieNumero( num.serie(), num.numero() ); }


/*!
 * \fn MPagos::buscarImporte( NumeroComprobante num )
 * Devuelve el importe de un recibo ya emitido segun el numero de comprobante
 * \param num Numero de Comprobante buscado
 * \returns Importe buscado o -1.1
 */
double MPagos::buscarImporte( NumeroComprobante num )
{
    if( !num.esValido() ) { return -1.1; }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT precio FROM recibos WHERE serie = %1 AND numero = %2" ).arg( num.serie() ).arg( num.numero() ) ) ) {
    if( cola.next() ) {
       return cola.record().value(0).toDouble();
    } else {
       qDebug() << "Error al hacer next en cola de importe de recibo";
       return -1.1;
    }
  } else {
    qDebug() << "error al hacer exec de cola de importe de recibo";
    return -1.1;
    }
}

/*!
 * \brief MPagos::buscarFechaEmisionRecibo
 * Busca la fecha de emision del recibo pasado como parametro
 * \param id_recibo Identificador del recibo
 * \return Fecha de emision o fecha invalida si no se encontró ningun dato
 */
QDate MPagos::buscarFechaEmisionRecibo( const int id_recibo )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT fecha_pago FROM recibos WHERE id_recibo = %1" ).arg( id_recibo ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        } else {
            qDebug() << "MPagos::buscarFechaEmisionRecibo::Error al hacer next en la busqueda de los datos";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "MPagos::buscarFechaEmisionRecibo::Error al hacer el exce en la busaqueda de los datos";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}

/*!
 * \brief MPagos::buscarFechaEmisionRecibo
 * Busca la fecha de emision del recibo pasado como parametro
 * \param id_recibo Identificador del recibo
 * \return Fecha de emision o fecha invalida si no se encontró ningun dato
 */
QDate MPagos::buscarFechaEmisionRecibo( NumeroComprobante num_recibo )
{
    return buscarFechaEmisionRecibo( buscarIdPorSerieNumero( num_recibo.serie(), num_recibo.numero() ) );
}

/*!
 * \brief MPagos::buscarSiAPagarLuego
 * Busca si el metodo de pago es luego
 * \param id_recibo Identificador del recibo
 * \return Verdadero si el metodo de pago es A Pagar Luego
 */
bool MPagos::buscarSiAPagarLuego( const int id_recibo )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT forma_pago FROM recibos WHERE id_recibo = %1" ).arg( id_recibo ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() == MPagos::SinPagar ) {
                return true;
            }
        } else {
            qDebug() << "MPagos::buscarNumeroComprobantePorId::Error al hacer next en la busqueda de los datos";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "MPagos::buscarNumeroComprobantePorId::Error al hacer el exce en la busaqueda de los datos";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MPagos::buscarNumeroComprobantePorId( const int id_recibo )
 * Devuelve un numero de comprobante para el ID de recibo pasado. Si el ID no existe o hubo error, devuelve un numero de comprobante erroneo.
 * \param id_recibo ID del recibo buscado
 * \returns NumeroComprobante buscado o invalido si hubo error o no existe el ID.
 */
NumeroComprobante &MPagos::buscarNumeroComprobantePorId( const int id_recibo )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT serie, numero FROM recibos WHERE id_recibo = %1" ).arg( id_recibo ) ) ) {
        if( cola.next() ) {
            NumeroComprobante *num = new NumeroComprobante( 0, cola.record().value(0).toInt(), cola.record().value(1).toInt() );
            return *num;
        } else {
            qDebug() << "MPagos::buscarNumeroComprobantePorId::Error al hacer next en la busqueda de los datos";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "MPagos::buscarNumeroComprobantePorId::Error al hacer el exce en la busaqueda de los datos";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    NumeroComprobante *invalido = new NumeroComprobante( 0, -1, -1 );
    return *invalido;
}

/*!
 * \brief MPagos::verificarFacturaPagada
 * Busca en las descripciones de los recibos para encontrar el identificador de la factua y la palabra factura
 * \param numero Numero de compobante de la factura.
 * \returns
 */
bool MPagos::verificarFacturaPagada( const QString numero )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(texto) FROM recibos WHERE texto LIKE '%%1%'" ).arg( numero ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            }
        }
    } else {
        qWarning( "Error al ejecutar la cola de averiguacion de factura pagada" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \brief MPagos::setearId
 * Carga los datos en la variable _registro con los elementos del recibo pasado como paremtro
 * \param id Identificador del recibo
 */
void MPagos::setearId( const int id )
{
    if( id > 0 ) {
        _id_actual = id;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT * FROM recibos WHERE id_recibo = %1").arg( id ) ) ) {
        if( cola.next() ) {
            _registro = cola.record();
        } else {
            qDebug( "Error al cargar los datos del recibo - id no existente!" );
            _id_actual = -1;
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtenicion de datos del registro de recibo";
        qDebug() << cola.lastQuery();
        qDebug() << cola.lastError().text();
        _id_actual = -1;
    }

}

/*!
 * \brief MPagos::getFecha
 * \return
 */
QDate MPagos::getFecha()
{
    if( _id_actual <= 0 )
        return QDate();

    return _registro.value( "fecha_pago" ).toDate();
}

/*!
 * \brief MPagos::getIdCliente
 * \return
 */
int MPagos::getIdCliente()
{
    if( _id_actual <= 0 )
        return -1;

    return _registro.value( "id_cliente" ).toInt();
}

/*!
 * \brief MPagos::getTexto
 * \return
 */
QString MPagos::getTexto()
{
    if( _id_actual <= 0 )
        return QString();

    return _registro.value( "texto" ).toString();
}

/*!
 * \brief MPagos::getTotal
 * \return
 */
double MPagos::getTotal()
{
    if( _id_actual <= 0 )
        return -0.0;

    return _registro.value( "precio" ).toDouble();
}

/*!
 * \brief MPagos::getFormaPago
 * \return
 */
MPagos::FormaPago MPagos::getFormaPago()
{
    if( _id_actual <= 0 )
        return MPagos::Otro;

    return (MPagos::FormaPago) _registro.value( "forma_pago" ).toInt();
}

/*!
 * \fn MPagos::existe( NumeroComprobante num )
 * Devuelve verdadero si el numero de comprobante existe
 * \param num Numero de comprobante buscando
 */
bool MPagos::existe( NumeroComprobante num ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT( id_recibo ) FROM recibos WHERE serie = %1 AND numero = %2" ).arg( num.serie() ).arg( num.numero() ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            }
        } else {
            qDebug() << "MPagos::existe: Error al hacer next en buscar si existe un recibo";
            qDebug() << cola.lastError().text();
        }
    } else {
        qDebug() << "Mpagos::existe: Error al hacer exec en buscar si existe un recibo";
        qDebug() << cola.lastQuery();
        qDebug() << cola.lastError().text();
    }
    return false;
}

/*!
 * \brief MPagos::anulado
 * Verifica si un recibo ha sido anulado o no.
 * \param num Numero de comprobante a verificar
 * \return Verdadero o falso incluso en caso de error.
 */
bool MPagos::anulado(NumeroComprobante num)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT cancelado FROM recibos WHERE serie = %1 AND numero = %2" ).arg( num.serie() ).arg( num.numero() ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toBool() == true ) {
                return true;
            } else {
                return false;
            }
        } else {
            qDebug() << "MPagos::existe: Error al hacer next en buscar si un recibo esta anulado";
            qDebug() << cola.lastError().text();
        }
    } else {
        qDebug() << "Mpagos::existe: Error al hacer exec en buscar si un recibo esta anulado";
        qDebug() << cola.lastQuery();
        qDebug() << cola.lastError().text();
    }
    return false;
}

/*!
 * \fn MPagos::buscarIdCliente( NumeroComprobante num )
 * Devuelve el ID del cliente del recibo especificado.
 */
int MPagos::buscarIdCliente( NumeroComprobante num )
{
    if( !num.esValido() ) { return -1; }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id_cliente FROM recibos WHERE serie = %1 AND numero = %2" ).arg( num.serie() ).arg( num.numero() ) ) ) {
        if( cola.next() ) {
             return cola.record().value(0).toInt();
        } else {
            qDebug() << "MPagos::existe: Error al hacer next en buscar id de cliente de un recibo";
        }
    } else {
        qDebug() << "Mpagos::existe: Error al hacer exec en buscar id de cliente de un recibo";
        qDebug() << cola.lastQuery();
        qDebug() << cola.lastError().text();
    }
    return -1;
}

/*!
 * \fn MPagos::cancelarRecibo( const int id_recibo, QString razon, QDateTime fechahora )
 * Cancela el recibo pasado como parametro, registra la cancelacion en la fecha hora pasada como parametro y con la razon correspondiente.
 * Ademas coloca todos los datos relacionados con este recibo en el estado que sea necesario para comprobar su cancelacion.
 * \param id_recibo ID del recibo
 * \param razon Razon de cancelacion del recibo
 * \param fechahora Fecha y hora de la cancelacion
 * \return Verdadero si todas las operaciones pudieron ser realizadas correctametne
 */
bool MPagos::cancelarRecibo( const int id_recibo, QString razon, QDateTime fechahora )
{
    // Inicio una transaccion
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
    // Me fijo si esta en algun item de cuenta corriente.
    int id_ctacte = MItemCuentaCorriente::buscarIDPorComprobante( MItemCuentaCorriente::Recibo, id_recibo );
    if( id_ctacte != -1 ) {
        if( !MItemCuentaCorriente::cancelarOperacion( id_ctacte ) ) {
            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
            return false;
        }
    }
    QSqlQuery cola;
    if( cola.prepare( "UPDATE recibos SET cancelado = :cancelado, razon_cancelacion = :razon, fechahora_cancelacion = :fechahora WHERE id_recibo = :id_recibo" ) ) {
        cola.bindValue( ":cancelado", true );
        cola.bindValue( ":id_recibo", id_recibo );
        cola.bindValue( ":fechahora", fechahora );
        cola.bindValue( ":razon", razon );
        if( cola.exec() ) {
            QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit();
            return true;
        } else {
            qDebug() << "Error al intentar ejecutar la cola de cancelacion de recibo";
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    }
    QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
    return false;
}
