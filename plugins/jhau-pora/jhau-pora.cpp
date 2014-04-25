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

#include "jhau-pora.h"
#include "EReporte.h"
#include "util.h"

bool JhauPora::inicializar()
{
    Q_INIT_RESOURCE(jhaupora);
    return true;
}

bool JhauPora::verificarTablas( QStringList /*tablas*/ )
{ return true; }

double JhauPora::version() const
{ return 0.1; }

int JhauPora::tipo() const
{ return EPlugin::info; }

QList< QActionGroup * > JhauPora::accionesBarra()
{ return QList<QActionGroup*>(); }

QString JhauPora::nombre() const
{ return "jhau-pora"; }

QWidgetList JhauPora::formsPreferencias()
{ return QWidgetList(); }

void JhauPora::crearMenu( QMenuBar* /*m*/ )
{}

void JhauPora::crearToolBar(QToolBar* /*t*/)
{}

void JhauPora::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(jhaupora); }

QImage JhauPora::imagenPrograma() const
{ return QImage( ":/imagenes/icono-jhau-pora.png" ); }

QString JhauPora::nombrePrograma() const { return "JHAU-PORA"; }

QIcon JhauPora::iconoPrograma() const
{ return QIcon( ":/imagenes/icono-jhau-pora.png" ); }

QString JhauPora::directorioBackup() const
{ return "jhau-pora"; }

QString JhauPora::directorioActualizaciones() const
{ return "jhau-pora"; }

QString JhauPora::empresa() const
{ return "jhau-pora"; }

QString JhauPora::companeros()
{ return "Asesor comercial: \n Daniel Sequeira <daniels598@gmail.com> - Celular: +54934255128211"; }


bool JhauPora::publicidad()
{ return false; }

QString JhauPora::reporte( int tipo )
{
    switch( tipo ) {
        case EReporte::Factura:
        { return "Factura"; break; }
        case EReporte::Recibo:
        { return "Recibo"; break; }
        case EReporte::Presupuesto:
        { return "Presupuesto"; break; }
        case EReporte::AnulacionFactura:
        { return "AnulacionFactura"; break; }
        case EReporte::Remito:
        { return "Remito"; break; }
        default: { return ""; break; }
    }
}

void JhauPora::reporteParametros( int tipo, QString &/*nombre*/, ParameterList &parametros )
{}

Q_EXPORT_PLUGIN2( jhaupora, JhauPora )
