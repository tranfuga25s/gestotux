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
#include "vautos.h"
#include <QIcon>
#include "mauto.h"
#include <QTableView>
#include <QSqlRelationalDelegate>
#include "edpatente.h"
#include "edmarca.h"
#include "edmodelo.h"
#include "edcolor.h"

VAutos::VAutos(QWidget *parent)
 : EVLista(parent)
{
 this->setAttribute( Qt::WA_DeleteOnClose );
 setObjectName( "VAutos" );
 setWindowTitle( "Automoviles" );
 setWindowIcon( QIcon( ":/imagenes/auto.png" ) );

 modelo = new MAuto( vista );

 vista->setModel( modelo );
 vista->hideColumn( 0 );
 vista->setItemDelegateForColumn( 2, new QSqlRelationalDelegate( vista ) );
 vista->setItemDelegateForColumn( 1, new EDPatente( vista ) );
 vista->setItemDelegateForColumn( 3, new EDMarca( vista ) );
 vista->setItemDelegateForColumn( 4, new EDModelo( vista ) );
 modelo->select();

 addAction( ActAgregar );
 addAction( ActEliminar );
 addAction( ActCerrar );
}


VAutos::~VAutos()
{}


/*!
    \fn VAutos::agregar()
 */
void VAutos::agregar()
{
 EVLista::agregar( false );
}


/*!
    \fn VAutos::contextMenuEvent ( QContextMenuEvent * e )
 */
void VAutos::contextMenuEvent ( QContextMenuEvent * e )
{
 QModelIndex indice = vista->indexAt( e->pos() );
 if( indice.isValid() )
 {
   //Creo el menu
   qWarning( "Crear menu" );
   EVLista::contextMenuEvent( e );
 }
 else
 {
  e->ignore();
 }
}
