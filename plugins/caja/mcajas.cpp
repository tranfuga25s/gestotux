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

#include "mcajas.h"

#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

#include "mmovimientoscaja.h"
#include "preferencias.h"

MCajas::MCajas(QObject *parent ) :
QSqlTableModel(parent)
{
    setTable( "caja" );
    this->setHeaderData( 0, Qt::Horizontal, "#ID" );
    this->setHeaderData( 1, Qt::Horizontal, "Nombre" );
    this->setHeaderData( 2, Qt::Horizontal, "Fecha de Alta"  );
    this->setHeaderData( 3, Qt::Horizontal, "Saldo Actual" );
}


QVariant MCajas::data( const QModelIndex &idx, int role ) const
{
    switch( role ) {
        case Qt::DisplayRole: {
            switch( idx.column() ) {
                case 3:
                { return QString( "$ %L1" ).arg( QSqlTableModel::data( idx, Qt::EditRole ).toDouble(), 10, 'f', 2 ); break; }
                default:
                { return QSqlTableModel::data( idx, role ); break; }
            }
            break;
        }
        case Qt::TextAlignmentRole: {
            switch( idx.column() ) {
                case 2:
                case 3:
                { return int( Qt::AlignCenter | Qt::AlignHCenter ); break; }
                default:
                { return QSqlTableModel::data( idx, role ); break; }
            }
            break;
        }
        default:
        {
            return QSqlTableModel::data( idx, role ); break;
        }
    }
}

Qt::ItemFlags MCajas::flags( const QModelIndex &/*idx*/ ) const
{
    return Qt::ItemFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
}

/*!
  @fn MCajas::agregarCaja( QString nombre, QDate fecha_alta = QDate::currentDate(), double saldo_inicial )
  Funcion para agregar una caja nueva al sistema
  @param nombre Nombre de la caja
  @param fecha_alta Fecha en que se da de alta
  @param saldo_inicial Saldo inicial de la caja
 */
bool MCajas::agregarCaja( QString nombre, QDate fecha_alta, double saldo_inicial )
{
  QSqlRecord rec = this->record();
  rec.setValue( "nombre", nombre );
  rec.setValue( "fecha_alta", fecha_alta );
  rec.setValue( "saldo", saldo_inicial );
  if( insertRecord( -1, rec ) ) {
      // Agregar el registro de transaccion inicial en la tabla de movimientos
      MMovimientosCaja *mmov = new MMovimientosCaja( this );
      if( saldo_inicial > 0.0 ) {
          if( mmov->agregarMovimiento( query().lastInsertId().toInt(), "Apertura de Caja - Saldo Inicial", QString(), saldo_inicial, 0.0, true ) )
          {  return true; } else { return false; }
      } else {
        if( mmov->agregarMovimiento( query().lastInsertId().toInt(), "Apertura de Caja - Saldo Inicial", QString(), 0.0, saldo_inicial, true ) )
        {  return true; } else { return false; }
      }
  } else {
      qWarning() << "Error al insertar caja nueva: " << this->lastError().text();
      return false;
  }
}

#include <QSqlQuery>
/*!
 * @fn MCajas::actualizarSaldo( int id_caja, double cantidad )
 * Funcion que actualiza el sado de una caja especifica
 * @param id_caja Identificador de la caja a la cual se le actualizará el saldo
 * @param cantidad Cantidad ( positiva para aumentar, negativa para disminuir ) que se modificara al saldo
 */
bool MCajas::actualizarSaldo( const int id_caja, const double cantidad )
{
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT saldo FROM %1 WHERE id_caja = %2" ).arg( "caja" ).arg( id_caja )  ) ) {
     if( cola.next() ) {
        double saldo_nuevo = cola.record().value(0).toDouble() + cantidad;
        if( cola.exec( QString( "UPDATE caja SET saldo = %1 WHERE id_caja = %2" ).arg( saldo_nuevo ).arg( id_caja ) ) ) {
            return true;
        } else {
            qWarning() <<  "Error al guardar el nuevo saldo.";
            return false;
        }
    } else {
        qWarning() << "Error al buscar el saldo anterior haciendo next. <br /> Posiblemente la caja predeterminada este mal guardada. Por favor, cambiela desde las preferencias e intente realizar la operacion nuevamente.";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
 } else {
    qWarning() <<  "Error al buscar el saldo anterior.";
    return false;
 }
}

/*!
 * @fn MCajas::saldo( const int id_caja )
 * Funcion que devuelve el saldo actual de una caja
 * @param id_caja Identificador de la caja a la cual se le actualizará el saldo
 * @return Saldo de la caja
 */
double MCajas::saldo( const int id_caja )
{
   QSqlQuery cola;
   if( cola.exec( QString( "SELECT saldo FROM %1 WHERE id_caja = %2" ).arg( "caja" ).arg( id_caja )  ) ) {
       //qDebug( QString( "Ejecutado: %1" ).arg( cola.lastQuery() ).toLocal8Bit() );
       if( cola.next() ) {
        return cola.record().value("saldo").toDouble();
       } else {
           qWarning( "Error de next en averiguar saldo de caja" );
           qDebug() << "Ejecutado: " << cola.lastQuery();
           return 0.0;
       }
   } else {
        qDebug( "Saldo no encontrado" );
        return 0.0;
   }
}

/*!
 * @fn MCajas::hacerCierre( const int id_caja, QDateTime fecha, double saldo )
 * Funcion que realiza el cierre de una caja.
 * @param id_caja Identificador de la caja a la cual se le actualizar el saldo
 * @param fecha Fecha y hora en que se realiza el cierre
 * @param saldo Saldo que se contabilizo
 * @return Saldo de la caja
 */
bool MCajas::hacerCierre( const int id_caja, QDateTime fecha, double saldo )
{
   MMovimientosCaja *m = new MMovimientosCaja();
   bool ret;
   if( m->verificarCierreCaja( id_caja ) ) {
     ret = m->agregarCierre( id_caja, fecha, saldo );
   } else {
       qWarning( "No se pudo realizar el cierre porque ya existe uno anterior y no hay operaciones aparte de esta");
       ret = false;
   }
   delete m;
   return ret;
}

/*!
 * @fn MCajas::nombreCaja( const int id_caja )
 * Funcion que retorna el nombre de una caja
 * @param id_caja Identificador de la caja
 * @returns Nombre de la caja
 */
QString MCajas::nombreCaja( const int id_caja )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT nombre FROM %1 WHERE id_caja = %2" ).arg( "caja" ).arg( id_caja )  ) ) {
        //qDebug( QString( "Ejecutado: %1" ).arg( cola.lastQuery() ).toLocal8Bit() );
        if( cola.next() ) {
         return cola.record().value(0).toString();
        } else { return QString();  }
    } else {
         qDebug( "Nombre no encontrado" );
         return QString();
    }
}

/*!
 * @fn MCajas::cajaPredeterminada()
 * Devuelve la caja que se haya elegido como predeterminada en las funciones de preferencias de cajas.
 * Si no se eligio ninguna, devuelve -1
 * @returns ID de la caja predeterminada
 */
int MCajas::cajaPredeterminada()
{
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias");
    p->beginGroup( "Caja" );
    p->beginGroup( "caja-predeterminada" );
    int id = p->value( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName(), -1 ).toInt();
    p->endGroup();
    p->endGroup();
    p->endGroup();
    p = 0;
    if( id <= 0 ) {
        // No hay ninguna caja elegida
        QSqlQuery cola;
        if( cola.exec( "SELECT COUNT(id_caja) FROM caja" ) ) {
            if( cola.next() ) {
                int cantidad = cola.record().value(0).toInt();
                if( cantidad == 1 ) {
                    // Existe una sola caja declarada... la devuelvo
                    return MCajas::buscarPrimerCaja();
                } else {
                    QMessageBox::information( 0, "Caja predeterminada", QString::fromUtf8( "No existe una caja predeterminada seleccionada. Se eligirá la primera disponible.\n Por favor, seleccióne una caja predeterminada desde las preferencias del programa" ) );
                    return MCajas::buscarPrimerCaja();
                }
            }
        } else {
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
            return false;
        }
    } else {
        // Verifico si existe la caja
        QSqlQuery cola;
        if( cola.exec( QString("SELECT COUNT(id_caja) FROM caja WHERE id_caja = %1" ).arg(id) ) ) {
            if( cola.next() ) {
                int cantidad = cola.record().value(0).toInt();
                if( cantidad == 1 ) {
                    // La caja existe
                    return id;
                }
            }
            return MCajas::buscarPrimerCaja();
        } else {
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
            return false;
        }
    }
    return false;
}

/**
 * @brief MCajas::buscarPrimerCaja
 * @return Primera caja encontrada
 */
int MCajas::buscarPrimerCaja() {
    QSqlQuery cola;
    if (cola.exec( "SELECT id_caja FROM caja LIMIT 1 " )) {
        if (cola.next()){
            return cola.record().value(0).toInt();
        } else {
            qWarning("No existe ninguna caja declarada! - Por favor agrege una.");
        }
    } else {
        qDebug() << "No se pudo ejecutar la cola para obtener la primer caja";
        qDebug() << cola.lastError().text();
    }
    return -2;
}

/*!
 * \fn MCajas::existen2omas()
 * Devuelve verdadero si existen 2 o mas cajas delcaradas, utilizado para definir si se pueden realizar transferencias
 */
bool MCajas::existen2omas()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT COUNT(nombre) FROM caja" ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 1 ) {
                return true;
            }
        }
    } else {
         qDebug() << "Error al ejecutar la cola de conteo de cajas";
         qDebug() << cola.lastError().text();
         qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MCajas::existeCaja( const QString nombre )
 * Funcion que devuelve verdadero si existe la caja con el nombre pasado como parametro
 * \param nombre Nombre a buscar
 */
bool MCajas::existeCaja( QString nombre )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(nombre) FROM %1 WHERE nombre = \"%2\"" ).arg( "caja" ).arg( nombre )  ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            }
        }
    } else {
         qDebug() << "Error al ejecutar la cola de conteo de cajas con nombre especificado.Nombre no encontrado";
         qDebug() << cola.lastError().text();
         qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MCajas::renombrarCaja( const int id_caja, QString nombre )
 * Renombra el nombre de la caja pasada como parametro
 * \param id_caja Identificador de la caja a renombrar
 * \param nombre Nuevo nombre que se le dará a la caja
 * \return Verdadero si se pudo renombrar la caja correctamente
 */
bool MCajas::renombrarCaja(const int id_caja, QString nombre)
{
    QSqlQuery cola;
    if( cola.exec( QString( "UPDATE caja SET nombre = \"%1\" WHERE id_caja = %2" ).arg( nombre ).arg( id_caja ) ) ) {
        return true;
    } else {
        qDebug() << "Error al intentar actualizar el nombre de la caja";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MCajas::tieneDatosRelacionados( const int id_caja )
 * Busca si existe algun dato relacionado con la caja y devuelve verdadero si existe.
 * \param id_caja Identificador de la caja
 * \return Verdadero si hay datos. Falso en caso contrario.
 */
bool MCajas::tieneDatosRelacionados( const int id_caja )
{
    if( id_caja == cajaPredeterminada() ) {
        qWarning( "La caja que intenta eliminar es la caja predeterminada. No se podrá eliminar" );
        return true;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id_caja) FROM movimientos_caja WHERE id_caja = %1" ).arg( id_caja ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return false;
        } else {
            qWarning( "Existen movimientos de caja que hacen referencia a esta caja." );
        }
    }
    return true;
}

/*!
 *  \fn MCajas::eliminarCaja( const int id_caja )
 * Elimina la caja que se pasa como paraemtro
 * \param id_caja Identificador de caja
 * \returns Verdadero si se pudo eliminar la caja, falso en caso contrario.
 */
bool MCajas::eliminarCaja( const int id_caja )
{
    QSqlQuery cola;
    if( cola.exec( QString( "DELETE FROM caja WHERE id_caja = %1" ).arg( id_caja ) ) ) {
        return true;
    } else {
        qDebug() << "Error al ejecutar la cola de eliminación de cajas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}
