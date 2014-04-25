/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
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

#include "caja.h"
#include <QMenuBar>
#include <QSqlDatabase>
#include "formprefcaja.h"

Q_EXPORT_PLUGIN2( Caja, Caja )

/*!
    \fn Caja::accionesBarra()
 */
QList<QActionGroup *> Caja::accionesBarra()
{ return QList<QActionGroup *>(); }


/*!
    \fn Caja::nombre() const
 */
QString Caja::nombre() const
{ return "caja"; }

/*!
    \fn Caja::formsPreferencias()
 */
QWidgetList Caja::formsPreferencias()
{
 QWidgetList lista;
 lista.append( new FormPrefCaja() );
 return lista;
}


/*!
    \fn Caja::inicializar()
 */
bool Caja::inicializar()
{
 Q_INIT_RESOURCE(caja);

 ActCajas = new QAction( this );
 ActCajas->setText( "Cajas" );
 ActCajas->setToolTip( "Ver el listado de las cajas que hay en el sistema." );
 ActCajas->setIcon( QIcon( ":/imagenes/caja.png" ) );
 connect( ActCajas, SIGNAL(triggered()), this, SLOT( verCajas() ) );

 ActVerEstado = new QAction( this );
 ActVerEstado->setText( "Ver Estado Caja" );
 ActVerEstado->setToolTip( "Ver el estado actual de la caja." );
 ActVerEstado->setIcon( QIcon( ":/imagenes/cajaestado.png" ) );
 connect( ActVerEstado, SIGNAL( triggered() ), this, SLOT( verEstadoCaja() ) );

 ActHacerCierre = new QAction( this );
 ActHacerCierre->setText( "Hacer cierre de caja" );
 ActHacerCierre->setToolTip( "Resume todas las operaciones del dia y cierra la caja." );
 ActHacerCierre->setIcon( QIcon( ":/imagenes/cierrecaja.png" ) );
 connect( ActHacerCierre, SIGNAL( triggered() ), this, SLOT( hacerCierre() ) );

 ActResumenes = new QAction( this );
 ActResumenes->setText( "Resumen" );
 ActResumenes->setToolTip( "Muestra el estado de la cajas hasta el momento." );
 ActResumenes->setIcon( QIcon( ":/imagenes/cajaresumen.png" ) );
 connect( ActResumenes, SIGNAL( triggered() ), this, SLOT( verResumenCaja() ) );

 return true;
}


/*!
    \fn Caja::verificarTablas( QStringList tablas )
    Verifica que existan las tablas del plugin de cajas
    \param tablas listado de tablas
    \return Falso si falta alguna.
 */
bool Caja::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "caja" ) )
 { qDebug( "Error al buscar la tabla caja" ); return false; }
 else if( !tablas.contains( "movimiento_caja" ) )
 { qDebug( "Error al buscar la tabla movimiento_caja" ); return false; }
 return true;
}

/*!
    \fn Caja::tipo() const
 */
int Caja::tipo() const
{ return EPlugin::comun; }


/*!
    \fn Caja::crearMenu( QMenuBar *m )
 */
void Caja::crearMenu( QMenuBar *m )
{
     QMenu *menuCaja = m->addMenu( /*QIcon( ":/imagenes/caja.png"),*/ "Caja" );
     menuCaja->addAction( ActVerEstado );
     menuCaja->addAction( ActHacerCierre );
     menuCaja->addAction( ActResumenes );
     menuCaja->addSeparator();
     menuCaja->addAction( ActCajas );
}


/*!
    \fn Caja::version() const
        Devuelve la version del plugin
 */
double Caja::version() const
{ return 0.02; }

/*!
    \fn Caja::crearToolBar( QToolBar *t )
 */
void Caja::crearToolBar( QToolBar *t )
{
    (void)t;
 return;
}

/*!
    \fn Caja::seCierraGestotux()
 */
void Caja::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(caja); return; }

#include "vcajas.h"
/*!
 * @fn Caja::verCajas
 * Slot llamado cuando se quiere ve la lista de cajas actuales.
 */
void Caja::verCajas()
{ emit agregarVentana( new VCajas() ); }

#include "VResumenCaja.h"
/*!
 * @fn Caja::verEstadoCaja()
 * Muestra el resumen de movimientos hasta el momento
 */
void Caja::verResumenCaja()
{ emit agregarVentana( new VResumenCaja()); }

#include "FormEstadoCaja.h"
/*!
 * @fn Caja::verEstadoCaja()
 * Slot llamado cuando se quiere mostrar la lista de cajas y sus estados
 */
void Caja::verEstadoCaja()
{
  // Dialogo mostrando el saldo que debe existir en cada caja
  FormEstadoCaja *f = new FormEstadoCaja();
  f->exec();
}

#include "FormCierreCaja.h"
void Caja::hacerCierre()
{
    // Genero el dialogo donde pongo el saldo actual y pongo el widget para hacer las sumas de lo que hay en la caja
    emit agregarVentana( new FormCierreCaja() );
}
