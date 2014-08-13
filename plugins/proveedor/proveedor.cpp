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
#include "proveedor.h"

#include <QSqlDatabase>

bool proveedor::inicializar()
{
 Q_INIT_RESOURCE(proveedor);
 ///////////////////////////////
 // Muestra los proveedores
 //////////////////////////////
 ActProveedores = new QAction( "Proveedores", this );
 ActProveedores->setStatusTip( "Mustra los distintos proveedores" );
 ActProveedores->setIcon( QIcon( ":/imagenes/proveedores.jpg" ) );
 connect( ActProveedores, SIGNAL( triggered() ), this, SLOT( proveedores() ) );
 return true;
}

bool proveedor::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "proveedor" ) )
 { qDebug( "Proveedor::Error al buscar la tabla proveedor" ); return false; }
 return true;
}

double proveedor::version() const
{ return 0.1; }

int proveedor::tipo() const
{ return EPlugin::comun; }

QList< QActionGroup * > proveedor::accionesBarra()
{
 QList<QActionGroup *> lista;
 QActionGroup *compras = new QActionGroup( this );
 compras->setObjectName( "compras" );
 compras->addAction( ActProveedores );
 lista.append( compras );
 return lista;
}

QString proveedor::nombre() const
{ return "proveedor"; }

QWidgetList proveedor::formsPreferencias()
{ return QWidgetList(); }

void proveedor::crearMenu(QMenuBar* m)
{
 QMenu *menuCompras = m->findChild<QMenu *>( "menuCompras" );
 if( menuCompras != 0 ) {
  menuCompras->addAction( ActProveedores );
 }
 QMenu *menuHerramientas = m->findChild<QMenu *>( "menuHerramientas" );
 if( menuHerramientas != 0 ) {
  menuHerramientas->addAction( ActProveedores );
 }
}

void proveedor::crearToolBar(QToolBar* t)
{
    (void)t;
}

QAction *proveedor::botonPantallaInicial()
{  return ActProveedores; }

#include "vproveedor.h"

/*!
    \fn proveedor::proveedores()
 */
void proveedor::proveedores()
{ emit agregarVentana( new VProveedor() ); }

Q_EXPORT_PLUGIN2( proveedor, proveedor )


/*!
    \fn proveedor::seCierraGestotux()
 */
void proveedor::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(proveedor); return; }
