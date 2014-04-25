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
#include "mduenos.h"

#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>

MDuenos::MDuenos(QObject *parent)
 : QSqlQueryModel(parent)
{
 this->setQuery( "SELECT * FROM clientes WHERE id IN ( SELECT id_cliente FROM car_carv_duenos )" );
}


MDuenos::~MDuenos()
{
}

QStringList MDuenos::getLista()
{
 QStringList l;
 QString anterior = this->query().lastQuery();
 this->setQuery( "SELECT * FROM clientes" );
 for( int i = 0; i<rowCount(); i++ )
 {
  l.append( QString( "%1, %2" ).arg( this->data( this->index(i,1), Qt::DisplayRole).toString() ).arg( this->data( this->index(i,2), Qt::DisplayRole).toString() ) );
 }
 this->setQuery( anterior );
 return l;
}
