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
#include "mtri.h"

MTri::MTri( QObject *padre )
 : QSqlRelationalTableModel(padre)
{
 setTable( "car_tri" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "#DTA" );
 setHeaderData( 2, Qt::Horizontal, "Categoria" );
 setRelation  ( 2, QSqlRelation ( "car_categorias", "id_categoria", "nombre" ) );
 setHeaderData( 3, Qt::Horizontal, "Est. Destino" );
 setRelation  ( 3, QSqlRelation ( "car_establecimientos", "id_establecimiento", "nombre" ) );
 setHeaderData( 4, Qt::Horizontal, "Est. Origen" );
 setRelation  ( 4, QSqlRelation ( "car_establecimientos", "id_establecimiento", "nombre" ) );
 setHeaderData( 5, Qt::Horizontal, "Comprador" );
 setRelation  ( 5, QSqlRelation ( "clientes", "id", "nombre" ) );
 setHeaderData( 6, Qt::Horizontal, "Vendedor" );
 setRelation  ( 6, QSqlRelation ( "clientes", "id", "nombre" ) );
 setHeaderData( 7, Qt::Horizontal, "# Guia" );
}


MTri::~MTri()
{
}


QVariant MTri::data(const QModelIndex& item, int role) const
{
    return QSqlRelationalTableModel::data(item, role);
}

bool MTri::setData(const QModelIndex& item, const QVariant& value, int role)
{
    return QSqlRelationalTableModel::setData(item, value, role);
}

