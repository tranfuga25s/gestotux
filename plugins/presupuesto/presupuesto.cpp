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
#include "presupuesto.h"

#include <QSettings>
#include <QStackedWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include "formagregarpresupuesto.h"

/*!
    \fn presupuesto::accionesBarra()
        Devuelve la lista de acciones que deben ir en cada una de las barras laterales en sus repectivos grupos
        @return QList<QActionGroup *> Lista
 */
QList<QActionGroup *> presupuesto::accionesBarra()
{
  QList<QActionGroup *> lista;
  QActionGroup *presupuestos = new QActionGroup( this );
  presupuestos->setObjectName( "presupuestos" );
  presupuestos->setProperty( "titulo", "Presupuestos" );
  presupuestos->setProperty( "icono",  ":/imagenes/presupuesto.png" );
  presupuestos->addAction( ActNuevoPresu );
  presupuestos->addAction( ActPresuAnteriores );
  lista.append( presupuestos );
  return lista;
}


/*!
    \fn presupuesto::nombre() const
 */
QString presupuesto::nombre() const
{
  return "presupuesto";
}


/*!
    \fn presupuesto::formsPreferencias()
 */
QWidgetList presupuesto::formsPreferencias()
{
  return QWidgetList();
}


/*!
    \fn presupuesto::inicializar()
 */
bool presupuesto::inicializar()
{
 Q_INIT_RESOURCE(presupuesto);
 // Genero las acciones y la lista
 ActNuevoPresu = new QAction( "Nuevo presupuesto", this );
 ActNuevoPresu->setIcon( QIcon( ":/imagenes/presupuesto-nuevo.png" ) );
 ActNuevoPresu->setStatusTip( "Genera un nuevo presupuesto" );
 connect( ActNuevoPresu, SIGNAL( triggered() ), this, SLOT( nuevoPresupuesto() ) );

 ActPresuAnteriores = new QAction( "Presupuestos Anteriores", this );
 ActPresuAnteriores->setIcon( QIcon( ":/imagenes/anteriores.png" ) );
 ActPresuAnteriores->setStatusTip( "Ver los presupuestos anteriores" );
 ActPresuAnteriores->setShortcut( QKeySequence( Qt::CTRL +Qt::Key_P ) );
 connect( ActPresuAnteriores, SIGNAL( triggered() ), this, SLOT( verAnteriores() ) );

 return true;
}


/*!
    \fn presupuesto::tipo() const
 */
int presupuesto::tipo() const
{
  return EPlugin::comun;
}


/*!
    \fn presupuesto::crearMenu( QMenuBar *m )
 */
void presupuesto::crearMenu( QMenuBar *m )
{
 QMenu *menuPresupuestos = m->addMenu( "Presupuestos" );
 menuPresupuestos->addAction( ActNuevoPresu );
 menuPresupuestos->addAction( ActPresuAnteriores );
}


/*!
    \fn presupuesto::version() const
 */
double presupuesto::version() const
{
  return 0.1;
}


/*!
    \fn presupuesto::verificarTablas( QStringList tablas )
    Verifica que existan las tablas necesarias para el plugin
 */
bool presupuesto::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "presupuestos" ) )
 { qDebug( "Presupuesto::Error al buscar la tabla presupuestos" ); return false; }
 if( !tablas.contains( "item_presupuesto" ) )
 { qDebug( "Presupuesto::Error al buscar la tabla item_presupuesto" ); return false; }
 return true;
}

/*!
    \fn presupuesto::nuevoPresupuesto()
 */
void presupuesto::nuevoPresupuesto()
{ emit agregarVentana( new FormAgregarPresupuesto() ); }


#include "vpresupuesto.h"
/*!
    \fn presupuesto::verAnteriores()
 */
void presupuesto::verAnteriores()
{
    VPresupuesto *v = new VPresupuesto();
    connect( v, SIGNAL( emitirFactura( int, QDate, MProductosTotales * ) ), this, SIGNAL( emitirFactura( int, QDate, MProductosTotales * ) ) );
    emit agregarVentana( v );
}


/*!
    \fn presupuesto::crearToolBar( QToolBar *t )
 */
void presupuesto::crearToolBar( QToolBar */*t*/ )
{}


/*!
    \fn presupuesto::seCierraGestotux()
 */
void presupuesto::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(presupuesto);
 return;
}

Q_EXPORT_PLUGIN2( presupuesto, presupuesto )
