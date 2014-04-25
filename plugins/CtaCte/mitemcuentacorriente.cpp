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
#include "mitemcuentacorriente.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

#include "mcuentacorriente.h"

MItemCuentaCorriente::MItemCuentaCorriente(QObject *parent, bool saldos )
 : QSqlRelationalTableModel(parent)
{
 setTable( "item_ctacte" );
 setHeaderData( 0, Qt::Horizontal, "IdOperacion" );
 setHeaderData( 1, Qt::Horizontal, "Fecha" );
 setHeaderData( 2, Qt::Horizontal, "Ref Id" );
 setHeaderData( 3, Qt::Horizontal, "Tipo" );
 setHeaderData( 4, Qt::Horizontal, "Descripcion" );
 setHeaderData( 5, Qt::Horizontal, "Debe" );
 setHeaderData( 6, Qt::Horizontal, "Haber" );
 setHeaderData( 7, Qt::Horizontal, "Numero cuenta Corriente" );
 setHeaderData( 8, Qt::Horizontal, "Numero de Comprobante" );
 setHeaderData( 9, Qt::Horizontal, "Saldo" );
 setHeaderData( 10, Qt::Horizontal, "Saldo" );
 this->saldos = new QHash<int, double>();
 this->saldos->clear();
 this->_saldo = saldos;
 _num_cuenta = "";
 this->sort( 1, Qt::DescendingOrder );
 connect( this, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), this, SLOT( actualizarSaldos( QModelIndex, QModelIndex ) ) );
}


MItemCuentaCorriente::~MItemCuentaCorriente()
{
    if( saldos != 0 ) {
        delete saldos;
        saldos = 0;
    }
}

Qt::ItemFlags MItemCuentaCorriente::flags(const QModelIndex& /*index*/) const
{
    return QFlags<Qt::ItemFlag>( !Qt::ItemIsEditable |  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

/*!
 * \fn MItemCuentaCorriente::sort( int column, Qt::SortOrder order )
 * Esta función nos permite "resetear" la lista de saldos cada vez que se reordenan los elementos.
 * \param column Columna por la cual se ordena
 * \param order Dirección de ordenamiento
 */
void MItemCuentaCorriente::sort( int column, Qt::SortOrder order )
{
    if( saldos != 0 )
        if( saldos->count() > 0 )
            saldos->clear();
    QSqlRelationalTableModel::sort( column, order );
}


/*!
    \fn MItemCuentaCorriente::agregarOperacion( const QString &numero_cuenta, const QString &num_comb, const int &num_ref, const TipoOperacionCtaCte tipo, const QDate &fecha, const QString &descripcion, const double &aplicar )
        Genera un registro unico de la opercion en cuenta corriente sin hacer confirmacion ( commit ) en la base de datos.
        @param numero_cuenta Numero de cuenta corriente a aplicar la operacion
        @param num_comb Numero del comprobante relacionado a la operacion
        @param num_ref ID de clave foranea que tiene la operacion
        @param tipo Tipo de operación
        @param fecha Fecha de la operacion
        @param descripcion Descripcion de la operacion
        @param aplicar Valor a aplicar a la cuenta. Si es positivo es en favor nuestro -> el cliente nos debe. Si es negativo  es a favor del cliente -> recibimos pago ( ej ).
        @return ID de la nueva operación o -1 si hubo un error.
 */
int MItemCuentaCorriente::agregarOperacion( const QString &numero_cuenta, const QString &num_comb, const int &num_ref, const TipoOperacionCtaCte tipo, const QDate &fecha, const QString &descripcion, const double &aplicar )
{
 QSqlQuery cola;
 cola.prepare( "INSERT INTO item_ctacte"
               "( id_ctacte, fecha, numero_comprobante, id_referencia, tipo_op, descripcion, debe, haber ) VALUES "
               "( :id_ctacte, :fecha, :num_comb, :id_referencia, :tipo_op, :descripcion, :debe, :haber )"
             );
 cola.bindValue( ":id_ctacte", numero_cuenta );
 cola.bindValue( ":fecha", fecha );
 cola.bindValue( ":num_comb", num_comb );
 cola.bindValue( ":id_referencia", num_ref );
 cola.bindValue( ":tipo_op", tipo );
 cola.bindValue( ":descripcion", descripcion );
 if( aplicar > 0.0 )
 {
  cola.bindValue( ":debe", aplicar );
  cola.bindValue( ":haber", 0.0 );
 }
 else
 {
  cola.bindValue( ":debe", 0.0 );
  cola.bindValue( ":haber", (-1) * aplicar );
 }

 if( cola.exec() )
 {
  // Actualizo el saldo
  bool act = MCuentaCorriente::actualizarSaldo( numero_cuenta, aplicar );
  QVariant id = cola.lastInsertId();
  if( act ){
      if( id.isValid() ) {
        return id.toInt();
      } else {
          qDebug( "ID de operación retornado invalido" );
          return -1;
      }
  } else {
      qDebug( "Error al actualizar el saldo en la cuenta corriente" );
      return -1;
  }
 }
 else
 {
  qWarning( "Error al intentar guardar la operacion de item de cuenta corriente" );
  qDebug() << cola.lastError().text();
  qDebug() << cola.lastQuery();
  return -1;
 }

}


#include <QDate>
/*!
    \fn MItemCuentaCorriente::data(const QModelIndex& item, int role) const
 */
QVariant MItemCuentaCorriente::data(const QModelIndex& item, int role) const
{
  switch( role )
 {
        case Qt::DisplayRole:
        {
                switch( item.column() )
                {
                        case 3:
                        {
                                switch( QSqlRelationalTableModel::data(item, role).toInt() )
                                {
                                        case Factura:
                                        { return "F"; break; }
                                        case Recibo:
                                        { return "R"; break; }
                                        case NotaCredito:
                                        { return "NC"; break; }
                                        case NotaDebito:
                                        { return "ND"; break; }
                                        case CobroServicio:
                                        { return "CS"; break; }
                                        case RecargoCobroServicio:
                                        { return "RCS"; break; }
                                        case AnulacionFactura:
                                        { return "FA"; break; }
                                        default:
                                        { return "";  break; }
                                }
                                break;
                        }
                        case 5:
                        case 6:
                        {
                                return QString( "$ %L1" ).arg( QSqlRelationalTableModel::data(item, role).toDouble(), 10, 'f', 2 );
                                break;
                        }
                        case 1:
                        {
                                return QSqlRelationalTableModel::data( item, role ).toDate().toString( Qt::DefaultLocaleShortDate );
                                break;
                        }
                        case 9:
                        {
                                // Genero el saldo si no esta generado.
                                if( _saldo && item.row() == this->rowCount() - 1 )
                                {
                                    saldos->insert( item.row(),
                                                    this->data( this->index( item.row(), 6 ), Qt::EditRole ).toDouble() +
                                                    this->data( this->index( item.row(), 5 ), Qt::EditRole ).toDouble()
                                                   );
                                    for( int i = this->rowCount() - 2; i>= 0; i-- ) {
                                        saldos->insert( i,
                                                        saldos->value( i + 1 ) -
                                                        this->data( this->index( i, 6 ), Qt::EditRole ).toDouble() +
                                                        this->data( this->index( i, 5 ), Qt::EditRole ).toDouble()
                                                       );
                                    }
                                }
                                return QString( "$ %L1" ).arg( saldos->value( item.row() ), 10, 'f', 2 );
                                break;
                        }
                        default:
                        {
                                return QSqlRelationalTableModel::data(item, role);
                                break;
                        }
                }
                break;
        }
        case Qt::TextAlignmentRole:
        {
                switch( item.column() )
                {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                                return int( Qt::AlignCenter | Qt::AlignVCenter );
                                break;
                        }
                        case 5:
                        case 6:
                        case 9:
                        case 10:
                        {
                                return int( Qt::AlignRight | Qt::AlignVCenter );
                                break;
                        }
                        default:
                        {
                                return QSqlRelationalTableModel::data(item, role);
                                break;
                        }
                }
                break;
        }
        case Qt::EditRole:
        {
                switch( item.column() )
                {
                        case 3:
                        { return QSqlRelationalTableModel::data( item, role ).toInt(); break; }
                        case 9:
                        {
                            if( !saldos->keys().contains( item.row() ) )
                            { return 0.0; }
                            else
                            { return saldos->value( item.row() ); }
                        }
                        default:
                        { return QSqlRelationalTableModel::data( item, role ); break; }
                }
                break;
        }
        default:
        {
                return QSqlRelationalTableModel::data(item, role);
        }
 }
}


/*!
    \fn MItemCuentaCorriente::seleccionarNumCuenta( const QString &num_cuenta )
 */
bool MItemCuentaCorriente::seleccionarNumCuenta( const QString &num_cuenta )
{
  _num_cuenta = num_cuenta;
  if( _num_cuenta.isEmpty() )
  {
   this->setFilter( _num_cuenta );
  }
  else
  {
      if( MCuentaCorriente::existeCuenta( num_cuenta ) ) {
        this->setFilter( QString( "num_cuenta = %1" ).arg( _num_cuenta ) );
      } else {
          qWarning( "El numero de cuenta corriente no existe" );
          return false;
      }
  }
  return true;
}

void MItemCuentaCorriente::actualizarSaldos( QModelIndex idx_inicio, QModelIndex idx_fin )
{
    for( int i = idx_inicio.row(); i<=idx_fin.row(); i++ ) {
        if( i == rowCount() ) {
            saldos->insert( 0, this->data( this->index( i, 6 ), Qt::EditRole ).toDouble() +
                    this->data( this->index( i, 5 ), Qt::EditRole ).toDouble() );
            emit dataChanged( this->index( i-1, 9 ), this->index( i-1, 9 ) );
        } else {
            saldos->insert( i, saldos->value(i+1) -
                    this->data( this->index( i, 6 ), Qt::EditRole ).toDouble() +
                    this->data( this->index( i, 5 ), Qt::EditRole ).toDouble() );
            if( i != 0 ) {
                emit dataChanged( this->index( i, 9 ), this->index( i-1, 9 ) );
            }
        }
    }
}

/*!
 * \fn MItemCuentaCorriente::agregarOperacion( const QString &numero_cuenta, const NumeroComprobante &num_comb, const int &num_ref, const TipoOperacionCtaCte tipo, const QDate &fecha, const QString &descripcion, const double &aplicar )
 */
int MItemCuentaCorriente::agregarOperacion( const QString &numero_cuenta, const NumeroComprobante &num_comb, const int &num_ref, const TipoOperacionCtaCte tipo, const QDate &fecha, const QString &descripcion, const double &aplicar ) {
    NumeroComprobante *c = new NumeroComprobante( num_comb );
    return agregarOperacion( numero_cuenta, c->aCadena(), num_ref, tipo, fecha, descripcion, aplicar );
}

double MItemCuentaCorriente::valorOperacion( const int id_op_ctacte ) {
    QSqlQuery cola;
    cola.prepare( "SELECT debe, haber FROM item_ctacte WHERE id_op_ctacte = :id_op_ctacte" );
    cola.bindValue( ":id_op_ctacte", id_op_ctacte );
    if( cola.exec() ) {
        if( cola.next() ) {
            return cola.record().value(0).toDouble() - cola.record().value(1).toDouble();
        } else {
            qDebug() << "CtaCte::MItemCuentaCorriente::valorOperación:: Error en el next de la cola para obtener los valores de la operación.";
            qDebug() << cola.lastError().text();
            return 0.0; /// \todo ver si esto es 0 o -1
        }
    } else {
        qDebug() << "CtaCte::MItemCuentaCorriente::valorOperación:: Error en el next de la cola para obtener los valores de la operación.";
        qDebug() << cola.lastError().text();
        return -1.0;
    }
}

int MItemCuentaCorriente::columnCount(const QModelIndex& /*parent*/) const
{
    if( _saldo ) {
        return QSqlRelationalTableModel::columnCount()+1;
    } else {
        return QSqlRelationalTableModel::columnCount();
    }
}

/*!
 * \fn MItemCuentaCorriente::buscarIDPorComprobante( TipoOperacionCtaCte tipo, int id )
 * Busca el identificador del movimiento de cuenta corriente repectivo al tipo de comprobante y id pasado como parametro.
 * Devuelve -1 en caso de no encontrarlo o que exista algun error.
 * \param tipo Tipo de Comprobante
 * \param id Identificador unico del comprobante.
 * \returns -1 en error o no encontrado
 */
int MItemCuentaCorriente::buscarIDPorComprobante( TipoOperacionCtaCte tipo, int id )
{
    if( id <= 0 ) {
        return -1;
    }
    QSqlQuery cola;
    if( cola.exec(
                QString( "SELECT id_op_ctacte FROM item_ctacte WHERE tipo_op = %1 AND id_referencia = %2" ).arg( tipo ).arg( id )
                ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qDebug( "no se encontro el id de operacion de ctacte por tipo/id de comprobante" );
        }
    } else {
        qDebug() << "Error al ejecutar la cola de busqueda de id de operacion de ctacte por tipo/id comprobante" ;
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}

/*!
 * \fn MItemCuentaCorriente::buscarNumeroCuentaCorrientePorIdOperacion( const int id_op_ctacte )
 *
 */
QString MItemCuentaCorriente::buscarNumeroCuentaCorrientePorIdOperacion( const int id_op_ctacte )
{
    if( id_op_ctacte <= 0 ) {
        return QString();
    }
    QSqlQuery cola;
    if( cola.exec(
                QString( "SELECT id_ctacte FROM item_ctacte WHERE id_op_ctacte = %1" ).arg( id_op_ctacte )
                ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toString();
        } else {
            qDebug( "no se encontro el id de cuentacte por id de operacion" );
        }
    } else {
        qDebug() << "Error al ejecutar la cola de busqueda de id de ctacte por id de operacion";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QString();
}

/*!
 * \fn MItemCuentaCorriente::cancelarOperacion( const int id_op_ctacte, QString razon, QDateTime fechahora )
 * ATENCION! Este metodo solo elimina la entrada de la operación de cuenta corriente, no realiza ninguna otra operación que no sea recalculo del saldo de cuenta corriente.
 * \param id_op_ctacte Identificador de la operacion de cuenta corriente.
 * \return Verdadero si pudo ser eliminado correctamente.
 */
bool MItemCuentaCorriente::cancelarOperacion( const int id_op_ctacte )
{
    QSqlQuery cola;
    // Saco el numero de cuenta para recalcular su saldo
    QString numero_cuenta = MItemCuentaCorriente::buscarNumeroCuentaCorrientePorIdOperacion( id_op_ctacte );
    if( numero_cuenta.isNull() || numero_cuenta.isEmpty() ) {
        qWarning( "Error al buscar el identificador de cuenta corriente cuando se intenta eliminar una entrada de su cuenta corriente." );
        return false;
    }
    // Elimino el registro de la operacion
    if( !cola.exec( QString( "DELETE FROM item_ctacte WHERE id_op_ctacte = %1" ).arg( id_op_ctacte ) ) ) {
        qWarning( "No se puede eliminar la operacion guardada en la cuenta corriente" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }
    // Busco la cuenta corriente que le corresponde a la operacion y actualizo su saldo
    if( !MCuentaCorriente::recalcularSaldo( numero_cuenta ) ) {
        qDebug() << "Error al actualizar el saldo de la cuenta corriente";
        return false;
    }
    return true;
}
