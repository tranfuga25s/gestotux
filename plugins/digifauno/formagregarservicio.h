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

#ifndef FORMAGREGARSERVICIO_H
#define FORMAGREGARSERVICIO_H

#include "eventana.h"
#include "ui_FormServPeluqueria.h"

class MMascota;
class MPeluqueria;
class QVariant;

/*!
 * 	\brief Formulario de agregar servicio
 *
 *	Formulario que permite agregar un nuevo servicio de peluqueria a una mascota
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormAgregarServicio : public EVentana, private Ui::FormAgregarServPeluqueriaBase
{
  Q_OBJECT
public:
  FormAgregarServicio( QWidget* parent = 0  );
  ~FormAgregarServicio();

protected slots:
    void cambioIndice( int indice );
    void agregar();
    void borrar();
    void cerrar();

private:
   /*!
    * Modelo de tabla peluqueria
    */
    MPeluqueria *modeloPeluqueria;
   /*!
    * id de mascota
    */
    QVariant id_mascota;
};

#endif

