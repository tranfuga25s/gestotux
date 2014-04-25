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
#ifndef VPRODUCTOS_H
#define VPRODUCTOS_H

#include <evlista.h>

/**
	\brief  Listado de Productos

	Vista de los productos

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class VProductos : public EVLista
{
Q_OBJECT
public:
    VProductos(QWidget *parent = 0);

private slots:
    void antes_de_insertar(  int row, QSqlRecord &registro );

protected slots:
    void verCategorias();
    void agregar(  bool autoeliminarid = true );
    void listaVenta();
    void listaStock();
    void actualizar();
    void modificar();
    void eliminar();
    void habilitarProducto();
    void deshabilitarProducto();
    void mostrarDeshabilitados( bool );
    void mostrarOcultarCosto( bool );

private:
    QAction *ActHabilitar;
    QAction *ActDeshabilitar;
    QAction *ActCategorias;
    QAction *ActListadoVenta;
    QAction *ActListadoStock;
    QAction *ActVerDeshabilitados;
    QAction *ActVerCosto;

};

#endif
