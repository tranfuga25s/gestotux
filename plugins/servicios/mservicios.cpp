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
#include "mservicios.h"

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>
#include <QDebug>

#include "math.h"

/*
CREATE TABLE IF NOT EXISTS `servicios` (
  `id_servicio` bigint(1) NOT NULL AUTO_INCREMENT,
  `nombre` tinytext NOT NULL,
  `descripcion` text,
  `fecha_alta` date NOT NULL,
  `fecha_baja` date DEFAULT NULL,
  `precio_base` double(10,3) NOT NULL,
  `periodo` int(10) NOT NULL,
  `dia_cobro` int(10) NOT NULL,
  `forma_incompleto` int(10) NOT NULL,
  PRIMARY KEY (`id_servicio`)
) ENGINE=MyISAM ;
*/

MServicios::MServicios(QObject *parent)
 : QSqlTableModel(parent)
{
 setTable( "servicios" );
 setHeaderData( 0, Qt::Horizontal, tr( "#ID" ) );
 setHeaderData( 1, Qt::Horizontal, tr( "Nombre" ) );
 setHeaderData( 2, Qt::Horizontal, tr( "Descripcion" ) );
 setHeaderData( 3, Qt::Horizontal, tr( "Fecha de Alta" ) );
 setHeaderData( 4, Qt::Horizontal, tr( "Fecha de Baja" ) );
 setHeaderData( 5, Qt::Horizontal, tr( "Precio Base" ) );
 setHeaderData( 6, Qt::Horizontal, tr( "Periodo" ) );
 setHeaderData( 7, Qt::Horizontal, tr( "Dia de Cobro" ) );
 setHeaderData( 8, Qt::Horizontal, tr( "Forma de cobro incompleto" ) );
}

/*!
 * @fn MServicios::data( const QModelIndex& item, int role ) const
 * Implementacion del metodo de datos para que las enumeraciones se vena como texto
 */
QVariant MServicios::data( const QModelIndex& item, int role ) const {
    if( !item.isValid() ) { return QVariant(); }
    switch( role ) {
        case Qt::DisplayRole:
        {
            switch( item.column() ) {
                case 6: // Periodo
                {
                        switch( QSqlTableModel::data( item, role ).toInt() ) {
                            case MServicios::Semanal:
                            { return "Semanal"; }
                            case MServicios::Quincenal:
                            { return "Quincenal"; }
                            case MServicios::Mensual:
                            { return "Mensual"; }
                            case MServicios::BiMensual:
                            { return "Bi-Mensual"; }
                            case MServicios::Trimestral:
                            { return "Trimestral"; }
                            case MServicios::Cuatrimestral:
                            { return "Cuatrimestral"; }
                            case MServicios::Seximestral:
                            { return "Semestral"; }
                            case MServicios::Anual:
                            { return "Anual"; }
                        }
                        break;
                }
                case 8: // Metodo Incompleto
                {
                        switch( QSqlTableModel::data( item, role ).toInt() ) {
                            case MServicios::DiasFaltantes:
                            { return "Dias Faltantes"; break; }
                            case MServicios::MesCompleto:
                            { return "Mes Completo"; break; }
                            default:
                            { return "Metodo Desconocido"; break;}
                        }
                        break;
                }
                case 5:
                { return QString( "$ %L1" ).arg( QSqlTableModel::data( item, role ).toDouble(), 10,'f', 2 ); }
                default:
                { return QSqlTableModel::data( item, role ); }
            }
        }
        case Qt::TextAlignmentRole:
        {
            switch( item.column() ) {
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                {
                    return int( Qt::AlignCenter | Qt::AlignHCenter );
                    break;
                }
                default:
                {
                    return QSqlTableModel::data( item, role );
                    break;
                }
            }
        }
        case Qt::UserRole:
        {
            return QSqlTableModel::data( index( item.row(), 0 ), Qt::EditRole );
            break;
        }
        default:
        { return QSqlTableModel::data( item, role ); }
    }
}

Qt::ItemFlags MServicios::flags( const QModelIndex &/*index*/ ) const
{
   return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/*!
 * \fn MServicios::asociarCliente( int id_cliente, int id_servicio, QDateTime fecha )
 * Asocia el cliente con el servicio correspondiente en la base de datos en la fecha de alta indicada.
 * @param id_cliente Identificador de cliente
 * @param id_servicio Identificador del servicio
 * @param fecha Fecha en que el cliente se da de alta en el servicio
 * @return Verdadero si la asociacion se pudo realizar correctamente
 */
bool MServicios::asociarCliente( int id_cliente, int id_servicio, QDateTime fecha )
{
 QSqlTableModel *modelo = new QSqlTableModel();
 modelo->setTable( "servicios_clientes" );
 modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
 QSqlRecord registro = modelo->record();
 registro.setValue( "id_cliente", id_cliente );
 registro.setValue( "id_servicio", id_servicio );
 registro.setValue( "fecha_alta", fecha );
 registro.setNull( "fecha_baja" );
 if( modelo->insertRecord( -1, registro ) )
 {
  if( !modelo->submitAll() )
  {
   qWarning() << "Error al hacer submit en el modelo: " << modelo->lastError().text();
   delete modelo;
   return false;
  }
 }
 else
 {
  qWarning() << "Error al hacer insertar en el modelo: " << modelo->lastError().text();
  delete modelo;
  return false;
 }
 delete modelo;
 qDebug( "Datos guardados correctamente" );
 return true;
}

/*!
    \fn MServicios::agregarServicio( QString nombre, QString detalle, QDate fecha_alta, double precio_base, int periodo, int dia_cobro, int forma_incompleto )
    Agrega un nuevo servicio a la base de datos con los datos pasados como parametro
    \param nombre Nombre del servicio
    \param detalle Explicacion mas detallada del servicio
    \param fecha_alta Fecha desde cuando empieza a funcionar el servicio
    \param precio_base Precio base sobre el cual se calculan los recargos
    \param periodo Periodo de cobro del servicio #MServicios::Periodo
    \param dia_cobro Dia del periodo en que se inicia el cobro del servicio
    \param forma_incompleto Forma de cobro cuando se da de alta un cliente fuera del inicio del periodo #MServicios::FormaIncompleto
    \param texto_facturacion Texto que se mostrarà en la facturacion
 */
bool MServicios::agregarServicio( QString nombre, QString detalle, QDate fecha_alta, double precio_base, int periodo, int dia_cobro, int forma_incompleto, QString texto_facturacion )
{
  // Teoricamente los datos se controlaron de ser correctos
  QSqlRecord registro = this->record();
  registro.setValue( "nombre", nombre );
  registro.setValue( "detalle", detalle );
  registro.setValue( "fecha_alta", fecha_alta );
  registro.setValue( "precio_base", precio_base );
  registro.setValue( "periodo", periodo ); // Agregar verificacion de enumeracion
  registro.setValue( "dia_cobro", dia_cobro );// Agregar verificacion de enumeracion
  registro.setValue( "forma_incompleto", forma_incompleto );// Agregar verificacion de enumeracion
  registro.setValue( "text_facturacion", texto_facturacion);
  if( this->insertRecord( -1, registro ) ) {
      return true;
  } else {
      qDebug() << "Error MServicios::agregarServicio(): " << this->lastError().text();
      return false;
  }
}

/*!
 * \fn MServicios::precioBase( int id_servicio )
 * Devuelve el precio base de un servicio
 * @param id_servicio Identificador del servicio al cual se le quiere saber el percio base.
 * @return precio base para el servicio solicitado o 0.0 si existiÃ³ un error.
 */
double MServicios::precioBase( int id_servicio )
{
  if( id_servicio == 0 ) {
      return -100.0;
  }
  QSqlQuery cola( QString( "SELECT precio_base FROM %2 WHERE id_servicio = %1" ).arg( id_servicio ).arg( "servicios" ) );
  if( cola.exec() ) {
    if( cola.next() )
    {
     return cola.record().value(0).toDouble();
    } else {
        qDebug( "Error al hacer next en la cola de precio base de servicio" );
        qDebug() << cola.lastQuery();
        return 0.0;
    }
  } else {
      qDebug( "Error al hacer exec en la cola de precio base de servicio" );
      qDebug() << cola.lastQuery();
      return 0.0;
  }
}

/*!
 * \fn MServicios::getNombreServicio( int id_servicio )
 * Devuelve el nombre de un servicio
 * @param id_servicio Identificador del servicio al cual se le quiere saber el nombre.
 * @return Nombre del servicio o una cadena vacÃ­a si existio un error al obtener el nombre
 */
QString MServicios::getNombreServicio( int id_servicio )
{
  QSqlQuery cola( QString( "SELECT nombre FROM %2 WHERE id_servicio = %1" ).arg( id_servicio ).arg( "servicios") );
  if( cola.exec()) {
      if( cola.next() ) {
          return cola.record().value(0).toString();
      } else { qDebug( "Servicios:MServicios:Error al hacer el next en la cola de nombre del servicio" ); return ""; }
  } else { qDebug( "Servicios:MServicios:Error al hacer el exec en la cola del nombre del servicio" ); return ""; }
}

/*!
 * \fn MServicios::getFechaAlta( const int id_servicio )
 * Devuelve la fecha de alta de un servicio
 * \param id_servicio Identificador de servicio
 * \return Fecha de alta si se pudo encontrar o un QDate() si existio un error.
 */
QDate MServicios::getFechaAlta( const int id_servicio )
{
    QSqlQuery cola( QString( "SELECT fecha_alta FROM servicios WHERE id_servicio = %1" ).arg( id_servicio ) );
    if( cola.exec() ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        } else {
            qDebug( "Servicios:MServicios:Error al buscar la fecha de alta del periodo -> next" );
            return QDate();
        }
    } else {
        qDebug( "Servicios:MServicios:Error al buscar la fecha de alta del periodo -> exec" );
        return QDate();
    }
}

/*!
 * \fn MServicios::getDiaFacturacion( const int id_servicio )
 * Devuelve el día en que se debe emitir los recibos de un servicio.
 * \param id_servicio Identificador de servicio
 * \return dia del período o -1 en caso de error
 */
int MServicios::getDiaFacturacion(const int id_servicio)
{
    QSqlQuery cola( QString( "SELECT dia_cobro FROM servicios WHERE id_servicio = %1" ).arg( id_servicio ) );
    if( cola.exec() ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qDebug( "Servicios:MServicios: Error al buscar el dia de cobro de un servicio -> next" );
            return -1;
        }
    } else {
        qDebug( "Servicios:MServicios:Error al busca el dia de cobro de un servicio-> exec" );
        return -1;
    }
}

/*!
 * \fn MServicios::obtenerPeriodo( const int id_servicio )
 * Devuelve el tipo de periodo para el servicio solicitado
 * \param id_servicio Servicio al caul se le desea obtener el periodo.
 * \return MServicios::Periodo o MServicios::Invalido en caso de error.
 */
MServicios::Periodo MServicios::obtenerPeriodo( const int id_servicio ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT periodo FROM servicios WHERE id_servicio = %1" ).arg( id_servicio )  ) ) {
        if( cola.next() ) {
            switch( cola.record().value(0).toInt() ) {
                case MServicios::Semanal:       { return MServicios::Semanal;       break; }
                case MServicios::Quincenal:     { return MServicios::Quincenal;     break; }
                case MServicios::Mensual:       { return MServicios::Mensual;       break; }
                case MServicios::BiMensual:     { return MServicios::BiMensual;     break; }
                case MServicios::Trimestral:    { return MServicios::Trimestral;    break; }
                case MServicios::Cuatrimestral: { return MServicios::Cuatrimestral; break; }
                case MServicios::Seximestral:   { return MServicios::Seximestral;   break; }
                case MServicios::Anual:         { return MServicios::Anual;         break; }
                default: { return MServicios::Invalido; break; }
            }
        } else {
            qDebug( "Error de next en cola de obtencion de periodo en serivicio" );
        }
    } else {
        qDebug( "Error de exec en la cola de obtencion de periodo de un servicio" );
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return MServicios::Invalido;
}

/*!
 * \fn MServicios::verificarSiPuedeEliminar( const int id_servicio )
 * Verifica si existe algun dato relacionado con el servicio que se intenta eliminar para definir si es posible eliminarlo o no.
 * \param id_servicio Identificador del servicio buscado
 * \return verdadero si no existe ningun dato asociado al servicio
 */
bool MServicios::verificarSiPuedeEliminar( const int id_servicio )
{
    QSqlQuery cola;
    bool retorno = true;
    // Asociacion cliente-servicio
    if( cola.exec( QString( "SELECT COUNT(id_servicio) FROM servicios_clientes WHERE id_servicio = %1"  ).arg( id_servicio ) ) ) {
        if( cola.next() )  {
            if( cola.record().value(0).toInt() > 0 ) { return false; }
        } else {
            qDebug( "MServicios::verificarSiPuedeEliminar::Error next 1 verificacion -  servicios_clientes" );
            return false;
        }
    } else {
        qDebug( "MServicios::verificarSiPuedeEliminar::Error exec 1 verificacion -  servicios_clientes" );
        return false;
    }
    // Asociacion recargo-servicio
    if( cola.exec( QString( "SELECT COUNT(id_servicio) FROM recargos WHERE id_servicio = %1"  ).arg( id_servicio ) ) ) {
        if( cola.next() )  {
            if( cola.record().value(0).toInt() > 0 ) { return false; }
        } else {
            qDebug( "MServicios::verificarSiPuedeEliminar::Error next 2 verificacion -  recargos" );
            return false;
        }
    } else {
        qDebug( "MServicios::verificarSiPuedeEliminar::Error exec 2 verificacion -  recargos" );
        return false;
    }
    // AsociaciÃ³n servicio-periodo
    if( cola.exec( QString( "SELECT COUNT(id_servicio) FROM periodo_servicio WHERE id_servicio = %1"  ).arg( id_servicio ) ) ) {
        if( cola.next() )  {
            if( cola.record().value(0).toInt() > 0 ) { return false; }
        } else {
            qDebug( "MServicios::verificarSiPuedeEliminar::Error next 3 verificacion - periodo_servicio" );
            return false;
        }
    } else {
        qDebug( "MServicios::verificarSiPuedeEliminar::Error exec 3 verificacion - periodo_servicio" );
        return false;
    }
    return retorno;
}

/*!
 * \fn MServicios::dadoDeBaja( const int id_servicio )
 * Devuelve verdadero si el servicio fue dado de baja
 * \param id_servicio Identificador del servicio
 */
bool MServicios::dadoDeBaja( const int id_servicio)
{
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT COUNT(id_servicio) FROM servicios WHERE id_servicio = %1 AND fecha_baja IS NULL").arg( id_servicio ) ) ) {
        qDebug( "Error al ejecutar la cola de obtenciÃ³n de dato de si el servicio esta dado de baja" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    } else {
        if( cola.next() )  {
            if ( cola.record().value(0).toInt() <= 0 ) {
                return true;
            }
        } else {
            qDebug( "Error al hacer next al ejecutar la cola de obtencion de si el servicio esta dado de baja" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    }
    return false;
}

/*!
 * \fn MServicios::darDeBaja( const int id_servicio, const QDate fecha )
 * Da de baja el servicio especificado
 * \param id_servicio Identificador del servicio
 * \param fecha Fecha en que se da de baja
 * \return Verdadero si pudo se dado de baja
 */
bool MServicios::darDeBaja( const int id_servicio, QModelIndex indice, const QDate fecha )
{
    QSqlQuery cola;
    cola.prepare( "UPDATE servicios SET fecha_baja = :fecha WHERE id_servicio = :id_servicio" );
    cola.bindValue( ":id_servicio", id_servicio );
    cola.bindValue( ":fecha", fecha );
    if( !cola.exec() ) {
        qDebug( "Error al ejecutar la cola de dar de baja el servicio especificado" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    qDebug( "Servicio dado de baja correctamente" );
    emit dataChanged( indice, indice );
    return true;
}

/**
 * @brief MServicios::getTextoFacturacion
 * Devuelve el texto de facturaciòn definido para un servicio
 * @param id_servicio Identificador del servicio
 * @return
 */
QString MServicios::getTextoFacturacion(const int id_servicio)
{
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT texto_facturacion FROM servicios WHERE id_servicio = %1").arg( id_servicio ) ) ) {
        qDebug( "Error al ejecutar la cola de obtenciÃ³n de dato de texto de facturacion" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    } else {
        if( cola.next() )  {
            return cola.record().value(0).toString();
        } else {
            qDebug( "Error al hacer next al ejecutar la cola de obtencion de si el servicio tiene texto de facturacion" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    }
    return QString();
}

QList<int> MServicios::listaDeServicios()
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

/**
 * @brief MServicios::existe
 * @param nombre
 * @return boolean
 */
bool MServicios::existe(const QString nombre)
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT( id_servicio ) FROM servicios WHERE nombre = '%1'" ).arg( nombre ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        } else {
            return false;
        }
    } else {
        qDebug( "Error al ejecutar la cola de contar la cantidad de servicios con un nombre" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
}

/*
"id_servicio" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL
"nombre" TEXT NOT NULL
"descripcion" TEXT
"fecha_alta" DATETIME NOT NULL
"fecha_baja" DATETIME
"precio_base" DOUBLE NOT NULL
"periodo" INTEGER NOT NULL
"dia_cobro" INTEGER NOT NULL
"forma_incompleto" INTEGER NOT NULL
*/
#include "mperiodoservicio.h"
#include "EReporte.h"
#include "../pagos/mpagos.h"
#include "../ventas/MFactura.h"
#include "../ventas/mitemfactura.h"
#include "eregistroplugins.h"
/*!
 * \fn MServicios::calcularCobroAlta( const int id_cliente, const int id_servicio, QDateTime fechaAlta )
 * Calcula cuanto se debe cobrar por el alta a partir de l afecha de alta y la del proximi periodo de cobro.
 * \param id_cliente Cliente al cual se le va a cobrar
 * \param id_servicio Identificador del servicio.
 * \param fechaAlta Fecha en que se dio de alta el cliente.
 * \return Verdadero si se pudo realizar el proceso correctamente.
 */
bool MServicios::calcularCobroAlta( const int id_cliente, const int id_servicio, QDateTime fechaAlta )
{
 if( id_cliente <= 0 ) { return false; }
 // Busco la forma de cobro del servicio
 QSqlQuery cola;
 int forma = FInvalido;
 if( cola.exec( QString( " SELECT forma_incompleto FROM servicios WHERE id_servicio = %1" ).arg( id_servicio ) ) ) {
     if( cola.next() ) {
        forma = cola.record().value(0).toInt();
     } else {
         qDebug( "Error al hacer next en busqueda de forma incompleto" );
     }
 } else {
     qDebug( "Error al hacer exec en la busqueda de forma incompleto" );
 }
 QString texto;
 double precio_final = 0.0;
 MPeriodoServicio *mps = new MPeriodoServicio();
 int periodoActual = mps->getPeriodoActual( id_servicio );
 switch( forma )
 {
    case DiasFaltantes:
    {
        // Busco el inicio del proximo periodo
        QDate proximo = mps->generarFechaInicioPeriodo( id_servicio, periodoActual + 1, QDate::currentDate().year() );
        // Calculo la cantidad de días hasta esa fecha
        int cant_dias = fechaAlta.date().daysTo( proximo );
        // Calculo el precio x día y precio total
        double precio_por_dia = precioBase( id_servicio ) / mps->diasEnPeriodo( obtenerPeriodo( id_servicio ) );
        precio_final = precio_por_dia * cant_dias;
        texto.append( QString::fromUtf8( "Cobro de días faltantes desde %1 hasta %2 por el servicio %3 para el período %4" )
                .arg( fechaAlta.date().toString( Qt::SystemLocaleShortDate ) )
                .arg( proximo.toString( Qt::SystemLocaleShortDate ) )
                .arg( getTextoFacturacion( id_servicio ) )
                .arg( periodoActual ) );
        break;
    }
    case MesCompleto:
    {
        // Cobro todo el mes como un periodo
        precio_final = precioBase( id_servicio );
        texto.append( QString( "Cobro del periodo %1 del servicio %2 por alta." )
                .arg( periodoActual )
                .arg( getNombreServicio( id_servicio ) ) );
        break;
    }
    case FInvalido:
    default:
    { delete mps; return false; break; }
 }
 delete mps;
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Genero el comprobante correspondiente
 if( !ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {

     MFactura *mf = new MFactura();
     int id_factura = mf->agregarFactura( id_cliente,
                                               QDateTime::currentDateTime(),
                                               MFactura::CuentaCorriente,
                                               precio_final );
     delete mf;
     if( id_factura <= 0 ) {
         qWarning( "Error al intentar generar la factura!" );
         return false;
     }
     MItemFactura *mif = new MItemFactura();
     if( mif->agregarItemFactura( id_factura,
                                           1,
                                           texto,
                                           precio_final,
                                           -1 ) ) { // pongo menos 1 porque no es un producto
         qWarning( "No se pudo generar el item de la factura!" );
         delete mif;
         return false;
     }
     delete mif;
     EReporte *r = new EReporte( this );
     r->factura();
     ParameterList lista;
     lista.append( "id_factura", id_factura );
     if( !r->hacer( lista ) ) {
         qWarning( "No se pudo imprimir el recibo pero quedo emitido" );
     }
 } else {
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     // Genero el comprobante correspondiente  ------>>>   SOLO HICOMP!!!!
     MPagos *mp = new MPagos();
     int id_recibo = mp->agregarRecibo( id_cliente, QDate::currentDate(), texto, precio_final, false, false );
     delete mp;
     if( id_recibo <= 0 )
     {
         qWarning( "Error al generar el recibo correspondiente" );
         return false;
     }
     EReporte *r = new EReporte( this );
     r->recibo();
     ParameterList lista;
     lista.append( "id_recibo", id_recibo );
     if( !r->hacer( lista ) ) {
         qWarning( "No se pudo imprimir el recibo pero quedo emitido" );
     }
 } // Fin hicomp - no hicomp
 return true;
}
