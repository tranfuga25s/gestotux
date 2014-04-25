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
#include "gestotuxdefault.h"

bool GestotuxDefault::publicidad()
{ return true; }
//{ return false; }

double GestotuxDefault::version() const
{ return 1; }

QIcon GestotuxDefault::iconoPrograma() const
{ return QIcon( ":/imagenes/gestotux32.png"  ); }

QImage GestotuxDefault::imagenPrograma() const
{ return QImage( ":/imagenes/gestotux.png"  ); }

QString GestotuxDefault::companeros()
{ return ""; }

QString GestotuxDefault::directorioActualizaciones() const
{ return ""; }

QString GestotuxDefault::directorioBackup() const
{ return ""; }

QString GestotuxDefault::empresa() const
{ return "uso generico"; }

QString GestotuxDefault::nombrePrograma() const
{ return "Gestotux 0.5 RC 13"; }

bool GestotuxDefault::inicializar()
{
    Q_INIT_RESOURCE(default);
    return true;
}

bool GestotuxDefault::verificarTablas( QStringList tablas )
{
    if( !tablas.contains( "paises" ) ) {
        qDebug( "No se encontro la tabla de paises" );
        return false;
    } else if( !tablas.contains( "provincias" ) ) {
        qDebug( "No se encontro la tabla de provincias" );
        return false;
    } else if( !tablas.contains( "estado_fiscal" ) ) {
        qDebug( "No se encontro la tabla de estados_fiscales" );
        return false;
    }
    return true;
}

int GestotuxDefault::tipo() const
{ return EPlugin::info; }

QList< QActionGroup * > GestotuxDefault::accionesBarra()
{ return QList<QActionGroup *>(); }

QString GestotuxDefault::nombre() const
{ return "default"; }

QWidgetList GestotuxDefault::formsPreferencias()
{ return QWidgetList();}

void GestotuxDefault::crearMenu(QMenuBar* /*m*/)
{}

void GestotuxDefault::crearToolBar(QToolBar* /*t*/)
{}

void GestotuxDefault::seCierraGestotux()
{
 qDebug( "Cerrado plugin default" );
 Q_CLEANUP_RESOURCE(default);
 return;
}

QAction *GestotuxDefault::botonPantallaInicial()
{ return 0; }

QString GestotuxDefault::reporte( int tipo ) {
    // Lista de reportes por default
    switch( tipo ) {
        case EReporte::Presupuesto:
        { return "Presupuesto"; break; }
        case EReporte::Factura:
        { return "Factura"; break; }
        case EReporte::Recibo:
        { return "Recibo"; break; }
        case EReporte::AnulacionFactura:
        { return "AnulacionFactura"; break; }
        case EReporte::Remito:
        { return "Remito"; break; }
        default:
        { return QString(); }
    }
}

void GestotuxDefault::reporteParametros(int, QString &, ParameterList & )
{}

Q_EXPORT_PLUGIN2( zinfo, GestotuxDefault )
