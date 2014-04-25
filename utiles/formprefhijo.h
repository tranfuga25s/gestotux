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
#ifndef FORMPREFHIJO_H
#define FORMPREFHIJO_H
/**
 * \brief Interfaz para los forms de configuracion
 *
 * Interfaz que contiene los metodos necesrios y basicos para que un formulario pueda ser usado en el
 * cuadro de configuraciones
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormPrefHijo
{
public:
    virtual void aplicar() = 0;
    virtual void cargar() = 0;
    virtual void guardar() = 0;

};

#endif
