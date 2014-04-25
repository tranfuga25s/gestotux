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
#include "mrecargos.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "mservicios.h"

MRecargos::MRecargos( QObject *parent, bool relaciones )
 : QSqlRelationalTableModel(parent), _precio_base(0.0)
{
 setTable( "recargos" );
 setHeaderData( 0, Qt::Horizontal, tr( "#ID" ) );
 setHeaderData( 1, Qt::Horizontal, tr( "Servicio" ) );
 setHeaderData( 2, Qt::Horizontal, tr( "Dias pasados" ) );
 setHeaderData( 3, Qt::Horizontal, tr( "Porcentaje del precio base" ) );
 setHeaderData( 4, Qt::Horizontal, tr( "Recargo Fijo" ) );
 setHeaderData( 5, Qt::Horizontal, tr( "Precio recargado final" ) );
 if( relaciones ) {
     setRelation  ( 1, QSqlRelation( "servicios", "id_servicio", "nombre" ) );
 }
}

/*!
 * \brief MRecargos::setData
 * \param idx
 * \param value
 * \param role
 * \return
 */
bool MRecargos::setData(const QModelIndex& idx, const QVariant& value, int role)
{
    switch( idx.column() ) {
    case 3:
    case 4:
    case 5:
        {
            bool valor = QSqlRelationalTableModel::setData( idx, value, role );
            emit dataChanged( index( idx.row(), 5 ), index( idx.row(), 5 ) );
            return valor;
            break;
        }
    default:
        {
            return QSqlRelationalTableModel::setData(idx, value, role);
            break;
        }
    }
}

/*!
 * \brief MRecargos::data
 * \param idx
 * \param role
 * \return
 */
QVariant MRecargos::data(const QModelIndex& idx, int role) const
{
 switch( role )
 {
  case Qt::EditRole:
  {
   switch( idx.column() )
   {
    case 0:
    case 2:
    { return QSqlRelationalTableModel::data( idx, role ).toInt(); break; }
    case 3:
    case 4:
    { return QSqlRelationalTableModel::data( idx, role ).toDouble(); break; }
    default:
    { return QSqlRelationalTableModel::data( idx, role ); break; }
   }
   break;
  }
  case Qt::DisplayRole:
  {
   switch( idx.column() )
   {
    case 3:
    { return QString( "%L1 %").arg( QSqlRelationalTableModel::data( idx, role ).toDouble() ); break; }
    case 4:
    { return QString( "$ %L1").arg( QSqlRelationalTableModel::data( idx, role ).toDouble() ); break; }
    case 5:
    {
     if( this->data( this->index( idx.row(), 3 ), Qt::EditRole ).toDouble() > 0.0 )
     {
      return QString( "$ %L1" ).arg( _precio_base + ( _precio_base * this->data( this->index( idx.row(), 3 ), Qt::EditRole ).toDouble() *.01 ) );
     }
     else if( this->data( this->index( idx.row(), 4 ), Qt::EditRole ).toDouble() > 0.0 )
     {
      return QString( "$ %L1" ).arg( _precio_base + this->data( this->index( idx.row(), 4 ), Qt::EditRole ).toDouble() );
     }
     else
     {
      return QString( "$ %L1" ).arg( _precio_base );
     }
     break;
    }
    default:
    { return QSqlRelationalTableModel::data( idx, role ); break; }
   }
  }
  case Qt::TextAlignmentRole:
  { return int( Qt::AlignCenter | Qt::AlignVCenter ); break; }
  default:
  { return QSqlRelationalTableModel::data( idx, role); break; }
 }
 return QSqlRelationalTableModel::data( idx, role );
}



/*!
    \fn MRecargos::agregarRecargo()
    Agrega un tipo de recargo a la lista de recargos para un servicio.
 */
void MRecargos::agregarRecargo()
{
 if( _servicio_actual <= 0 ) {
   qDebug() << "Error, no hay un id de servicio seteado ( id <= 0 )";
   return;
 } else {
   qDebug() << "id_servicio = " << _servicio_actual;
 }
 QSqlRecord registro = this->record();
 registro.remove( 0 );
 registro.setValue( "id_servicio", _servicio_actual );
 registro.setValue( "cant_dias", 1 );
 registro.setGenerated( 3, true );
 /*for( int i = 0; i < registro.count(); i++ ) {
     qDebug( QString( "%1: %2 - %3" ).arg( i ).arg( registro.fieldName( i ) ).arg( registro.value( i ).toString() ).toLocal8Bit() );
 }*/
 if( !this->insertRecord( -1, registro ) ) {
     qDebug( "No se pudo agregar el registro" );
 }
}


/*!
    \fn MRecargos::setearServicio( int id_servicio )
    Metodo para setear el filtro de los recargos actuales segun el ID.
    El ID debe ser mayor que cero.
    @param id_servicio ID del servicio a filtrar.
 */
void MRecargos::setearServicio( int id_servicio )
{
    if( id_servicio > 0 ) {
        this->_servicio_actual = id_servicio;
        this->setFilter( QString( "id_servicio = %1" ).arg( id_servicio ) );
        // Busco el precio base
        this->_precio_base = MServicios::precioBase( _servicio_actual );
        this->select();
    } else {
        qDebug() << "MRecargos::setearServicio::id de servicio erroneo: " << id_servicio;
    }
}


/*!
    \fn MRecargos::setearPrecioBase( double precio )
    Setea el precio base para el servicio seleccionado en la clase para calcular los precios.
    @param precio Precio a setear
 */
void MRecargos::setearPrecioBase( double precio )
{
    _precio_base = precio;
    emit dataChanged( index( 0, 5 ), index( rowCount(), 5 ) );
}


/*!
    \fn MRecargos::columnCount( const QModelIndex & index ) const
    Sobre carga para indicar de que existe una columna mas que calcula el sobrecargo.
 */
int MRecargos::columnCount( const QModelIndex &/*index*/ ) const
{
 return QSqlRelationalTableModel::columnCount() + 1;
}

#include "mperiodoservicio.h"
/*!
 * \fn double MRecargos::calcularRecargo( const int id_recargo, bool precio_final )
 * Devuelve la cantidad a aplicar de recargo precio del servicio * porcentaje o recargo fijo, si precio_final es verdadero se agrega el precio base a lo devuleto
 * \param id_recargo Identificador del recargo
 * \param precio_final Precio final del servicio incluyendo la base si es verdadero, sino retornara solamente el recargo que se le debe aplicar al precio base.
 * \returns cantidad a recargar
 */
double MRecargos::calcularRecargo( const int id_recargo, bool precio_final )
{
    QSqlQuery cola;
    // busco el tipo de recargo
    cola.prepare( "SELECT porcentaje, recargo, id_servicio FROM recargos WHERE id_recargo = :id_recargo" );
    cola.bindValue( ":id_recargo", id_recargo );
    if( !cola.exec() ) {
        qDebug() << "Error al ejecutar la cola de averiguacionde los datos del recargo";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1.0;
    }
    if( !cola.next() ) {
        qDebug() << "El recargo pasado como parametro no exite!";
        return -1.0;
    }
    double porcentaje = cola.record().value(0).toDouble();
    double recargo = cola.record().value(1).toDouble();
    int id_servicio = cola.record().value(2).toInt();
    if( id_servicio == 0 ) {
        qDebug( "Error al averiguarl el id del servicio cuando se busca un recargo" );
    }
    double precio = MServicios::precioBase( id_servicio );
    if( precio <= 0.0 ) {
        qDebug( "Existio un error al intentar buscar el precio base del servicio buscando el recargo" );
        return -1.0;
    }
    if( porcentaje > 1 ) { porcentaje = porcentaje / 100; }
    double aplicar = 0.0;
    if( recargo <= 0.0 ) {
        if( precio_final ){
            aplicar = precio * ( 1 + porcentaje );
        } else {
            aplicar = precio * porcentaje;
        }
    } else {
        if( precio_final ) {
            aplicar =  precio + recargo;
        } else {
            aplicar = recargo;
        }
    }
    return aplicar;
}

/*!
 * \brief MRecargos::calcularRecargoGenerico
 * Calcula el recargo de un recibo cualquiera sobre los recargos del primer servicio que se encuentre
 * \param precio_base Precio base del recibo
 * \param fecha_emision Fecha de emisión del recibo
 * \return Valor que se debe agregar al precio del recibo por recargo de pago a destermino
 */
double MRecargos::calcularRecargoGenerico( double precio_base, QDate fecha_emision )
{
    // Busco los recargos comunes
    QSqlQuery cola;
    if( cola.exec( "SELECT id_servicio FROM recargos GROUP BY id_servicio HAVING COUNT( id_servicio ) >= 2 LIMIT 1" ) ) {
        if( cola.next() ) {
            int id_servicio = cola.record().value(0).toInt();
            // Busco los recargos del servicio.
            int cantidad_dias_pasados = fecha_emision.daysTo( QDate::currentDate() );
            //qDebug() << "Cantidad de días pasados: " << cantidad_dias_pasados;
            if( cantidad_dias_pasados <= 0 ) {
                return 0.0; // Evita que se calcule con el primer recargo.
            }
            double ultimo_porcentaje = 0.0;
            double ultimo_recargo = 0.0;
            if( cola.exec( QString( "SELECT cant_dias, porcentaje, recargo FROM recargos WHERE id_servicio = %1" ).arg( id_servicio ) ) ) {
                while( cola.next() ) {
                    int cant_dias = cola.record().value(0).toInt();
                    //qDebug() << "Probando contra Ndias="<<cant_dias;
                    if( cantidad_dias_pasados <= cant_dias ) {
                        if( ultimo_recargo == 0.0 && ultimo_porcentaje == 0.0 ) {
                            // Estoy en el caso de que la cantidad de días es menor al primer recargo
                            // Si entró en el primer recargo, la cantidad de días esta antes de la aplicación.
                            //qDebug() << "Primer if";
                            return 0.0;
                        } else {

                            if( ultimo_recargo != 0.0 ) {
                                return ultimo_recargo;
                            } else {
                                return ultimo_porcentaje * precio_base;
                            }
                        }
                    } else {
                        // Actualizo los valores para que me entre bien en el for anterior
                        ultimo_recargo = cola.record().value(2).toDouble();
                        //qDebug() << "Ultimo_recargo:"<<ultimo_recargo;
                        ultimo_porcentaje = cola.record().value(1).toDouble();
                        //qDebug() << "Ultimo_porcentaje:"<<ultimo_porcentaje;
                        //qDebug() << "Actualizados datos";
                    }
                }
                //qDebug() << "Fin ciclo!";
                // Si llego hasta aca, aplico los ultimos valores
                if( ultimo_recargo != 0.0 ) {
                    return ultimo_recargo;
                } else {
                    return ultimo_porcentaje * precio_base;
                }
            } else {
                qDebug() << "Error al ejecutar la cola de averiguacion de recargos para un servicio generico";
                qDebug() << cola.lastError().text();
                qDebug() << cola.lastQuery();
            }
        } else {
            qDebug() << "No se encontró ningun servcio con 2 recargos echos";
        }
    } else {
        qDebug() << "Error al intentar encontrar un servicio con 2 recargos echos";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return precio_base;
}
