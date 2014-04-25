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
#include "mestablecimiento.h"

MEstablecimiento::MEstablecimiento( QObject *parent )
 : QSqlTableModel( parent )
{
 setTable( "car_establecimientos" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Nombre" );
 setHeaderData( 2, Qt::Horizontal, "#RESMPA" );
}


MEstablecimiento::~MEstablecimiento()
{
}

#include <QStringList>


/*!
    \fn MEstablecimiento::listaNombres()
 */
QStringList MEstablecimiento::listaNombres()
{
 QStringList listaT;
 for( int i=0; i<this->rowCount(); i++ )
 {
  listaT.append( this->data( this->index( i, 1 ), Qt::DisplayRole ).toString() );
 }
 return listaT;
}
