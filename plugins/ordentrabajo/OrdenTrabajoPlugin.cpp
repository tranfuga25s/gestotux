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
#include "OrdenTrabajoPlugin.h"

#include <QSqlDatabase>

/*!
 * \brief OrdenTrabajoPlugin::inicializar
 * \return
 */
bool OrdenTrabajoPlugin::inicializar()
{
 Q_INIT_RESOURCE(ordentrabajo);
 ///////////////////////////////
 // Muestra las ordenes de trabajo
 //////////////////////////////
 ActOrdenTrabajo = new QAction( "Ordenes de trabajo", this );
 ActOrdenTrabajo->setStatusTip( "Muestra la lista de ordenes de trabajo" );
 ActOrdenTrabajo->setIcon( QIcon( ":/imagenes/orden_trabajo.png" ) );
 connect( ActOrdenTrabajo, SIGNAL( triggered() ), this, SLOT( verOrdenesTrabajo() ) );
 ///////////////////////////////////
 // Agrega una nueva orden de trabajo
 ///////////////////////////////////
 ActAgregarOrdenTrabajo = new QAction( "Agregar orden de trabajo", this );
 ActAgregarOrdenTrabajo->setStatusTip( "Agrega una nueva orden de trabajo" );
 ActAgregarOrdenTrabajo->setIcon( QIcon( ":/imagenes/orden_trabajo.png" ) );
 connect( ActAgregarOrdenTrabajo, SIGNAL( triggered() ), this, SLOT( agregarOrdenTrabajo() ) );
 ///////////////////////////////////
 // Muestra el listado de tecnicos
 ///////////////////////////////////
 ActVerTecnicos = new QAction( "Tecnicos", this );
 ActVerTecnicos->setStatusTip( "Muestra el listado de tecnicos habilitados" );
 ActVerTecnicos->setIcon( QIcon( ":/imagenes/tecnico.png" ) );
 connect( ActVerTecnicos, SIGNAL( triggered() ), this, SLOT( verTecnicos() ) );
 ///////////////////////////////////
 // Muestra los tipos de operaciones
 ///////////////////////////////////
 ActVerTipoOperacion = new QAction( "Tipo operaciones", this );
 ActVerTipoOperacion->setStatusTip( "Muestra el listado de operaciones disponibles" );
 //ActVerTipoOperacion->setIcon( QIcon( ":/imagenes/proveedores.jpg" ) );
 connect( ActVerTipoOperacion, SIGNAL( triggered() ), this, SLOT( verTipoOperacion() ) );
 ///////////////////////////////////
 // Muestra el listado de equipamiento en el sistema
 ///////////////////////////////////
 ActVerEquipamiento = new QAction( "Equipamientos", this );
 ActVerEquipamiento->setStatusTip( "Muestra el listado de equipamientos dados de alta en el sistema" );
 ActVerEquipamiento->setIcon( QIcon( ":/imagenes/equipamiento.png" ) );
 connect( ActVerEquipamiento, SIGNAL( triggered() ), this, SLOT( verEquipamientos() ) );

 return true;
}

/*!
 * \brief OrdenTrabajoPlugin::verificarTablas
 * \param tablas
 * \return
 */
bool OrdenTrabajoPlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "orden_trabajo" ) )
 { qWarning( "OrdenTrabajoPlugin::Error al buscar la tabla orden_trabajo" ); return false; }
 if( !tablas.contains( "tecnico" ) )
 { qWarning( "OrdenTrabajoPlugin::Error al buscar la tabla tecnico" ); return false; }
 if( !tablas.contains( "equipamiento" ) )
 { qWarning( "OrdenTrabajoPlugin::Error al buscar la tabla equipamiento" ); return false; }
 if( !tablas.contains( "tipo_operacion_orden_trabajo" ) )
 { qWarning( "OrdenTrabajoPlugin::Error al buscar la tabla tipo_operacion_orden_trabajo" ); return false; }
 if( !tablas.contains( "operacion_orden_trabajo" ) )
 { qWarning( "OrdenTrabajoPlugin::Error al buscar la tabla operacion_orden_trabajo" ); return false; }
 if( !tablas.contains( "v_equipamiento" ) )
 { qWarning( "OrdenTrabajoPlugin::Error al buscar al vista de equipamientos" ); return false; }
 return true;
}

/*!
 * \brief OrdenTrabajoPlugin::version
 * \return
 */
double OrdenTrabajoPlugin::version() const
{ return 0.1; }

/*!
 * \brief OrdenTrabajoPlugin::tipo
 * \return
 */
int OrdenTrabajoPlugin::tipo() const
{ return EPlugin::comun; }

/*!
 * \brief OrdenTrabajoPlugin::accionesBarra
 * \return
 */
QList< QActionGroup * > OrdenTrabajoPlugin::accionesBarra()
{
 /*QList<QActionGroup *> lista;
 QActionGroup *compras = new QActionGroup( this );
 compras->setObjectName( "compras" );
 compras->addAction( ActProveedores );
 lista.append( compras );
 return lista;*/
 return QList<QActionGroup *>();
}

/*!
 * \brief OrdenTrabajoPlugin::nombre
 * \return
 */
QString OrdenTrabajoPlugin::nombre() const
{ return "ordentrabajo"; }

#include "formprefordentrabajo.h"
/*!
 * \brief OrdenTrabajoPlugin::formsPreferencias
 * \return
 */
QWidgetList OrdenTrabajoPlugin::formsPreferencias()
{
    QWidgetList lista;
    lista.append( new FormPrefOrdenTrabajo() );
    return lista;
}

/*!
 * \brief OrdenTrabajoPlugin::crearMenu
 * \param m
 */
void OrdenTrabajoPlugin::crearMenu( QMenuBar* m )
{
 QMenu *menuHerramientas = m->findChild<QMenu *>( "menuHerramientas" );
 if( menuHerramientas != 0 ) {
     QMenu *menuOrden = menuHerramientas->addMenu( "Orden de trabajo" );
     menuOrden->addAction( ActAgregarOrdenTrabajo );
     menuOrden->addAction( ActOrdenTrabajo );
     menuOrden->addAction( ActVerTecnicos );
     menuOrden->addAction( ActVerTipoOperacion );
     menuOrden->addAction( ActVerEquipamiento );
 }
}

/*!
 * \brief OrdenTrabajoPlugin::crearToolBar
 */
void OrdenTrabajoPlugin::crearToolBar(QToolBar* /*t*/)
{}

/*!
 * \brief OrdenTrabajoPlugin::botonPantallaInicial
 * \return
 */
QAction *OrdenTrabajoPlugin::botonPantallaInicial()
{ return ActOrdenTrabajo; }

/*!
    \fn OrdenTrabajoPlugin::seCierraGestotux()
 */
void OrdenTrabajoPlugin::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(ordentrabajo); return; }

#include "vordentrabajo.h"
/*!
 * \brief OrdenTrabajoPlugin::verOrdenesTrabajo
 * Muestra la lista de ordenes de trabajo
 */
void OrdenTrabajoPlugin::verOrdenesTrabajo()
{
    emit agregarVentana( new VOrdenTrabajo() );
}

#include "vequipamiento.h"
/*!
 * \brief OrdenTrabajoPlugin::verEquipamientos
 * Slot llamado  para mostrar el equipamiento dado de alta.
 */
void OrdenTrabajoPlugin::verEquipamientos()
{ emit agregarVentana( new VEquipamiento() ); }

#include "vtipooperacionordentrabajo.h"
/*!
 * \brief OrdenTrabajoPlugin::verTipoOperacion
 * Slot llamado para mostrar la ventana de tipos de operaciones de orden de trabajo
 */
void OrdenTrabajoPlugin::verTipoOperacion()
{ emit agregarVentana( new VTipoOperacionOrdenTrabajo() ); }

#include "vtecnicos.h"
void OrdenTrabajoPlugin::verTecnicos()
{ emit agregarVentana( new VTecnicos() ); }

#include "ordentrabajowizard.h"
/*!
 * \brief OrdenTrabajoPlugin::agregarOrdenTrabajo
 * Agrega una nueva orden de trabajo
 */
void OrdenTrabajoPlugin::agregarOrdenTrabajo()
{
    OrdenTrabajoWizard *w = new OrdenTrabajoWizard();
    w->exec();
}

Q_EXPORT_PLUGIN2( ordentrabajo, OrdenTrabajoPlugin )
