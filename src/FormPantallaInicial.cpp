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

#include "FormPantallaInicial.h"
#include "EFlecha.h"
#include "eregistroplugins.h"
#include "ebackup.h"
#include "vcliente.h"
#include "formpreferencias.h"

#include <QPaintEngine>

FormPantallaInicial::FormPantallaInicial(QWidget *parent) :
    EVentana(parent)
{
    setupUi(this);
    this->setObjectName( "inicio" );
    this->setWindowTitle( "Inicio" );
    this->setWindowIcon( QIcon( ":/imagenes/inicio.png" ) );

    // Seteo los iconos
    // Generales
    TBBackup->setIcon( QIcon( ":/imagenes/backup.png" ) );
    TBBackup->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_B ) );
    TBBackup->setStatusTip( "Muestra la ventana de backup( Shift + b )" );
    connect( TBBackup, SIGNAL( clicked() ), this, SLOT( backup() ) );

    TBPreferencias->setIcon( QIcon( ":/imagenes/configure.png" ) );
    TBPreferencias->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_O ) );
    TBPreferencias->setStatusTip( "Muestra las preferencias del programa ( Shift + o ) " );
    connect( TBPreferencias, SIGNAL( clicked() ), this, SLOT( preferencias() ) );

    TBClientes->setIcon( QIcon( ":/imagenes/clientes.png" ) );
    //TBClientes->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_C ) );
    TBClientes->setStatusTip( "Muestra el listado de clientes ( Shift + c )" );
    connect( TBClientes, SIGNAL( clicked() ), this, SLOT( clientes() ) );

    this->TBNotas->setVisible( false );

    /////////////////////////////////////////////////////////////////////////////////////////////
    // ERegistroPlugins::getInstancia
    ////////////////////////////////////////////////////////////////////////////////////////////
    //  Presupuestos
    if( ERegistroPlugins::getInstancia()->existePlugin( "presupuesto" ) ) {
        connect( TBPresupuestos, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("presupuesto")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBPresupuestos->setIcon(ERegistroPlugins::getInstancia()->plugin("presupuesto")->botonPantallaInicial()->icon() );
        TBPresupuestos->setShortcut( Qt::SHIFT + Qt::Key_P );
        TBPresupuestos->setStatusTip( "Muestra la ventana para emitir un presupeusto nuevo ( Shift + P )" );
    } else {
        TBPresupuestos->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Caja
    if( ERegistroPlugins::getInstancia()->existePlugin( "caja" ) ) {
        connect( TBCaja, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("caja")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBCaja->setIcon(ERegistroPlugins::getInstancia()->plugin("caja")->botonPantallaInicial()->icon() );
        TBCaja->setStatusTip( "Muestra el estado de las cajas del sistema ( Shift + e )" );
        TBCaja->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_E ) );
    } else {
        TBCaja->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Compras
    if( ERegistroPlugins::getInstancia()->existePlugin( "compras" ) ) {
        connect( TBCompras, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("compras")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBCompras->setIcon(ERegistroPlugins::getInstancia()->plugin("compras")->botonPantallaInicial()->icon() );
        TBCompras->setStatusTip( "Muestra el listado de compras ( Shift + M )" );
        TBCompras->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_M ) );
    } else {
        TBCompras->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Productos
    if( ERegistroPlugins::getInstancia()->existePlugin( "productos" ) ) {
        connect( TBProductos, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("productos")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBProductos->setIcon( ERegistroPlugins::getInstancia()->plugin("productos")->botonPantallaInicial()->icon() );
        TBProductos->setShortcut( Qt::SHIFT + Qt::Key_I );
        TBProductos->setStatusTip( "Muestra el listado de productos ( Shift + i )" );
    } else {
        TBProductos->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Cuentas Corrientes
    if( ERegistroPlugins::getInstancia()->existePlugin( "ctacte" ) ) {
        if( ERegistroPlugins::getInstancia()->plugin("ctacte")->botonPantallaInicial() )
        { connect( TBCuentasCorrientes, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("ctacte")->botonPantallaInicial(), SIGNAL(triggered()) );
          TBCuentasCorrientes->setIcon( ERegistroPlugins::getInstancia()->plugin("ctacte")->botonPantallaInicial() ->icon());
          TBCuentasCorrientes->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_X ) );
          TBCuentasCorrientes->setStatusTip( "Muestra el listado de cuentas corrientes ( Shift + X ) " );
        } else { qWarning( "Error de accion de cuenta corriente") ; }
    } else {
        TBCuentasCorrientes->setVisible( false );
    }

    //////////////////////////////////////////////////////////////////////////////////
    // Proveedores
    if( ERegistroPlugins::getInstancia()->existePlugin( "proveedor" ) ) {
        TBProveedores->setIcon( ERegistroPlugins::getInstancia()->plugin("proveedor")->botonPantallaInicial()->icon() );
        connect( TBProveedores, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("proveedor")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBProveedores->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_L ) );
        TBProveedores->setStatusTip( "Muestra el listado de proveedores del sistema ( Shift + L ) " );
    } else {
        TBProveedores->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Servicios
    if( ERegistroPlugins::getInstancia()->existePlugin( "servicios" ) ) {
        TBServicios->setIcon( ERegistroPlugins::getInstancia()->plugin("servicios")->botonPantallaInicial()->icon() );
        connect( TBServicios, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("servicios")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBServicios->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_S ) );
        TBServicios->setStatusTip( "Muestra el listado de servicios actuales ( Shift + s )" );
    } else {
        TBServicios->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Gastos
    if( ERegistroPlugins::getInstancia()->existePlugin( "gastos" ) ) {
        TBGastos->setIcon( ERegistroPlugins::getInstancia()->plugin("gastos")->botonPantallaInicial()->icon() );
        connect( TBGastos, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("gastos")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBGastos->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_G ) );
        TBGastos->setStatusTip( "Muestra el listado de gastos ( Shift + g )" );
    } else {
        TBGastos->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Pagos / Recibos
    if( ERegistroPlugins::getInstancia()->existePlugin( "pagos" ) ) {
        TBRecibos->setIcon( ERegistroPlugins::getInstancia()->plugin("pagos")->botonPantallaInicial()->icon() );
        connect( TBRecibos, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("pagos")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBRecibos->setStatusTip( "Muestra la ventana para emitir un nuevo recibo ( Shift + R )" );
    } else {
        TBRecibos->setVisible( false );
    }
    //////////////////////////////////////////////////////////////////////////////////
    // Ventas / Facturas
    if( ERegistroPlugins::getInstancia()->existePlugin( "ventas" ) ) {
        TBFacturas->setIcon( ERegistroPlugins::getInstancia()->plugin("ventas")->botonPantallaInicial()->icon() );
        connect( TBFacturas, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("ventas")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBFacturas->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_V ) );
        TBFacturas->setStatusTip( "Muestra la ventana para emitir una factura ( Shift + v )" );
    } else {
        TBFacturas->setVisible( false );
    }
    ///////////////////////////////////////////////////////////////////////////////////
    // Plugin de remitos
    if( ERegistroPlugins::getInstancia()->existePlugin( "remitos" ) ) {
        TBRemitos->setIcon( ERegistroPlugins::getInstancia()->plugin("remitos")->botonPantallaInicial()->icon() );
        connect( TBRemitos, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("remitos")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBRemitos->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_T ) );
        TBRemitos->setStatusTip( "Genera un nuevo remito" );
    } else {
        TBRemitos->setVisible( false );
    }
    ///////////////////////////////////////////////////////////////////////////////////
    // Plugin de ordenes de trabajo
    if( ERegistroPlugins::getInstancia()->existePlugin( "ordentrabajo" ) ) {
        TBOrdenTrabajo->setIcon( ERegistroPlugins::getInstancia()->plugin("ordentrabajo")->botonPantallaInicial()->icon() );
        connect(  TBOrdenTrabajo, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("ordentrabajo")->botonPantallaInicial(), SIGNAL(triggered()) );
        //TBOrdenTrabajo->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_T ) );
        TBOrdenTrabajo->setStatusTip( "Muestra la lista de ordenes de trabajo" );
    } else {
        TBOrdenTrabajo->setVisible( false );
    }
    ////////////////////////////////////////////////////////////////////////////////////
    // Plugin de Planes de Cuotas
    if( ERegistroPlugins::getInstancia()->existePlugin( "cuotas" ) ) {
        TBCuotas->setIcon( ERegistroPlugins::getInstancia()->plugin("cuotas")->botonPantallaInicial()->icon() );
        connect(  TBCuotas, SIGNAL( clicked() ), ERegistroPlugins::getInstancia()->plugin("cuotas")->botonPantallaInicial(), SIGNAL(triggered()) );
        TBCuotas->setStatusTip( "Muestra la lista de planes de cuota activos" );
    } else {
        TBCuotas->setVisible( false );
    }

    // Dibujo las flechas
   /* EFlecha *f1 = new EFlecha( this );
    f1->setearOrigen( TBPresupuestos );
    f1->setearDestino( TBFacturas );
    f1->setearTamFlecha( 5 );

    EFlecha *f2 = new EFlecha( this );
    f2->setearOrigen( TBFacturas );
    f2->setearDestino( TBRecibos );
    f2->setearTamFlecha( 5 );

    EFlecha *f3 = new EFlecha( this );
    f3->setearOrigen( TBFacturas );
    f3->setearDestino( TBCaja );
    f3->setearTamFlecha( 5 ); */
}

void FormPantallaInicial::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void FormPantallaInicial::backup()
{ emit agregarVentana( new Ebackup( this ) ); }

void FormPantallaInicial::preferencias()
{ emit agregarVentana( new FormPreferencias( ) ); }

void FormPantallaInicial::clientes()
{ emit agregarVentana( new VCliente()); }
