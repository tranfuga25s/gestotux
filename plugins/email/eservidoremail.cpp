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
#include "eservidoremail.h"
#include <QSqlRecord>

EServidorEmail::EServidorEmail( QObject *parent, QSqlDatabase db )
 : QSqlTableModel( parent, db )
{
 	//setEditStrategy( QSqlTableModel::OnManualSubmit );
	setTable( "servidores_email" );
	setHeaderData( 0, Qt::Horizontal, tr( "#ID" ) );
	setHeaderData( 1, Qt::Horizontal, tr( "Nombre" ) );
	setHeaderData( 2, Qt::Horizontal, tr( "Direccion" ) );
	setHeaderData( 3, Qt::Horizontal, tr( "# Puerto" ) );
	setHeaderData( 4, Qt::Horizontal, tr( "Usuario" ) );
	setHeaderData( 5, Qt::Horizontal, tr( "Contraseña" ) );
 	setHeaderData( 6, Qt::Horizontal, tr( "De" ) );
	setHeaderData( 6, Qt::Horizontal, tr( "Predeterminado" ) );
	_buscado_predeterminado = false;
}


EServidorEmail::~EServidorEmail()
{
}




/*!
    \fn EServidorEmail::puerto()
 */
int EServidorEmail::puerto()
{
 buscarPredeterminado();
 return _puerto;
}


/*!
    \fn EServidorEmail::direccion()
 */
QString EServidorEmail::direccion()
{
 buscarPredeterminado();
 return _direccion;
}


/*!
    \fn EServidorEmail::usuario()
 */
QString EServidorEmail::usuario()
{
 buscarPredeterminado();
 return _usuario;
}


/*!
    \fn EServidorEmail::password()
 */
QString EServidorEmail::password()
{
 buscarPredeterminado();
 return _password;
}

#include <QSqlQuery>
/*!
    \fn EServidorEmail::buscarPredeterminado()
 */
void EServidorEmail::buscarPredeterminado()
{
 if( _buscado_predeterminado )
 { return; }
 QSqlQuery cola( "SELECT * FROM servidores_email WHERE predeterminado = 1 OR predeterminado LIKE 'true' LIMIT 1", this->database() );
 if( cola.next() )
 {
  this->_nombre = cola.record().value( "nombre" ).toString();
  this->_direccion = cola.record().value( "direccion" ).toString();
  this->_usuario = cola.record().value( "usuario" ).toString();
  this->_password = cola.record().value( "pass" ).toString();
  this->_puerto = cola.record().value( "puerto" ).toInt();
  this->_de = cola.record().value( "de" ).toString();
  _buscado_predeterminado = true;
 }
 else
 {
  qWarning( "No se pudo encontrar ningun servidor de email predeterminado" );
 }
}


/*!
    \fn EServidorEmail::de()
 */
QString EServidorEmail::de()
{
 buscarPredeterminado();
 return this->_de;
}
