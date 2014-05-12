/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller   *
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

#ifndef FORMAGREGARCOMPRA_H
#define FORMAGREGARCOMPRA_H

#include "eventana.h"
#include "ui_FormAgregarCompraBase.h"
class MProveedor;
class MCompra;
class MProductosTotales;

/*!
 * @brief Formulario que permite agregar una compra
 */
class FormAgregarCompra : public EVentana, public Ui::FormAgregarCompraBase
{
  Q_OBJECT

public:
  FormAgregarCompra( MCompra *m = 0, QWidget* parent = 0  );

signals:
  void actualizarVista();

private:
  MCompra *modelo;
  MProductosTotales *mcp;

protected slots:
    void guardar();
    void agregarProducto();
    void eliminarProducto();
    void arreglarProductoAgregado( int anterior, int nuevo );
    void cambioProveedor( int id_proveedor );
};

#endif

