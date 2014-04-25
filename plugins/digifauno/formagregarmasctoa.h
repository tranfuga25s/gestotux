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

#ifndef FORMAGREGARMASCTOA_H
#define FORMAGREGARMASCTOA_H

#include "eventana.h"
#include "ui_FormAgregarMascota.h"

class MDueno;
class MRazas;

/*!
 * 	\brief Formulario para agregar mascotas
 *
 *	Formulario que permite agregar una mascota e incluso agregar un dueño si es necesario
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormAgregarMascota : public EVentana, private Ui::FormAgregarMascotaBase
{
  Q_OBJECT

public:
  FormAgregarMascota( QWidget* parent = 0 );
  ~FormAgregarMascota();

protected:
   /*!
    * Modelo para dueños
    */
    MDueno *modeloDueno;
   /*!
    * Modelo de razas para autocompletado
    */
    MRazas *modeloRazas;
   /*!
    * Auto completador para el Line edit de razas
    */
    QCompleter *completarazas;

protected slots:
    void guardar();

public slots:
    void cancelar();
    void agregarDueno();

};

#endif

