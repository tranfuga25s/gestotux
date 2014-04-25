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
#include "emautos.h"

EMAutos::EMAutos(QObject *parent)
 : QSqlQueryModel(parent)
{
 filtrarPorCliente( -1 );
}


EMAutos::~EMAutos()
{
}




/*!
    \fn EMAutos::filtrarPorCliente( int id_cliente )
 */
void EMAutos::filtrarPorCliente( int id_cliente )
{
 if( QSqlDatabase::database().driverName() == "QMYSQL" )
 {
  if( id_cliente != -1 )
  {
   	this->setQuery( QString( "SELECT id_auto, CONCAT(CONCAT(CONCAT(CONCAT(CONCAT(CONCAT( patente, ' - ' ) , marca ), ' ' ), modelo ), ' - ' ), color )  FROM autos WHERE id_dueno = %1" ).arg( id_cliente ) );
	return;
  }
  else
  {
   	this->setQuery( "SELECT id_auto, CONCAT(CONCAT(CONCAT(CONCAT(CONCAT(CONCAT( patente, ' - ' ) , marca ), ' ' ), modelo ), ' - ' ), color )  FROM autos" );
	return;
  }
 }
 else if( QSqlDatabase::database().driverName() == "QSQLITE" )
 {
  if( id_cliente != -1 )
  {
   	this->setQuery( QString( "SELECT id_auto,  patente || ' - ' || marca ||' '|| modelo || ' - ' || color   FROM autos WHERE id_dueno = %1" ).arg( id_cliente ) );
	return;
  }
  else
  {
   	this->setQuery( "SELECT id_auto,  patente || ' - ' || marca ||' '|| modelo || ' - ' || color   FROM autos" );
	return;
  }
 }
 else
 {
	qDebug( "No se encontro el driver de base de datos" );
	return;
 }
}
