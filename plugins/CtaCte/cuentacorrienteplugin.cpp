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
#include "cuentacorrienteplugin.h"

#include <QSqlDatabase>
#include <QApplication>
#include <QDate>
#include <QDebug>

#include "preferencias.h"
#include "vcuentacorriente.h"
#include "formprefctacte.h"

bool CuentaCorrientePlugin::inicializar()
{

 ActCuentasCorrientes = new QAction( this );
 ActCuentasCorrientes->setText( "Cuentas Corrientes" );
 ActCuentasCorrientes->setIcon( QIcon( ":/imagenes/ctacte.png" ) );
 ActCuentasCorrientes->setToolTip( "Muestra la lista de cuentas corrientes" );
 connect( ActCuentasCorrientes, SIGNAL( triggered() ), this, SLOT( verCuentasCorrientes() ) );

 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "CtaCte" );

 if( p->value( "habilitada", true ).toBool() )
 { QApplication::instance()->setProperty( "habilitada-ctacte", true ); }
 else
 { QApplication::instance()->setProperty( "habilitada-ctacte", false ); }

 p->endGroup();
 p->endGroup();

 return true;
}

bool CuentaCorrientePlugin::verificarTablas( QStringList tablas )
{
 if( !tablas.contains( "ctacte" ) )
 { qWarning() << QString::fromUtf8( "No se encontro la tabla de cuenta corriente <br /> Se creará" ); return false; }
 if( !tablas.contains( "item_ctacte" ) )
 { qWarning() << "Error al buscar la tabla de item cuenta corriente"; return false; }
 return true;
}

double CuentaCorrientePlugin::version() const
{
 return 0.1;
}

int CuentaCorrientePlugin::tipo() const
{
 return EPlugin::comun;
}

QList< QActionGroup * > CuentaCorrientePlugin::accionesBarra()
{
 return QList<QActionGroup *>();
}

QString CuentaCorrientePlugin::nombre() const
{
 return "ctacte";
}

QWidgetList CuentaCorrientePlugin::formsPreferencias()
{
 QWidgetList lista;
 lista.append( new FormPrefCtaCte() );
 return lista;
}

void CuentaCorrientePlugin::crearMenu( QMenuBar* m )
{
 QMenu *menuHerramientas = m->findChild<QMenu *>( "menuHerramientas" );
 if( menuHerramientas == 0 )
 {
  qDebug() << "CtaCte: Error en las baras de menu";
 }
 else
 {
  menuHerramientas->addAction( ActCuentasCorrientes );
 }
}

void CuentaCorrientePlugin::crearToolBar(QToolBar* /*t*/)
{
}

void CuentaCorrientePlugin::seCierraGestotux()
{}

Q_EXPORT_PLUGIN2( ctacte, CuentaCorrientePlugin )

/*!
    \fn CuentaCorrientePlugin::verCuentasCorrientes()
 */
void CuentaCorrientePlugin::verCuentasCorrientes()
{
 VCuentaCorriente *f = new VCuentaCorriente();
 connect( f, SIGNAL( emitirRecibo( int, QString, double ) ), this, SIGNAL( emitirRecibo( int, QString, double ) ) );
 connect( f, SIGNAL( mostrarRecibo( int ) ), this, SIGNAL( mostrarRecibo( int ) ) );
 connect( f, SIGNAL( mostrarFactura( int ) ), this, SIGNAL( mostrarFactura( int ) ) );
 connect( f, SIGNAL( mostrarRemito( int ) ), this, SIGNAL( mostrarRemito( int ) ) );
 emit agregarVentana( f );
}
