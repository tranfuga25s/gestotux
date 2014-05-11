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
#include "compras.h"

#include <QSqlDatabase>

bool Compras::inicializar()
{
 Q_INIT_RESOURCE(compras);

 ActAgregarCompra = new QAction( "Agregar Nueva Compra", this );
 ActAgregarCompra->setIcon( QIcon( ":/imagenes/agregar_compras.png" ) );
 connect( ActAgregarCompra, SIGNAL( triggered() ), this, SLOT( agregarCompra() ) );

 ActCompras = new QAction( "Compras", this );
 ActCompras->setIcon( QIcon( ":/imagenes/compras.png" ) );
 ActCompras->setStatusTip( "Muestra el historial de compras" );
 connect( ActCompras, SIGNAL( triggered() ), this, SLOT( ver_compras() ) );

 return true;
}

bool Compras::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "compras" ) )
 { qWarning( "Error al buscar la tabla compras" ); return false; }
 if( !tablas.contains( "compras_productos" ) )
 { qWarning( "Error al buscar la tabla compras_productos" ); return false; }
 if( !tablas.contains( "resumen_compra" ) )
 { qWarning( "Error al buscar la tabla resumen_compra" ); return false; }
 return true;
}

double Compras::version() const
{ return 0.11; }

int Compras::tipo() const
{ return EPlugin::comun; }

QList< QActionGroup * > Compras::accionesBarra()
{
 QList<QActionGroup *> lista;
 QActionGroup *compras = new QActionGroup( this );
 compras->setObjectName( "compras" );
 compras->setProperty( "icono", ":/imagenes/compras.png" );
 compras->setProperty( "titulo", "Compras y Gastos" );
 compras->addAction( ActAgregarCompra );
 lista.append( compras );
 return lista;
}

QString Compras::nombre() const
{ return "compras"; }

#include "formprefcompras.h"
/*!
 * \brief Compras::formsPreferencias
 * \return
 */
QWidgetList Compras::formsPreferencias()
{
    QWidgetList lista;
    lista.append( new FormPrefCompras() );
    return lista;
}

/*!
 * \brief Compras::crearMenu
 * \param m
 */
void Compras::crearMenu(QMenuBar* m)
{
 QMenu *mCompras = m->addMenu( "Compras" );
 mCompras->setObjectName( "menuCompras" );
 mCompras->addAction( ActAgregarCompra );
 mCompras->addSeparator();
 mCompras->addAction( ActCompras );
}

/*!
 * \brief Compras::crearToolBar
 */
void Compras::crearToolBar(QToolBar*/* t */)
{}

#include "formagregarcompra.h"
/*!
    \fn Compras::agregarCompra()
 */
void Compras::agregarCompra()
{ emit agregarVentana( new FormAgregarCompra() ); }


#include "vcompras.h"
/*!
    \fn Compras::ver_compras()
 */
void Compras::ver_compras()
{ emit agregarVentana( new VCompras() ); }

/*!
    \fn Compras::seCierraGestotux()
 */
void Compras::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(compras);
 return;
}

Q_EXPORT_PLUGIN2( compras, Compras )