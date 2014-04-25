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

#ifndef FPREFGENERAL_H
#define FPREFGENERAL_H

#include "formprefhijo.h"
#include "ui_FPrefGeneral.h"
#include "eventana.h"

/*!
 *	@brief Clase que maneja las preferencias de estilo e inicializacion
 *
 *	Clase que muestra las opciones de tipo de estilo, maximizacion y splash screen del programa
 *
 *	@author Esteban Zeller <tranfuga_25s@hotmail.com>
 */

class FPrefGeneral : public EVentana, public FormPrefHijo, private Ui::FPrefGeneralBase
{
  Q_OBJECT

public:
  FPrefGeneral( QWidget* parent = 0 );

public slots:
    void cargar();
    void guardar();
    void aplicar();

protected slots:
    void cambioSobreEstilo( bool estado );
    void cambioSobreEstilo( const QString &estilo );

};

#endif

