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
#ifndef BARRALATERAL_H
#define BARRALATERAL_H

#include <QDockWidget>
class BotonesLaterales;

/**
	\brief Clase que sirve de contenedor de la barra lateral

	Clase que sirve unicamente para mostrar la barra lateral y mantener sus configuraciones...

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class BarraLateral : public QDockWidget
{
 Q_OBJECT
public:
    BarraLateral( const QString & title, QWidget * parent = 0 );
    ~BarraLateral();
   /*!
    * Barra lateral
    */
    BotonesLaterales *ls;

};

#endif
