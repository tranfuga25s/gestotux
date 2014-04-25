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
#ifndef EINFORMEIMPRESORA_H
#define EINFORMEIMPRESORA_H

#include <QPrinter>

/**
\brief Clase de impresora de informes

Clase que permite que se pueda usar el formulario de configuracion de impresora para informes cada vez.
Como mecesita una impresora en el constructor, en este constructor se inicializa una impresora con las
caracteristicas de las preferencias para el formulario. Tambien se usara en la impresion y el formulario
de previsualizacion.

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class EInformeImpresora : public QPrinter
{
public:
    EInformeImpresora();
    ~EInformeImpresora();
    void guardar();

};

#endif
