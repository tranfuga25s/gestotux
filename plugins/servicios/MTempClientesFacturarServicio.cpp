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

#include "MTempClientesFacturarServicio.h"

#include <QColor>
#include <QSize>

MTempClientesFacturarServicio::MTempClientesFacturarServicio(QObject *parent) :
    QAbstractTableModel(parent)
{
    // Inicializo los arrays
    clientes = new QHash<int, QString>();
    marcados = new QHash<int, bool>();
    ids = new QHash<int, int>();
}

bool MTempClientesFacturarServicio::insertRow(int row, const QModelIndex& parent )
{
    if( row == -1 )
    { row = this->marcados->size(); }
    beginInsertRows( parent, row, row );
    clientes->insert( row, "" );
    marcados->insert( row, false );
    ids->insert( row, -1 );
    endInsertRows();
    emit dataChanged( this->index( row, 0 ), this->index( row, 1 ) );
    return true;

}

bool MTempClientesFacturarServicio::removeRow(int row, const QModelIndex& parent )
{
    beginRemoveRows( parent, row, row );
    clientes->remove( row );
    marcados->remove( row );
    ids->remove( row );
    endRemoveRows();
    return true;
}

bool MTempClientesFacturarServicio::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if( !index.isValid() )
    {
      //qDebug( QString( "Indice invalido Dueños: col=%1, row=%2, role=%3").arg( index.column() ).arg( index.row() ).arg( role ).toLocal8Bit() );
      return false;
    }
    switch( role )
    {
           case Qt::EditRole:
           {
                   switch( index.column() )
                   {
                           // Marcado
                           case 0:
                           {
                                this->marcados->insert( index.row(), value.toBool() );
                                emit dataChanged( index, index );
                                return true;
                                break;
                           }
                           // Cliente
                           case 1:
                           {
                                this->clientes->insert( index.row(), value.toString() );
                                emit dataChanged( index, index );
                                return true;
                                break;
                           }
                           // Id del cliente
                           case 2:
                           {
                                this->ids->insert( index.row(), value.toInt() );
                                emit dataChanged( index, index );
                                return true;
                                break;
                           }
                           default:
                           { return false; break; }
                   }
                   break;
           }
           default:
           { return false; break; }
     }
}

int MTempClientesFacturarServicio::columnCount(const QModelIndex& /*parent*/ ) const
{ return 3; }

int MTempClientesFacturarServicio::rowCount(const QModelIndex& /*parent*/ ) const
{ return this->marcados->size(); }

Qt::ItemFlags MTempClientesFacturarServicio::flags(const QModelIndex& index) const
{
  if( index.column() != 0 )
  { return QFlags<Qt::ItemFlag>(!Qt::ItemIsEditable |  Qt::ItemIsSelectable ); }
  else
  { return QFlags<Qt::ItemFlag>(Qt::ItemIsEditable | Qt::ItemIsEnabled); }

}

QVariant MTempClientesFacturarServicio::data(const QModelIndex& idx, int role) const
{
    if( !idx.isValid() )
    {
     //qDebug( QString( "Indice invalido Dueños: col=%1, row=%2, role=%3").arg( idx.column() ).arg( idx.row() ).arg( role ).toLocal8Bit() );
     return( QVariant() );
    }
    switch( role )
    {
           case Qt::DisplayRole:
           {
                   switch( idx.column() )
                   {
                           // Marcado
                           case 0:
                           {
                                   return this->marcados->value( idx.row() );
                                   break;
                           }
                           // Cliente
                           case 1:
                           {
                                   return this->clientes->value( idx.row() );
                                   break;
                           }
                           // ID cliente
                           case 2:
                           {
                                return this->ids->value( idx.row() );
                                break;
                           }
                           default:
                           {
                                   return QVariant();
                                   break;
                           }
                   }
                   break;
           }
           case Qt::TextColorRole:
           {
                   switch ( idx.column() )
                   {
                           case 0:
                           {
                                   return QColor(Qt::blue);
                                   break;
                           }
                           default:
                           {
                                   return QColor(Qt::black);
                                   break;
                           }
                   }
                   break;
           }
           case Qt::EditRole:
           {
                   switch( idx.column() )
                   {
                           // Marcado
                           case 0:
                           {
                                   // tengo que devolver si esta marcado
                                   return this->marcados->value( idx.row() );
                                   break;
                           }
                           default:
                           {
                                   return false;
                                   break;
                           }
                   }
                   break;
           }
           case Qt::TextAlignmentRole:
           {
                   switch ( idx.column() )
                   {
                           case 1:
                           {
                                   return int( Qt::AlignLeft | Qt::AlignVCenter );
                                   break;
                           }
                           default:
                           {
                                   return int( Qt::AlignRight | Qt::AlignVCenter );
                                   break;
                           }
                   }
                   break;
           }
           case Qt::ToolTipRole:
           case Qt::StatusTipRole:
           {
                   return QVariant( "Haga doble click o seleccione y F2 para editar" );
                   break;
           }
           case Qt::SizeHintRole:
           {
                    switch( idx.column() )
                    {
                         case 0:
                         { return QSize( 1, 1 ); break; }
                         case 1:
                         { return QSize( 3, 1 ); break; }
                         default:
                         { return QVariant(); break; }
                    }
                    break;
           }
           default:
           { return QVariant(); break; }
    }
}

QVariant MTempClientesFacturarServicio::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
      if( section == 0 ) { return  QString::fromUtf8( "¿Facturar?" ); } else { return "Cliente"; }
  } else {
      return section;
  }
}

#include <QSqlQuery>
#include <QSqlRecord>
/*!
 * \brief MTempClientesFacturarServicio::cargarClientesDelServicio
 * Carga todos los clientes del servicio que hay declarados
 * \param id Identificador del servicio
 */
void MTempClientesFacturarServicio::cargarClientesDelServicio( const int id )
{
    // Busco los clientes qe estan adheridos al servicio solcitiado
    QSqlQuery cola( QString( "SELECT c.razon_social, c.id FROM servicios_clientes, clientes c WHERE ( servicios_clientes.`id_cliente` = c.id ) AND ( servicios_clientes.id_servicio = %1 ) ORDER BY c.razon_social ASC" ).arg( id ) );
    //qDebug( cola.lastQuery().toLocal8Bit() );
    while ( cola.next() ) {
        this->insertRow( -1 );
        this->setData( this->index( this->rowCount()-1, 0 ), true, Qt::EditRole );
        QString val = cola.record().value(0).toString();
        this->setData( this->index( this->rowCount()-1, 1 ), val, Qt::EditRole  );
        this->setData( this->index( this->rowCount()-1, 2 ), cola.record().value(1).toInt(), Qt::EditRole );
    }
}

/*!
 * \brief MTempClientesFacturarServicio::cargarClientesDelServicioAFacturar
 *  Genera los datos del modelo con los clientes adheridos pero que no estén dados de baja
 * \param id Identificador del servicio
 */
void MTempClientesFacturarServicio::cargarClientesDelServicioAFacturar( const int id )
{
    // Busco los clientes qe estan adheridos al servicio solcitiado pero no dados de baja
    QSqlQuery cola( QString( "SELECT c.razon_social, c.id FROM servicios_clientes, clientes c WHERE ( servicios_clientes.`id_cliente` = c.id ) AND ( servicios_clientes.id_servicio = %1 ) AND servicios_clientes.fecha_baja IS NULL ORDER BY c.razon_social ASC" ).arg( id ) );
    //qDebug( cola.lastQuery().toLocal8Bit() );
    while ( cola.next() ) {
        this->insertRow( -1 );
        this->setData( this->index( this->rowCount()-1, 0 ), true, Qt::EditRole );
        QString val = cola.record().value(0).toString();
        this->setData( this->index( this->rowCount()-1, 1 ), val, Qt::EditRole  );
        this->setData( this->index( this->rowCount()-1, 2 ), cola.record().value(1).toInt(), Qt::EditRole );
    }
}