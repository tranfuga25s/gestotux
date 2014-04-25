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
#include "ventas.h"

#include <QSqlDatabase>

Q_EXPORT_PLUGIN2( ventas, Ventas )

/*!
    \fn Ventas::accionesBarra()
 */
QList<QActionGroup *> Ventas::accionesBarra()
{
 QList<QActionGroup *> lista;
 /////////////////////////////////////////////////////////////
 // Ventas
 QActionGroup *ventas = new QActionGroup( this );
 ventas->setObjectName( "ventas" );
 ventas->setProperty( "icono", ":/imagenes/ventas.jpg" );
 ventas->setProperty( "titulo", "Ventas" );
 ventas->addAction( ActAgregarVentas );
 lista.append( ventas );
 return lista;
}


/*!
    \fn Ventas::nombre() const
 */
QString Ventas::nombre() const
{
  return "ventas";
}

#include "formprefventa.h"
/*!
    \fn Ventas::formsPreferencias()
 */
QWidgetList Ventas::formsPreferencias()
{
    QWidgetList lista;
    lista.append( new FormPrefVenta() );
    return lista;
}


/*!
    \fn Ventas::inicializar()
 */
bool Ventas::inicializar()
{
 Q_INIT_RESOURCE(ventas);
 _acciones.clear();

 ActAgregarVentas = new QAction( "Agregar venta", this );
 ActAgregarVentas->setIcon( QIcon( ":/imagenes/factura-nueva.png" ) );
 connect( ActAgregarVentas, SIGNAL( triggered() ), this, SLOT( agregarVenta() ) );

 ActVerFacturas = new QAction( "Ver Facturas", this );
 ActVerFacturas->setIcon( QIcon( ":/imagenes/factura.png" ) );
 ActVerFacturas->setStatusTip( "Permite ver las facturas emitidas y anularlas." );
 connect( ActVerFacturas, SIGNAL( triggered() ), this, SLOT( verFacturas() ) );

 return true;
}


/*!
    \fn Ventas::verificarTablas( QStringList tablas )
 */
bool Ventas::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "producto" ) )
 { qWarning( "Ventas::Error al buscar la tabla producto" ); return false; }
 else if( !tablas.contains( "categoria_producto" ) )
 { qWarning( "Ventas::Error al buscar la tabla categorias_producto" ); return false; }
 if( !tablas.contains( "factura" ) )
 { qWarning( "Ventas::Error al buscar la tabla factura" ); return false; }
 if( !tablas.contains( "item_factura" ) )
 { qWarning( "Ventas::Error al buscar la tabla item_factura" ); return false; }
 return true;
}


/*!
    \fn Ventas::tipo() const
 */
int Ventas::tipo() const
{
 return EPlugin::comun;
}


/*!
    \fn Ventas::crearMenu( QMenuBar *m )
 */
void Ventas::crearMenu( QMenuBar *m )
{
 QMenu *mVentas = m->addMenu( "Ventas" );
 mVentas->addAction( ActAgregarVentas );
 mVentas->addAction( ActVerFacturas );
 mVentas->addSeparator();
}


/*!
    \fn Ventas::version() const
 */
double Ventas::version() const
{
 return 0.12;
}

void Ventas::crearToolBar( QToolBar */*t*/ )
{}


#include "formagregarventa.h"
/*!
    \fn Ventas::agregarVenta()
 */
void Ventas::agregarVenta()
{
    FormAgregarVenta *f = new FormAgregarVenta();
    connect( f, SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ), this, SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ) );
    connect( this, SIGNAL( planCuotaSetearIdCuota( int ) ), f, SLOT( setearIdPlanCuota( int ) ) );
    connect( f, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ), this, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ) );
        connect( f, SIGNAL( emitirGarantia( int, int, QString, int ) ), this, SIGNAL( emitirGarantia( int, int, QString, int ) ) );
    emit agregarVentana( f );
}

void Ventas::mostrarFactura( int /*id_factura */ )
{
    qWarning( "Todavía no está implementada la vision de una factura emitida" );
}

#include "vventas.h"
/*!
 * \fn Ventas::verFacturas()
 * Muestra el listado de facturas y permite cancelar o anular las que sean necesarias
 */
void Ventas::verFacturas()
{
    VVentas *f = new VVentas();
    connect( f, SIGNAL( emitirRecibo( int, QString, double ) ), this, SIGNAL( emitirRecibo( int, QString, double ) ) );
    emit agregarVentana( f );
}

/*!
    \fn Ventas::seCierraGestotux()
 */
void Ventas::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(ventas);
 return;
}

/*!
 * \fn Ventas::agregarFactura( int, QDate, MProductosTotales * )
 */
void Ventas::agregarFactura( int id_cliente, QDate fecha, MProductosTotales * mpt )
{
    FormAgregarVenta *f = new FormAgregarVenta();
    f->setearCliente( id_cliente );
    f->setearFecha( fecha );
    f->setearItems( mpt );
    connect( f, SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ), this, SIGNAL( emitirPlanCuota( int, double, MPlanCuota::TipoComprobante ) ) );
    connect( this, SIGNAL( planCuotaSetearIdCuota( int ) ), f, SLOT( setearIdPlanCuota( int ) ) );
    connect( f, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ), this, SIGNAL( emitirPlanCuotaSetIdFactura( int, int ) ) );
    connect( f, SIGNAL( emitirGarantia( int, int, QString, int ) ), this, SIGNAL( emitirGarantia( int, int, QString, int ) ) );
    emit agregarVentana( f );
}
