/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
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
#include "mmascota.h"

#include <QSqlIndex>
#include <QSqlField>
#include <QSqlError>
#include <QColor>

MMascota::MMascota( QObject *parent )
 : QSqlRelationalTableModel( parent )
{
 setTable( "mascota" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Dueño" );
   setRelation( 1, QSqlRelation( "dueno", "id", "apellido" ) );
 setHeaderData( 2, Qt::Horizontal, "Nombre" );
 setHeaderData( 3, Qt::Horizontal, "Raza" );
 setHeaderData( 4, Qt::Horizontal, "Genero" );
}


MMascota::~MMascota()
{
}


QVariant MMascota::data(const QModelIndex& item, int role) const
{
  if( !item.isValid() )
 {
   qDebug( QString( "Indice invalido Dueños: col=%1, row=%2, role=%3").arg( item.column() ).arg( item.row() ).arg( role ).toLocal8Bit() );
   return( QVariant() );
 }
 switch( role )
 {
	case Qt::DisplayRole:
	{
		switch( item.column() )
		{
			// ID
			case 0:
			{
				return QSqlRelationalTableModel::data(item, role).toString().prepend( "#" );
				break;
			}
			case 4: // Genero
			{
				if( QSqlRelationalTableModel::data( item, role ).toInt() == 0 )
				{
					return "Macho";
				}
				else
				{
					return "Hembra";
				}
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
	case Qt::TextColorRole:
	{
		switch ( item.column() )
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
	case Qt::TextAlignmentRole:
	{
		switch ( item.column() )
		{
			case 0:
			{
				return int( Qt::AlignHCenter | Qt::AlignVCenter );
				break;
			}
			default:
			{
				return int( Qt::AlignLeft | Qt::AlignVCenter );
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
	default:
	{
		return QSqlRelationalTableModel::data( item, role);
		break;
	}
 }
}

bool MMascota::setData(const QModelIndex& item, const QVariant& value, int role)
{
    return QSqlRelationalTableModel::setData(item, value, role);
}




/*!
    \fn MMascota::agregarMascota( QVariant nombre, QVariant raza, QVariant genero, QModelIndex dueno )
 */
bool MMascota::agregarMascota( QVariant nombre, QVariant raza, QVariant genero, QModelIndex dueno )
{
 if( !dueno.isValid() )
 {return false;}
 QVariant id_dueno = relationModel( 1 )->data( dueno, 0 );
 if( id_dueno.isValid() && id_dueno.toInt() <= 0 )
 {return false;}
 QSqlRecord registro = record();
 registro.setValue( 1, id_dueno );
 registro.setValue( 2, nombre );
 registro.setValue( 3, raza );
 registro.setValue( 4, genero );
 if( this->insertRecord( -1, registro ) )
 {
  return true;
 }
 else
 {
  qDebug( "Error al insertar registro de mascota" );
  qDebug( QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( lastError().type() ).arg( lastError().number() ).arg( lastError().text() ).toLocal8Bit() );
  return false;
 }
}
