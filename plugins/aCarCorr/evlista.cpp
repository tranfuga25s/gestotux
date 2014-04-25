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
#include "evlista.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QAction>
#include <QGridLayout>

EVLista::EVLista( QWidget *parent, QObject *child, Qt::WFlags fl )
: EVentana( parent, fl )
{
 QGridLayout *layout = new QGridLayout( this );
 vista = new QTableView(this);
 vista->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
 layout->addWidget( vista );
 this->setLayout( layout );

 // Propiedades varias
 vista->setSelectionMode( QAbstractItemView::SingleSelection );
 vista->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
 vista->setTextElideMode( Qt::ElideRight );

 //////////////////////////////////////////////////////////////////////////////////////////
 // Acciones Genericas
 ////////////////////////////////////////////////////////////////////////////////////////
 ActAgregar = new QAction( "&Agregar", this );
 ActAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
 ActAgregar->setShortcut( QKeySequence( "Ctrl+a" ) );
 ActAgregar->setToolTip( "Agregar un nuevo item ( Ctrl + a )" );
 connect( ActAgregar, SIGNAL( triggered() ), child, SLOT( agregar() ) );

 ActModificar = new QAction( "&Modificar", this );
 ActModificar->setIcon( QIcon( ":/imagenes/editar.png" ) );
 ActModificar->setShortcut( QKeySequence( "Ctrl+m" ) );
 ActModificar->setToolTip( "Modifica el item actual ( Ctrl + m )" );
 connect( ActModificar, SIGNAL( triggered() ), child, SLOT( modificar() ) );

 ActImprimir = new QAction( "&Imprimir", this );
 ActImprimir->setIcon( QIcon( ":/imagenes/impresora.png" ) );
 ActImprimir->setToolTip( "Imprime el/los items seleccionados ( Ctrl + i )" );
 ActImprimir->setShortcut( QKeySequence( "Ctrl+i" ) );
 connect( ActImprimir, SIGNAL( triggered() ), child, SLOT( imprimir() ) );

 ActEliminar = new QAction( "&Eliminar", this );
 ActEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
 ActEliminar->setShortcut( QKeySequence( "Ctrl+e" ) );
 ActEliminar->setToolTip( "Eliminar el o los items seleccionados ( Ctrl + e )" );
 connect( ActEliminar, SIGNAL( triggered() ), child, SLOT( eliminar()  ) );

 ActBuscar = new QAction( "&Buscar", this );
 ActBuscar->setIcon( QIcon( ":/imagenes/buscar.png" ) );
 ActBuscar->setShortcut( QKeySequence( "Ctrl+b" ) );
 ActBuscar->setToolTip( "Buscar items ( Ctrl + b )" );
 connect( ActBuscar, SIGNAL( triggered() ), child, SLOT( buscar() ) );

 ActCerrar = new QAction( "Cer&rar", this );
 ActCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
 ActCerrar->setShortcut( QKeySequence( "Ctrl+r" ) );
 ActCerrar->setToolTip( "Cierra esta ventana ( Ctrl + r )" );
 connect( ActCerrar, SIGNAL( triggered() ), child, SLOT( cerrar() ) );

}


EVLista::~EVLista()
{
}


/*!
    \fn EVLista::cerrar()
    Llamada cuando se presiona el boton cerrar, cierra la ventana
 */
void EVLista::cerrar()
{
 this->close();
}


/*!
    \fn EVLista::agregar()
	Agrega un nuevo registro mediante la vista al modelo
 */
void EVLista::agregar()
{
 QSqlRecord registro = modelo->record();
 registro.remove( 0 );
 if( !modelo->insertRecord( -1, registro ) )
 {
  qDebug( "Error al insertar el registro" );
  qDebug( QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( modelo->lastError().type() ).arg( modelo->lastError().number() ).arg( modelo->lastError().text() ).toLocal8Bit() );
 }
}

/*!
    \fn EVLista::eliminar()
	Elimina los items seleccionados de la vista
 */
void EVLista::eliminar()
{
 //Preguntar al usuario si esta seguro
 QItemSelectionModel *selectionModel = vista->selectionModel();
 QModelIndexList indices = selectionModel->selectedIndexes();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para eliminar",
                   QMessageBox::Ok );
   return;
 }
 //Hacer dialogo de confirmacion..
 int ret;
 ret = QMessageBox::warning( this, "Esta seguro?",
                   QString( "Esta seguro de eliminar %1 item?").arg( indices.size() ),
                   "Si", "No" );
 if ( ret == 0 )
 {
	QModelIndex indice;
	int ultima_row = -1;
	foreach( indice, indices )
	{
		if( indice.isValid() )
		{
			if( indice.row() != ultima_row )
			{
				ultima_row = indice.row();
				modelo->removeRow( indice.row() );
			}
		}
	}
 }
 return;
}

/*!
    \fn EVLista::closeEvent( QCloseEvent * c)
	Metodo llamado al cerrarse el form
	@param c evento de cierre
 */
void EVLista::closeEvent( QCloseEvent * c)
{
 delete vista;
 modelo->submitAll();
 delete modelo;
 EVentana::closeEvent( c );
}


/*!
    \fn EVLista::buscar()
 */
void EVLista::buscar()
{
  qWarning( "No implementado" );
}


/*!
    \fn EVLista::imprimir()
 */
void EVLista::imprimir()
{
 qWarning( "No implementado" );
}


/*!
    \fn EVLista::modificar()
 */
void EVLista::modificar()
{
 qWarning( "No implementado" );
}
