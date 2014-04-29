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
#include "mauto.h"

MAuto::MAuto(QObject *parent)
 : QSqlRelationalTableModel(parent)
{
 setTable( "autos" );
 setHeaderData( 1, Qt::Horizontal, tr( "#ID" ) );
 setHeaderData( 1, Qt::Horizontal, tr( "Patente" ) );
 setHeaderData( 2, Qt::Horizontal, tr( "Dueño" ) );
 setRelation( 2, QSqlRelation( "clientes", "id", "nombre" ) );
 setHeaderData( 3, Qt::Horizontal, tr( "Marca" ) );
 setHeaderData( 4, Qt::Horizontal, tr( "Modelo" ) );
 setHeaderData( 5, Qt::Horizontal, tr( "Año" ) );
 setHeaderData( 6, Qt::Horizontal, tr( "Color" ) );
 setHeaderData( 7, Qt::Horizontal, tr( "Modificado" ) );
 setHeaderData( 8, Qt::Horizontal, tr( "Creado" ) );
}


MAuto::~MAuto()
{
}


QVariant MAuto::data(const QModelIndex& item, int role) const
{
  // Devuelvo el id para esta situacion, el combo del delegate lo necesita
  /*if( item.column() == 2 )
  {
   return QSqlTableModel::data( item, Qt::EditRole ).toInt();
  }
  else*/
  return QSqlRelationalTableModel::data(item, role);
}
