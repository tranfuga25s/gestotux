/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   *
 *   juiraze@yahoo.com.ar   *
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
#include "mpresupuestos.h"
#include <QColor>
#include <QDate>

MPresupuestos::MPresupuestos(QObject *parent, bool relacion, bool soloLectura )
 : QSqlRelationalTableModel(parent)
{
 setTable( "presupuesto" );
 setHeaderData( 0, Qt::Horizontal, "Num Presupuesto" );
 setHeaderData( 1, Qt::Horizontal, "Fecha" );
 setHeaderData( 2, Qt::Horizontal, "Kilometraje" );
 setHeaderData( 3, Qt::Horizontal, "Automovil" );
 setHeaderData( 4, Qt::Horizontal, "Total" );
 setHeaderData( 5, Qt::Horizontal, "Titulo" );
 setHeaderData( 6, Qt::Horizontal, "Contenido" );
 setHeaderData( 7, Qt::Horizontal, "Modificado" );
 if( relacion )
 {
  setRelation( 3, QSqlRelation( "autos", "id_auto", "patente" ) );
 }
 _soloLectura = soloLectura;
}


MPresupuestos::~MPresupuestos()
{
}


QVariant MPresupuestos::data(const QModelIndex& idx, int role) const
{
 if( !idx.isValid() )
  {
   qDebug( QString( "Indice invalido presupuesto: col=%1, row=%2, role=%3").arg( idx.column() ).arg( idx.row() ).arg( role ).toLocal8Bit() );
   return( QVariant() );
  }
 switch( role )
 {
	case Qt::DisplayRole:
	{
		switch( idx.column() )
		{
			case 1:
			{
				return QSqlRelationalTableModel::data(idx, role).toDate().toString( Qt::SystemLocaleDate );
				break;
			}
			case 2:
			{
			       return QString::number( QSqlRelationalTableModel::data(idx, role).toInt() ).append( " Km" );
			       break;
			}
			default:
			{
				return QSqlRelationalTableModel::data(idx, role);
				break;
			}
		}
		break;
	}
	case Qt::TextColorRole:
	{
		switch ( idx.column() )
		{
			case 2:
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
		return int( Qt::AlignHCenter | Qt::AlignVCenter );
		break;
	}
	default:
	{
		return QSqlRelationalTableModel::data( idx, role);
		break;
	}
 }
}


/*!
    \fn MPresupuestos:: flags ( const QModelIndex & index ) const
 */
Qt::ItemFlags MPresupuestos:: flags ( const QModelIndex & index ) const
{
 if( _soloLectura )
 {
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
 }
 else
 { return QSqlRelationalTableModel::flags( index ); }
}
