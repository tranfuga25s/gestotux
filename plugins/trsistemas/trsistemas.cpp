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

#include "trsistemas.h"
#include "EReporte.h"
#include "mpagos.h"
#include "util.h"

bool TRSistemas::inicializar()
{
 Q_INIT_RESOURCE(trsistemas);
 return true;
}

bool TRSistemas::verificarTablas( QStringList /*tablas*/ )
{ return true; }

double TRSistemas::version() const
{ return 0.1; }

int TRSistemas::tipo() const
{ return EPlugin::info; }

QList< QActionGroup * > TRSistemas::accionesBarra()
{ return QList<QActionGroup*>(); }

QString TRSistemas::nombre() const
{ return "trsistemas"; }

QWidgetList TRSistemas::formsPreferencias()
{ return QWidgetList(); }

void TRSistemas::crearMenu( QMenuBar* /*m*/ )
{}

void TRSistemas::crearToolBar(QToolBar* /*t*/)
{}

void TRSistemas::seCierraGestotux()
{ Q_CLEANUP_RESOURCE(trsistemas); }

QImage TRSistemas::imagenPrograma() const
{ return QImage( ":/imagenes/logoTR.png" ); }

QString TRSistemas::nombrePrograma() const { return "TR SII"; }

QIcon TRSistemas::iconoPrograma() const
{ return QIcon( ":/imagenes/iconoTR.png" ); }

QString TRSistemas::directorioBackup() const
{ return "trsistemas"; }

QString TRSistemas::directorioActualizaciones() const
{ return "trsistemas"; }

QString TRSistemas::empresa() const
{ return "TR Sistemas Informáticos Integrales"; }

QString TRSistemas::companeros()
{ return ""; }


bool TRSistemas::publicidad()
{ return false; }

QString TRSistemas::reporte( int tipo )
{
    switch( tipo ) {
        case EReporte::Factura:
        { return "Factura-TR"; break; }
        case EReporte::Recibo:
        { return "Recibo-TR"; break; }
        case EReporte::Presupuesto:
        { return "Presupuesto-TR"; break; }
        case EReporte::AnulacionFactura:
        { return "AnulacionFactura"; break; }
        case EReporte::Remito:
        { return "Remito-TR"; break; }
        default: { return ""; break; }
    }
}

void TRSistemas::reporteParametros( int tipo, QString &/*nombre*/, ParameterList &parametros )
{
    switch( tipo ) {
        case EReporte::Recibo:
        {
            // Busco el identificador de recibo
            int id_recibo = parametros.value( "id_recibo" ).toInt();
            MPagos *mr = new MPagos();
            double importe = mr->buscarImporte( mr->buscarNumeroComprobantePorId( id_recibo ) );
            parametros.append( "texto_importe", Util::importeATexto( importe ) );
            delete mr;
            break;
        }
        default: { return; break; }
    }
}

Q_EXPORT_PLUGIN2( trsistemas, TRSistemas )
