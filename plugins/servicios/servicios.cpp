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
#include "servicios.h"
#include <QSqlDatabase>

#include "vservicios.h"
#include "formrecargos.h"

bool Servicios::inicializar()
{
 Q_INIT_RESOURCE(servicios);

 //Acciones
 ActServicios = new QAction( this );
 ActServicios->setText( "Servicios" );
 ActServicios->setStatusTip( "Muestra todos los servicios disponibles que hay" );
 ActServicios->setIcon( QIcon( ":/imagenes/servicios.png" ) );
 connect( ActServicios, SIGNAL( triggered() ), this, SLOT( mostrarServicios() ) );

 ActRecargos = new QAction( this );
 ActRecargos->setText( "Recargos" );
 ActRecargos->setStatusTip( "Muestra la lista de recargos posibles que se pueden utilizar en cualquier servicio" );
 connect( ActRecargos, SIGNAL( triggered() ), this, SLOT( mostrarRecargos() ) );

 ActFacturarServicios = new QAction( this );
 ActFacturarServicios->setText( "Facturar Servicios" );
 ActFacturarServicios->setStatusTip( "Factura todos los servicios que no han sido dados de baja" );
 ActFacturarServicios->setIcon( QIcon( ":/imagenes/facturar_servicio.png" ) );
 connect( ActFacturarServicios, SIGNAL( triggered() ), this, SLOT( facturarServicios() ) );

 ActVerfRecargos = new QAction( this );
 ActVerfRecargos->setText( "Verificar recargos" );
 connect( ActVerfRecargos, SIGNAL( triggered() ), this, SLOT( verificarRecargos() ) );

 return true;
}

bool Servicios::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "servicios" ) )
 { qDebug( "Servicios::Error al buscar la tabla servicios" ); return false; }
 if( !tablas.contains( "servicios_clientes" ) )
 { qDebug( "Servicios::Error al buscar la tabla servicios_clientes" ); return false; }
 if( !tablas.contains( "recargos" ) )
 { qDebug( "Servicios::Error al buscar la tabla recargos" ); return false; }
 if( !tablas.contains( "periodo_servicio" ) )
 { qDebug( "Servicios::Error al buscar la tabla periodo_servicio" ); return false; }
 if( !tablas.contains( "cobro_servicio_cliente_periodo" ) )
 { qDebug( "Servicios::Error al buscar la tabla cobro_servicio_cliente_periodo" ); return false; }
 if( !tablas.contains( "recargo_cobro_servicio_cliente" ) )
 { qDebug( "Servicios::Error al buscar la tabla recargo_cobro_servicio_cliente" ); return false; }
 if( !tablas.contains( "v_servicios_clientes" ) )
 { qDebug( "Servicios::Error al buscar la vista v_servicios_clientes" ); return false; }
 return true;
}

double Servicios::version() const
{
 return 0.1;
}

int Servicios::tipo() const
{
 return EPlugin::comun;
}

QList< QActionGroup * > Servicios::accionesBarra()
{
 return QList<QActionGroup *>();
}

QString Servicios::nombre() const
{
 return "servicios";
}

QWidgetList Servicios::formsPreferencias()
{
 return QWidgetList();
}

void Servicios::crearMenu(QMenuBar* m)
{
 QMenu *serv = m->addMenu( "Servicios" );
 serv->addAction( ActServicios );
 serv->addAction( ActRecargos );
 serv->addAction( ActVerfRecargos );
 serv->addAction( ActFacturarServicios );
 return;
}

void Servicios::crearToolBar(QToolBar* /*t*/)
{
}

void Servicios::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(servicios);
}

/*!
    \fn Servicios::mostrarServicios
 */
void Servicios::mostrarServicios()
{
  emit agregarVentana( new VServicios() );
}

/*!
    @fn Servicios::mostrarRecargos()
    Muestra el listado de recargos
 */
void Servicios::mostrarRecargos()
{
   emit agregarVentana( new FormRecargos() );
}

#include "FormFacturarServicio.h"
#include "mservicios.h"
/*!
 * \fn void Servicios::facturarServicios()
 * Abre la ventana para realizar la facturacion de los servicios
 */
void Servicios::facturarServicios()
{
    // Selecciono un servicio de la lista
    QList<int> lista_ids = MServicios::listaDeServicios();
    foreach( int id_servicio, lista_ids ) {
        emit agregarVentana( new FormFacturarServicio( id_servicio ) );
    }
    return;
}

QAction *Servicios::botonPantallaInicial()
{ return ActServicios; }

#include "FormVerificarRecargos.h"
void Servicios::verificarRecargos()
{  emit agregarVentana( new FormVerificarRecargos() ); }

Q_EXPORT_PLUGIN2( servicios, Servicios )
