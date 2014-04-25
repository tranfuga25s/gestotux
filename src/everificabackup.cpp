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
#include "everificabackup.h"
#include "preferencias.h"
#include <QDate>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QSqlDatabase>

EVerificaBackup::EVerificaBackup(QWidget *parent)
 : QPushButton(parent)
{
 this->setToolTip( "Verificación de Backups" );
 tiempo = new QTimer(this);
 tiempo->setInterval( 10000 );
 connect( tiempo, SIGNAL( timeout() ), this, SLOT( verificar() ) );
 tiempo->start();
 this->setFlat(true);
}

/*!
    \fn EVerificaBackup::verificar()
	Funcion llamada cada cierto tiempo que permite verificar si el backup no cumple con la frecuencia pedida.
 */
void EVerificaBackup::verificar()
{
 if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() != "QSQLITE" )
 { tiempo->stop(); delete tiempo; return; }
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 int cant_dias = 0;
 switch( p->value( "frecuenciaBackup", 1 ).toInt() )
 {
	// 1 dia
	case 1:
	{ cant_dias = 1; break; }
	// 2 dias
	case 2:
	{ cant_dias = 2; break; }
	// 1 semana
	case 3:
	{ cant_dias = 7; break; }
	// 2 semanas
	case 4:
	{ cant_dias = 7*2; break; }
	// 1 mes
	case 5:
	{ cant_dias = 30; break; }
	// 2 meses
	case 6:
	{ cant_dias = 60; break; }
	// Nunca
	case 7:
	{ return; }
	// Por si todavia no se eligio ninguna preferencia
	default:
	{ cant_dias = 15; break; }
 }
 p->endGroup();
 p->beginGroup( "backup" );
 QDate fecha = p->value( "fecha", QDate( 1900, 1, 1 ) ).toDate();
 //qDebug( qPrintable( "Fecha ultimo backup =" +  fecha.toString("dd/MM/yyyy" ) ) );
 p->endGroup();
 int dif = fecha.daysTo( QDate::currentDate() );
 if( dif > cant_dias )
 {
  show();
  QIcon icono( ":/imagenes/aviso.png" );
  this->setIcon( icono );
  this->setIconSize( QSize( 16, 16 ) );
  connect( this, SIGNAL( clicked() ), this, SIGNAL( abrirBackups() ) );
  this->setToolTip( "El ultimo backup fue echo hace mas de " + QString::number( cant_dias ) + " dias.\n Haga click aqui para generar un backup" );
 }
 else
 {
  hide();
 }
 p->endGroup();
 p=0;
}


