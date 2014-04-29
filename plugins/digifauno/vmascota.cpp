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
#include "vmascota.h"
#include "mmascota.h"
#include <QTableView>
#include "dmascota.h"
#include <QSqlRecord>

VMascota::VMascota( QWidget *parent )
 : EVLista( parent )
{
 setObjectName("listaMascotas");
 setWindowTitle( "Listado de Mascotas y Dueños" );
 setWindowIcon( QIcon( ":/imagenes/mascotas.gif" ) );
 modelo = new MMascota( this );
 vista->setModel( modelo );
 vista->hideColumn( 0 );
 vista->resizeRowsToContents();
 vista->setItemDelegate( new DMascota( vista ) );
 modelo->select();
 vista->setSortingEnabled( true );
 vista->setAlternatingRowColors( true );
 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActEliminar );
 addAction( ActCerrar );
}


VMascota::~VMascota()
{
}

/*!
 *  \fn VMascota::antes_de_insertar( int row, QSqlRecord &record )
 *	Funcion que setea los valores por default de el registro de una mascota
 */
void VMascota::antes_de_insertar( int row, QSqlRecord& record )
{
  record.remove( 0 );
  /*qDebug( QString( "Cantidad de campos: %1").arg( record.count() ).toLocal8Bit() );
  qDebug( "Poniendo valores por default" );
  for( int i = 0; i<record.count(); i++ )
  {
   qDebug( QString( "Campo: %1, generado? %2, null? %3, valor? |%4|" ).arg( record.fieldName( i ) ).arg( record.isGenerated( i ) ).arg( record.isNull( i ) ).arg( record.value( i ).toString() ).toLocal8Bit() );
  }*/
}


/*!
    \fn VMascota::menuContextual( const QModelIndex &indice, QMenu *menu )
 */
void VMascota::menuContextual( const QModelIndex &indice, QMenu *menu )
{
    /// @todo implement me
}
