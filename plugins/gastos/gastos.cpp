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
#include "gastos.h"
#include <QSqlDatabase>
#include <QMessageBox>

Q_EXPORT_PLUGIN2( gastos, Gastos );

bool Gastos::inicializar()
{
 Q_INIT_RESOURCE(gastos);
 /////////////////////////////////////
 // Muestra los gastos
 /////////////////////////////////////
 ActGastos = new QAction( "Gastos", this );
 ActGastos->setStatusTip( "Ver la lista de gastos" );
 ActGastos->setIcon( QIcon( ":/imagenes/gasto.jpg" ) );
 connect( ActGastos, SIGNAL( triggered() ), this, SLOT( ver_gastos() ) );
 /////////////////////////////////////
 // Muestra el agregar un gasto
 /////////////////////////////////////
 ActAgregarGasto = new QAction( "Agregar Gasto", this );
 ActAgregarGasto->setStatusTip( "Agregar nuevo gasto" );
 ActAgregarGasto->setIcon( QIcon( ":/imagenes/gasto.jpg" ) );
 connect( ActAgregarGasto, SIGNAL( triggered() ), this, SLOT( agregarGasto() ) );

 return true;
}

bool Gastos::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "gastos" ) )
 { qWarning( "Gastos::Error al buscar la tabla gastos" ); return false; }
 if( !tablas.contains( "categoria_gastos" ) )
 { qWarning( "Gastos::Error al buscar la tabla categorias_gastos" ); return false; }
 return true;
}

double Gastos::version() const
{
 return 0.1;
}

int Gastos::tipo() const
{
 return EPlugin::comun;
}

QList< QActionGroup * > Gastos::accionesBarra()
{
 /*QList<QActionGroup *> lista;
 QActionGroup *gastos = new QActionGroup( this );
 gastos->setObjectName( "Gastos" );
 gastos->addAction( ActAgregarGasto );
 lista.append( Gastos );
 return lista;*/
 return QList<QActionGroup *>();
}

QString Gastos::nombre() const
{
 return "gastos";
}

QWidgetList Gastos::formsPreferencias()
{
 return QWidgetList();
}

void Gastos::crearMenu(QMenuBar* m)
{
 QMenu *menuHerramientas = m->findChild<QMenu *>( "menuHerramientas" );
 if( menuHerramientas == 0 )
 {
  qDebug( "Error en las baras de menu" );
 }
 else
 {
  menuHerramientas->addAction( ActGastos );
 }
}

void Gastos::crearToolBar(QToolBar* t)
{
    (void)t;
}


#include "formagregargasto.h"
#include "mcategoriasgastos.h"
/*!
    \fn Gastos::agregarGasto()
        Abre la ventana para agregar un gasto
 */
void Gastos::agregarGasto()
{
    if( !MCategoriasGastos::vacio() ) {
        emit agregarVentana( new FormAgregarGasto() );
        return;
    } else {
        QMessageBox::information( 0, "Faltan datos", "No existe ninguna categoría de gastos definida. \n Ingrese a la lista de gastos para agregar una nueva categoría" );
        return;
    }
}


#include "vgastos.h"
/*!
    \fn Gastos::ver_gastos()
    Muestra el listado de gastos que haya cargado
 */
void Gastos::ver_gastos()
{ emit agregarVentana( new VGastos() ); }


/*!
    \fn Gastos::seCierraGestotux()
 */
void Gastos::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(gastos); return; }
