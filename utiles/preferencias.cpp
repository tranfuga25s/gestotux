/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
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
#include "preferencias.h"

preferencias *preferencias::instance = 0;

/*!
  \fn preferencias::getInstancia()
  Devuleve un puntero al objeto que mantiene las preferencias de la aplicación
 */
preferencias* preferencias::getInstancia()
{
 if (instance==0)
 {
  instance = new preferencias();
 }
 return instance;
}


preferencias::preferencias()
: QSettings( "Tranfuga", "GestoTux" )
//: QSettings( "TRSis", "GestoTux" )
{
 // Seteo el numero de la libreria
 this->setValue( "Preferencias/Generales/utiles/numseq", 1 );
}


preferencias::~preferencias()
{
 if (this == instance)
 {
   instance = 0;
 }
 sync();
 delete instance;
}

/*!
    \fn preferencias::inicio()
 */
void preferencias::inicio()
{
 while( !group().isEmpty() )
 {
  endGroup();
 }
}
