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
#include "mproveedor.h"

#include <QSqlDatabase>
#include <QMessageBox>

/**
 * @brief Compras::inicializar
 * @return
 */
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

/**
 * @brief Compras::verificarTablas
 * @param tablas
 * @return
 */
bool Compras::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "compras" ) )
 { qDebug( "Error al buscar la tabla compras" ); return false; }
 if( !tablas.contains( "compras_productos" ) )
 { qDebug( "Error al buscar la tabla compras_productos" ); return false; }
 if( !tablas.contains( "resumen_compra" ) )
 { qDebug( "Error al buscar la tabla resumen_compra" ); return false; }
 return true;
}

/**
 * @brief Compras::version
 * @return
 */
double Compras::version() const
{ return 0.11; }

/**
 * @brief Compras::tipo
 * @return
 */
int Compras::tipo() const
{ return EPlugin::comun; }

/**
 * @brief Compras::accionesBarra
 * @return
 */
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

/**
 * @brief Compras::nombre
 * @return
 */
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

/**
 * @brief Compras::crearToolBar
 */
void Compras::crearToolBar(QToolBar*/* t */)
{}

#include "formagregarcompra.h"
/**
 * @brief Compras::agregarCompra
 */
void Compras::agregarCompra()
{
    // Validar que exista al menos un proveedor para ingresar una compra
    if (!MProveedor::existenProveedores()) {
        QMessageBox::warning(0, tr("No hay proveedores"), "No existen proveedores para agregar una nueva compra");
        return;
    }
    emit agregarVentana( new FormAgregarCompra() );
}


#include "vcompras.h"
/**
 * @brief Compras::ver_compras
 */
void Compras::ver_compras()
{ emit agregarVentana( new VCompras() ); }

/**
 * @brief Compras::seCierraGestotux
 */
void Compras::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(compras);
 return;
}

Q_EXPORT_PLUGIN2( compras, Compras )