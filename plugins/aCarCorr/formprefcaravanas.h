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
#ifndef FORMPREFCARAVANAS_H
#define FORMPREFCARAVANAS_H

#include "formprefhijo.h"
#include "ui_FormPrefCaravanas.h"
#include "eventana.h"
class MPrefCategorias;

/**
 * \brief Formulario de configuracion de categorias especificas
 *
 * Formulario que permite definir las categorias especificas de caravanas que no tienen numero de caravanas
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormPrefCaravanas : public EVentana, public FormPrefHijo, public Ui::FormPrefCaravanas
{
Q_OBJECT
public:
    FormPrefCaravanas(QWidget *parent = 0);
    ~FormPrefCaravanas();

public slots:
    void cargar();
    void guardar();
    void aplicar();

private:
    MPrefCategorias *modelo;
};

#endif
