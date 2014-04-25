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
#include "eventana.h"

#include <QString>
#include <QMdiSubWindow>
#include <QIcon>

EVentana::EVentana( QWidget *padre, Qt::WFlags fl  )
 : QWidget( padre, fl )
{
 this->padre = padre;
 setAttribute( Qt::WA_DeleteOnClose );
}

EVentana::~EVentana()
{
}

#include "eayuda.h"
/*!
    \fn EVentana::keyPressEvent( QKeyEvent *event )
    Implementacion de autobusqueda del archivo de ayuda al presionar la tecla F1 en la ventana.
    @param event Evento de tecla a procesar
 */
void EVentana::keyPressEvent( QKeyEvent *event )
{
 if (  event->key() ==  Qt::Key_F1 )
 {
   EAyuda *ayuda = EAyuda::instancia();
   QString buscar;
   if( this->focusWidget()->objectName().isEmpty() )
   {
     // No hay objeto seleccionado, busco el nombre del form
	if( !this->objectName().isEmpty() )
	{
		buscar = this->objectName();
	}
	else
	{
		buscar = "noencontrado";
	}
   }
   else
   {
 	if( this->focusWidget()->parentWidget()->objectName().isEmpty() )
	{
		buscar = this->focusWidget()->objectName();
	}
	else
	{
		buscar = this->focusWidget()->parentWidget()->objectName() + "::" + this->focusWidget()->objectName();
	}
   }
   if( ayuda->hayAyuda( buscar ) )
   {
	ayuda->mostrarAyuda( buscar );
	event->accept();
   }
   else
   {
    event->ignore();
   }
 }
 else
 {
	QWidget::keyPressEvent( event );
 }
}


/*!
    \fn EVentana::closeEvent( QCloseEvent *e )
 */
void EVentana::closeEvent( QCloseEvent *e )
{
 QWidget::closeEvent( e );
}
