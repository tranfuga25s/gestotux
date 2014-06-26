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
#include "remitosplugins.h"

#include "formagregarremito.h"
#include "vremito.h"
#include "formprefremito.h"

#include <QSqlDatabase>
#include <QDebug>

Q_EXPORT_PLUGIN2( remitos, RemitoPlugin )

/*!
    \fn RemitoPlugin::accionesBarra()
 */
QList<QActionGroup *> RemitoPlugin::accionesBarra()
{
 QList<QActionGroup *> lista;
 return lista;
}


/*!
    \fn RemitoPlugin::nombre() const
 */
QString RemitoPlugin::nombre() const
{
  return "remitos";
}

/*!
    \fn RemitoPlugin::formsPreferencias()
 */
QWidgetList RemitoPlugin::formsPreferencias()
{
    QWidgetList lista;
    lista.append( new FormPrefRemito() );
    return lista;
}


/*!
    \fn RemitoPlugin::inicializar()
 */
bool RemitoPlugin::inicializar()
{
 Q_INIT_RESOURCE(remitos);

 ActAgregarRemito = new QAction( "Agregar remito", this );
 ActAgregarRemito->setIcon( QIcon( ":/imagenes/remito-nuevo.png" ) );
 ActAgregarRemito->setStatusTip( "Muestra el listado de remitos emitidos" );
 connect( ActAgregarRemito, SIGNAL( triggered() ), this, SLOT( agregarNuevoRemito() ) );

 ActVerRemitos = new QAction( "Listado de remitos", this );
 ActVerRemitos->setIcon( QIcon( ":/imagenes/remito.png" ) );
 ActVerRemitos->setStatusTip( "Permite ver las facturas emitidas y anularlas." );
 connect( ActVerRemitos, SIGNAL( triggered() ), this, SLOT( verRemitos() ) );

 return true;
}


/*!
    \fn RemitoPlugin::verificarTablas( QStringList tablas )
 */
bool RemitoPlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "producto" ) )
 { qDebug( "RemitoPlugin::Error al buscar la tabla producto" ); return false; }
 else if( !tablas.contains( "categoria_producto" ) )
 { qDebug( "RemitoPlugin::Error al buscar la tabla categorias_producto" ); return false; }
 if( !tablas.contains( "remito" ) )
 { qDebug( "RemitoPlugin::Error al buscar la tabla remito" ); return false; }
 if( !tablas.contains( "item_remito" ) )
 { qDebug( "RemitoPlugin::Error al buscar la tabla item_remito" ); return false; }
 return true;
}


/*!
    \fn RemitoPlugin::tipo() const
 */
int RemitoPlugin::tipo() const
{
 return EPlugin::comun;
}


/*!
    \fn RemitoPlugin::crearMenu( QMenuBar *m )
 */
void RemitoPlugin::crearMenu( QMenuBar *m )
{
 QMenu *mVentas = m->addMenu( "Remitos" );
 mVentas->addAction( ActAgregarRemito );
 mVentas->addAction( ActVerRemitos );
}


/*!
    \fn RemitoPlugin::version() const
 */
double RemitoPlugin::version() const
{
 return 0.02;
}

void RemitoPlugin::crearToolBar( QToolBar */*t*/ )
{}


/*!
    \fn RemitoPlugin::agregarNuevoRemito()
 */
void RemitoPlugin::agregarNuevoRemito()
{
    FormAgregarRemito *f = new FormAgregarRemito();
    connect( f, SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ), this, SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ) );
    connect( this, SIGNAL( planCuotaSetearIdCuota( int ) ), f, SLOT( setearIdPlanCuota( int ) ) );
    connect( f, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ), this, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ) );
    connect( f, SIGNAL( emitirGarantia( int, int, QString, int ) ), this, SIGNAL( emitirGarantia( int, int, QString, int ) ) );
    emit agregarVentana( f );
}


/*!
 * \fn RemitoPlugin::verRemitos()
 * Muestra el listado de remitos y permite cancelar o anular las que sean necesarias
 */
void RemitoPlugin::verRemitos()
{ emit agregarVentana( new VRemito() ); }

/*!
    \fn RemitoPlugin::seCierraGestotux()
 */
void RemitoPlugin::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(remitos);
 return;
}

/*!
 * \fn RemitoPlugin::agregarRemito(int, QDate, MProductosTotales *)
 */
void RemitoPlugin::agregarRemito( int id_cliente, QDate fecha, MProductosTotales * mpt )
{
    FormAgregarRemito *f = new FormAgregarRemito();
    f->setearCliente( id_cliente );
    f->setearFecha( fecha );
    f->setearItems( mpt );
    connect( f, SIGNAL( emitirPlanCuota( int, double ) ), this, SIGNAL( emitirPlanCuota( int, double ) ) );
    connect( this, SIGNAL( planCuotaSetearIdCuota( int ) ), f, SLOT( setearIdPlanCuota( int ) ) );
    connect( f, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ), this, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ) );
    connect( f, SIGNAL( emitirGarantia( int, int, QString, int ) ), this, SIGNAL( emitirGarantia( int, int, QString, int ) ) );
    emit agregarVentana( f );
}

#include "formremito.h"
/*!
 * \brief RemitoPlugin::mostrarRemito
 * \param id_remito Numero de remito a buscar
 */
void RemitoPlugin::mostrarRemito( int id_remito )
{
    FormRemito *f = new FormRemito();
    f->setearValor( id_remito );
    emit agregarVentana( f );
}
