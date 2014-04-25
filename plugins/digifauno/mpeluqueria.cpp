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
#include "mpeluqueria.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QDate>
#include <QColor>

MPeluqueria::MPeluqueria(QObject *parent , bool relacion )
 : QSqlRelationalTableModel(parent)
{
 setTable( "peluqueria" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Mascota" );
 setHeaderData( 2, Qt::Horizontal, "Descripcion" );
 setHeaderData( 3, Qt::Horizontal, "Precio" );
 setHeaderData( 4, Qt::Horizontal, "Fecha" );
 limite = -1;
 if( relacion == true )
 {
  setRelation( 1, QSqlRelation( "mascota", "id", "nombre" ) );
 }
}


MPeluqueria::~MPeluqueria()
{
}


QVariant MPeluqueria::data(const QModelIndex& idx, int role) const
{
  if( !idx.isValid() )
 {
   qDebug( QString( "Indice invalido Peluqueria: col=%1, row=%2, role=%3").arg( idx.column() ).arg( idx.row() ).arg( role ).toLocal8Bit() );
   return( QVariant() );
 }
 switch( role )
 {
	case Qt::DisplayRole:
	{
		switch( idx.column() )
		{
			// ID
			case 0:
			{
				return QSqlTableModel::data(idx, role).toString().prepend( "#" );
				break;
			}
			case 3:
			{
				return QSqlRelationalTableModel::data( idx, role ).toString().prepend( "$" );
				break;
			}
			case 4:
			{
				return QSqlRelationalTableModel::data( idx, role ).toDate().toString( "dd-MM-yyyy" );
			}
			default:
			{
				return QSqlTableModel::data(idx, role);
				break;
			}
		}
		break;
	}
	case Qt::TextColorRole:
	{
		switch ( idx.column() )
		{
			case 3:
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
		return QSqlTableModel::data( idx, role );
		break;
	}
	case Qt::TextAlignmentRole:
	{
		switch ( idx.column() )
		{
			case 3:
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
	case Qt::CheckStateRole:
	{
		return QSqlTableModel::data( idx, role );
		break;
	}
	default:
	{
		return QSqlTableModel::data( idx, role);
		break;
	}
 }
}

bool MPeluqueria::setData(const QModelIndex& item, const QVariant& value, int role)
{
    return QSqlRelationalTableModel::setData(item, value, role);
}



/*!
    \fn MPeluqueria::agregarServicio( QVariant desc, QVariant precio, QVariant fecha, QVariant id_mascota )
 */
bool MPeluqueria::agregarServicio( QVariant desc, QVariant precio, QVariant fecha, QVariant id_mascota )
{
/* if( !mascota.isValid() )
 {
   qDebug( "Inidce Mascota es invalido" );
   return false;
 }
 QVariant id_mascota = relationModel( 1 )->data( mascota, 0 );
 if( id_mascota.isValid() && id_mascota.toInt() <= 0 )
 {return false;}*/
 QSqlRecord registro = record();
 registro.setValue( 1, id_mascota );
 registro.setValue( 2, desc );
 registro.setValue( 3, precio );
 registro.setValue( 4, fecha );
 for( int i = 0; i<registro.count(); i++ )
 {
   qDebug( QString( "Campo: %1, generado? %2, null? %3, valor? |%4|" ).arg( registro.fieldName( i ) ).arg( registro.isGenerated( i ) ).arg( registro.isNull( i ) ).arg( registro.value( i ).toString() ).toLocal8Bit() );
 }
 if( insertRecord( -1, registro ) )
 {
   qDebug( "Registro de servicio agregado correctamente" );
   return true;
 }
 else
 {
   qDebug( "Error al insertar registro de servicio de mascota" );
   qDebug( QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( lastError().type() ).arg( lastError().number() ).arg( lastError().text() ).toLocal8Bit() );
   return false;
 }
}

/*!
    \fn MPeluqueria::flags ( const QModelIndex & index ) const
     Reimplementacion necesaria para determinar que los items de la columna 6 no son editables
 */
Qt::ItemFlags MPeluqueria::flags ( const QModelIndex & index ) const
{
 if ( _solo_lectura )
 {
   return Qt::ItemIsSelectable && Qt::ItemIsEnabled && !Qt::ItemIsEditable;
 }
 else
 {
  return QSqlRelationalTableModel::flags( index );
 }
}


/*!
    \fn MPeluqueria::setLimite( int lim )
 */
void MPeluqueria::setLimite( int lim )
{
 limite = lim;
}


/*!
    \fn MPeluqueria::rowCount ( const QModelIndex & parent ) const
 */
int MPeluqueria::rowCount( const QModelIndex & parent ) const
{
 int valor_real = QSqlRelationalTableModel::rowCount( parent );
 if ( limite != -1 && valor_real > limite )
 {
  return limite;
 }
 else
 {
  return valor_real;
 }
}
