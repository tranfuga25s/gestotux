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
#include <QAction>
#include <QSqlDatabase>

#include "pagosplugin.h"
#include "formagregarrecibo.h"
#include "formrecibo.h"
#include "vpagos.h"

bool PagosPlugin::inicializar()
{
 Q_INIT_RESOURCE(pagos);
 ActPagos = new QAction( "Lista de pagos", this );
 ActPagos->setStatusTip( "Visualiza todos los pagos echos recientemente" );
 ActPagos->setIcon( QIcon( ":/imagenes/recibo.png" ) );
 //ActPagos->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_R ) );
 connect( ActPagos, SIGNAL( triggered() ), this, SLOT( verPagos() ) );

 ActAgregarRecibo = new QAction( "Agregar Recibo", this );
 ActAgregarRecibo->setIcon( QIcon( ":/imagenes/recibo-nuevo.png" ) );
 ActAgregarRecibo->setStatusTip( "Ingresa un nuevo registro de recibo al sistema y lo imprime automaticamente" );
 connect( ActAgregarRecibo, SIGNAL( triggered() ), this, SLOT( agregarRecibo() ) );

 return true;
}

bool PagosPlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "recibos" ) )
 { qDebug( "Recibos::Error al buscar la tabla recibos" ); return false; }
 return true;
}

double PagosPlugin::version() const
{ return 0.1; }

int PagosPlugin::tipo() const
{ return EPlugin::comun; }

QList< QActionGroup * > PagosPlugin::accionesBarra()
{ return QList<QActionGroup*>(); }

QString PagosPlugin::nombre() const
{ return "pagos"; }

QWidgetList PagosPlugin::formsPreferencias()
{
 return QWidgetList();
}

void PagosPlugin::crearMenu( QMenuBar* m )
{
    QMenu *mPagos = m->addMenu( "Pagos" );
    mPagos->addAction( ActPagos );
    mPagos->addAction( ActAgregarRecibo );
}

void PagosPlugin::crearToolBar(QToolBar*)
{}

void PagosPlugin::seCierraGestotux()
{
    Q_CLEANUP_RESOURCE(pagos);
}


/*!
    \fn PagosPlugin::verPagos()
 */
void PagosPlugin::verPagos()
{
 emit agregarVentana( new VPagos() );
}


/*!
    \fn PagosPlugin::agregarRecibo()
*/
void PagosPlugin::agregarRecibo()
{
    FormAgregarRecibo *f = new FormAgregarRecibo();
    f->setearModelo(0);
    emit agregarVentana( f );
}

/*!
    \fn PagosPlugin::agregarRecibo()
    Slot llamado desde otro plugin cuando se desea agregar un nuevo recibo con datos predefinidos
    \param id_cliente Identificador del cliente
    \param texto Texto que contendrá el recibo
    \param total Importe del recibo
*/
void PagosPlugin::agregarRecibo( int id_cliente, QString texto, double total )
{
    FormAgregarRecibo *f = new FormAgregarRecibo();
    f->setearModelo(0);
    f->setearDatos( id_cliente, texto, total );
    emit agregarVentana( f );
}

/*!
 * \brief PagosPlugin::mostrarRecibo
 *  Genera una ventana donde se ven los datos de un recibo ya emitido
 * \param id_recibo Identificador del recibo a mostrar
 */
void PagosPlugin::mostrarRecibo( int id_recibo )
{
    FormRecibo *f = new FormRecibo();
    f->setId( id_recibo );
    emit agregarVentana( f );
}

Q_EXPORT_PLUGIN2( pagos, PagosPlugin )