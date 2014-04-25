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
#include "listadocaravanas.h"

#include "irenderizador.h"
#include "evisorinformes.h"
#include "einformeimpresora.h"

Q_EXPORT_PLUGIN2( listadocaravanas, ListadoCaravanas );

/*!
    \fn ListadoCaravanas::inicializar()
	Inicializa las acciones del plugin
	@return inicializacion correcta o incorrecta
 */
bool ListadoCaravanas::inicializar()
{
 ActInformeCaravanas = new QAction( this );
 ActInformeCaravanas->setText( "Caravanas x establecimiento" );
 connect( ActInformeCaravanas, SIGNAL( triggered() ), this, SLOT( informeCaravanas() ) );
 return true;
}


/*!
    \fn ListadoCaravanas::nombre() const
	Devuelve el nombre interno que se utiliza para referenciar al plugin
	@return Nombre interno del plugin
 */
QString ListadoCaravanas::nombre() const
{
 return "listadocaravanas";
}


/*!
    \fn ListadoCaravanas::version() const
	Devuelve el numero de version del plugin
	@return version del plugin
 */
double ListadoCaravanas::version() const
{
 return 0.1;
}


/*!
    \fn ListadoCaravanas::crearMenu( QMenu *menu )
	Metodo para agregar las acciones del resumen implementado al menu
	@param menu Puntero al menu
 */
void ListadoCaravanas::crearMenu( QMenu *menu )
{
  menu->addAction( ActInformeCaravanas );
}


#include "mestablecimiento.h"
#include <QInputDialog>
/*!
    \fn ListadoCaravanas::informeCaravanas()
	Slot que realiza el informe de las caravanas que se encuentran actualmente en un establecimiento
 */
void ListadoCaravanas::informeCaravanas()
{
 // Pongo el dialogo de todos los establecimientos
 bool ok;
 MEstablecimiento *modelo = new MEstablecimiento();
 modelo->select();
 QString item = QInputDialog::getItem( 0, "Elija el Establecimiento", "Establecimiento:", modelo->listaNombres(), 0, false, &ok, 0 );
 delete modelo;
 if( ok && !item.isEmpty() )
 {
  EVisorInformes *visor = new EVisorInformes( new EInformeImpresora() );
  // Lo heredo del visor para que se destruya cuando se cierre el informe
  IRenderizador *render = new IRenderizador( visor );
  render->setEstablecimiento( item );
  render->hacerInforme();
  connect( visor, SIGNAL(paintRequested( QPrinter* )), render, SLOT(imprimir(QPrinter*)));
  agregarVentana( visor );
 }
}


/*!
    \fn ListadoCaravanas::accionesBarra()
 */
QList<QAction *>  ListadoCaravanas::accionesBarra()
{
 QList<QAction *> lista;
 lista.append( ActInformeCaravanas );
 return lista;
}
