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

#ifndef FORMACERCADE_H
#define FORMACERCADE_H

#include "eventana.h"
#include "ui_FormAcercaDeBase.h"

/*!
 * 	\brief Formulario de "Acerca de"
 *
 *  Implementacion de la ventana de Acerca de
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormAcercaDe : public EVentana, private Ui::FormAcercaDe
{
  Q_OBJECT

public:
  FormAcercaDe(QWidget* parent = 0);
  ~FormAcercaDe();
};

#endif

