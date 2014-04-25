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
#include "vestablecimiento.h"

#include "mestablecimiento.h"
#include <QSqlRecord>
#include <QTableView>

VEstablecimiento::VEstablecimiento(QWidget *parent)
 : EVLista( parent, this )
{
 setAttribute( Qt::WA_DeleteOnClose );
 this->setObjectName( "establecimiento" );
 modelo = new MEstablecimiento( this );
 vista->setModel( modelo );
 vista->hideColumn( 0 );
 modelo->select();

 addAction( ActAgregar );
 addAction( ActCerrar );

 this->setWindowIcon( QIcon( ":/imagenes/establecimiento.png" ) );
 this->setWindowTitle( "Establecimientos" );
}


VEstablecimiento::~VEstablecimiento()
{
}

void VEstablecimiento::antes_de_insertar(int row, QSqlRecord& record)
{
 record.setValue( 1, QString( "nombre" ) );
 record.setValue( 2, QString( "#respma" ) );
 record.remove( 0 );
}

void VEstablecimiento::eliminar()
{
	return;
    EVLista::eliminar();
}

