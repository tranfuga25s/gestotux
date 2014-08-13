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
#include "garantiasplugin.h"

#include <QString>
#include <QIcon>
#include <QAction>
#include <QMenuBar>
#include <QList>
#include <QStackedWidget>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

/*!
    \fn GarantiasPlugin::accionesBarra()
        Retorna las acciones que tiene la barra lateral para este plugin
        @return Lista de acciones
 */
QList<QActionGroup *> GarantiasPlugin::accionesBarra()
{ return QList<QActionGroup*>(); }

/*!
 * \brief GarantiasPlugin::inicializar
 * \return
 */
bool GarantiasPlugin::inicializar()
{
    Q_INIT_RESOURCE(garantias);

    ActGarantias = new QAction( this );
    ActGarantias->setText( QString::fromUtf8( "Garantías" ) );
    ActGarantias->setIcon( QIcon(":/imagenes/garantia.png" ) );
    ActGarantias->setStatusTip( "Muestra el listado de garantías vigentes en el sistema" );
    connect( ActGarantias, SIGNAL( triggered() ), this, SLOT( verGarantias() ) );

    ActAgregarGarantia = new QAction( this );
    ActAgregarGarantia->setText( QString::fromUtf8( "Agregar Garantía" ) );
    ActAgregarGarantia->setIcon( QIcon( ":/imagenes/garantia_agregar.png" ) );
    ActAgregarGarantia->setStatusTip( "Muestra la ventana para agregar una nueva garantia" );
    connect( ActAgregarGarantia, SIGNAL( triggered() ), this, SLOT( agregarGarantia() ) );

    ActVerVencimientos = new QAction( this );
    ActVerVencimientos->setText( "Ver vencimientos" );
    ActVerVencimientos->setIcon( QIcon( ":/imagenes/garantia_vencimiento.png" ) );
    ActVerVencimientos->setStatusTip( "Ver los vencimientos de garantias cercanas a una fecha" );
    connect( ActVerVencimientos, SIGNAL( triggered() ), this, SLOT( verVencimientos() ) );

    return true;
}

#include "formprefgarantias.h"
/*!
    \fn GarantiasPlugin::formsPreferencias()
 */
QWidgetList GarantiasPlugin::formsPreferencias()
{
 QWidgetList lista;
 lista.append( new FormPrefGarantias() );
 return lista;
}

/*!
    \fn GarantiasPlugin::nombre() const
 */
QString GarantiasPlugin::nombre() const
{ return "garantias"; }


/*!
    \fn GarantiasPlugin::tipo() const
 */
int GarantiasPlugin::tipo() const
{ return EPlugin::comun; }


/*!
    \fn GarantiasPlugin::crearMenu( QMenuBar *m )
 */
void GarantiasPlugin::crearMenu( QMenuBar *m )
{
 QMenu *menuGarantias = m->addMenu( "Garantias" );
 menuGarantias->addAction( ActGarantias );
 menuGarantias->addAction( ActAgregarGarantia );
 menuGarantias->addSeparator();
 menuGarantias->addAction( ActVerVencimientos );
}

/*!
 * \brief GarantiasPlugin::verificarTablas
 * \param tablas
 * \return
 */
bool GarantiasPlugin::verificarTablas( QStringList tablas )
{
    if( !tablas.contains( "factura" ) )
    { qDebug() << "Error al buscar la tabla factura"; return false; }
    if( !tablas.contains( "clientes" ) )
    { qDebug() << "Error al buscar la tabla clientes"; return false; }
    if( !tablas.contains( "equipamientos" ) )
    { qDebug() << "Error al buscar la tabla de equipamientos"; return false; }
    if( !tablas.contains( "garantias" ) )
    { qDebug() << "Error al buscar la tabla garantias"; return false; }
    if( !tablas.contains( "v_garantias" ) )
    { qDebug() << "Error al buscar la tabla v_garantias"; return false; }
    if( !tablas.contains( "v_garantias_svg" ) )
    { qDebug() << "Error al buscar la tabla v_garantias_svg"; return false; }
    return true;
}


/*!
    \fn GarantiasPlugin::version() const
 */
double GarantiasPlugin::version() const
{  return 0.2; }

/*!
    \fn GarantiasPlugin::crearToolBar( QToolBar *t )
 */
void GarantiasPlugin::crearToolBar( QToolBar */*t*/ )
{}


/*!
    \fn GarantiasPlugin::seCierraGestotux()
 */
void GarantiasPlugin::seCierraGestotux()
{
    Q_CLEANUP_RESOURCE( garantias );
}

#include "vgarantias.h"
/*!
 * \fn GarantiasPlugin::verGarantias()
 * Muestra el listado de garantias que estan actualmente
 */
void GarantiasPlugin::verGarantias()
{
    emit agregarVentana( new VGarantias() );
}

#include "dagregargarantia.h"
/*!
 * \brief GarantiasPlugin::agregarGarantia
 * \param id_comprobante
 * \param id_producto
 * \param nombre_producto
 * \param id_cliente
 */
void GarantiasPlugin::agregarGarantiaProducto( int id_comprobante, int id_producto, QString nombre_producto, int id_cliente )
{
    DAgregarGarantia *d = new DAgregarGarantia();
    d->setearIdCliente( id_cliente );
    d->setearIdComprobante( id_comprobante );
    d->setearIdProducto( id_producto );
    d->setearNombreProducto( nombre_producto );
    d->exec();
}

/*!
 * \brief GarantiasPlugin::agregarGarantiaEquipamiento
 * \param id_comprobante
 * \param id_equipamiento
 * \param nombre_producto
 * \param id_cliente
 */
void GarantiasPlugin::agregarGarantiaEquipamiento( int id_comprobante, int id_equipamiento, QString nombre_producto, int id_cliente )
{
    DAgregarGarantia *d = new DAgregarGarantia();
    d->setearIdCliente( id_cliente );
    d->setearIdComprobante( id_comprobante );
    d->setearIdEquipamiento( id_equipamiento );
    d->setearNombreProducto( nombre_producto );
    d->exec();
}


/*!
 * \brief GarantiasPlugin::agregarGarantia
 */
void GarantiasPlugin::agregarGarantia()
{
    DAgregarGarantia *d = new DAgregarGarantia();
    d->exec();
}

#include "formvencimientogarantias.h"
/*!
 * \brief GarantiasPlugin::verVencimientos
 */
void GarantiasPlugin::verVencimientos()
{
    emit agregarVentana( new FormVencimientoGarantias() );
}

/*!
 * \brief GarantiasPlugin::botonPantallaInicial
 * \return
 */
QAction *GarantiasPlugin::botonPantallaInicial()
{
    return ( new QAction( this ) );
}

Q_EXPORT_PLUGIN2( garantias, GarantiasPlugin )
