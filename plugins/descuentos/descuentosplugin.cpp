/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller                                  *
 *   juiraze@yahoo.com.ar                                                  *
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
#include "descuentosplugin.h"

#include "formprefdescuentos.h"

#include <QSqlDatabase>
#include <QAction>

/*!
    \fn DescuentosPlugin::accionesBarra()
 */
QList<QActionGroup *> DescuentosPlugin::accionesBarra()
{  return QList<QActionGroup *>();  }

/*!
    \fn DescuentosPlugin::nombre() const
 */
QString DescuentosPlugin::nombre() const
{  return "descuentos";  }

/*!
    \fn DescuentosPlugin::formsPreferencias()
 */
QWidgetList DescuentosPlugin::formsPreferencias()
{
 QWidgetList lista;
 lista.append( new FormPrefDescuentos() );
 return lista;
}


/*!
    \fn DescuentosPlugin::inicializar()
 */
bool DescuentosPlugin::inicializar()
{
 Q_INIT_RESOURCE(descuentos);
 return true;
}


/*!
    \fn DescuentosPlugin::verificarTablas( QStringList tablas )
 */
bool DescuentosPlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "tipo_descuento" ) )
 { qDebug( "DescuentosPlugin::Error al buscar la tabla tipo_descuento" ); return false; }
 else if( !tablas.contains( "descuentos" ) )
 { qDebug( "DescuentosPlugin::Error al buscar la tabla descuentos" ); return false; }
 else if( !tablas.contains( "descuento_comprobante" ) )
 { qDebug( "DescuentosPlugin::Error al buscar la tabla descuento_comprobante" ); return false; }
 return true;
}


/*!
    \fn DescuentosPlugin::tipo() const
 */
int DescuentosPlugin::tipo() const
{  return EPlugin::comun; }


/*!
    \fn DescuentosPlugin::crearMenu( QMenuBar *m )
 */
void DescuentosPlugin::crearMenu( QMenuBar */*m*/ )
{}


/*!
    \fn DescuentosPlugin::version() const
        Devuelve la version del plugin
 */
double DescuentosPlugin::version() const
{ return 0.01; }

/*!
    \fn DescuentosPlugin::crearToolBar( QToolBar *t )
 */
void DescuentosPlugin::crearToolBar( QToolBar */*t*/ )
{}

/*!
    \fn DescuentosPlugin::seCierraGestotux()
 */
void DescuentosPlugin::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(descuentos); return; }

QAction *DescuentosPlugin::botonPantallaInicial()
{  return 0; }

Q_EXPORT_PLUGIN2( descuentos, DescuentosPlugin )