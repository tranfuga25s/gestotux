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
#include "mdueno.h"
#include "preferencias.h"

#include <QColor>
#include <QSqlIndex>

MDueno::MDueno( QObject * parent)
 : QSqlTableModel( parent )
{
 setTable( "dueno" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Nombre" );
 setHeaderData( 2, Qt::Horizontal, "Apellido" );
 setHeaderData( 3, Qt::Horizontal, "Direccion" );
 setHeaderData( 4, Qt::Horizontal, "E-mail" );
 setHeaderData( 5, Qt::Horizontal, "Telefono" );
 setHeaderData( 6, Qt::Horizontal, "Celular" );
 setEditStrategy( QSqlTableModel::OnRowChange );
}


MDueno::~MDueno()
{
}


QVariant MDueno::data(const QModelIndex& idx, int role) const
{
 if( !idx.isValid() )
 {
   qDebug( QString( "Indice invalido Dueños: col=%1, row=%2, role=%3").arg( idx.column() ).arg( idx.row() ).arg( role ).toLocal8Bit() );
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
			case 4:
			{
			        preferencias *p = preferencias::getInstancia();
			        return QColor( p->value( "General/color_mail", "#0000FF" ).toString() );
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
		switch ( idx.column() )
		{
			case 0:
			{
				return int( Qt::AlignHCenter | Qt::AlignVCenter );
				break;
			}
			case 4:
			{
				return int( Qt::AlignHCenter | Qt::AlignVCenter );
				break;
			}
			case 5:
			case 6:
			{
				return int( Qt::AlignRight | Qt::AlignVCenter );
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
		return QSqlTableModel::data( idx, role);
		break;
	}
 }
}


/*!
    \fn MDueno::primeInsert ( int row, QSqlRecord & record )
 */
void MDueno::primeInsert ( int row, QSqlRecord & record )
{
 ultima_row = row;
 QSqlTableModel::primeInsert( row, record );
}


int MDueno::get_ultima_row() const
{
	return ultima_row;
}
