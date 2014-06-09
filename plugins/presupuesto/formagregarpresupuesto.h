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

#ifndef FORMAGREGARPRESUPUESTO_H
#define FORMAGREGARPRESUPUESTO_H

#include "eventana.h"
#include "ui_FormPresupuestoBase.h"

class QSqlQueryModel;
class MProductosTotales;
class ECBProductosModel;

/*!
 * \brief Formulario para agregar un presupuesto
 *
 * Clase que permite agregar un nuevo presupuesto al sistema.
 * \author Esteban Zeller
 */
class FormAgregarPresupuesto : public EVentana, private Ui::FormPresupuestoBase
{
  Q_OBJECT

public:
  FormAgregarPresupuesto(QWidget* parent = 0, Qt::WFlags fl = 0 );

protected slots:
    void cancelar();
    void guardar( bool cerrar );
    void guardar();
    void agregarProducto();
    void eliminarProducto();
    void borrarTodoProducto();
    void cambioCliente( int id_combo );
    void agregarDescuento();
    void eliminarDescuento();

private:
   /*!
    * Modelo para mostrar la lista de productos e items
    */
    MProductosTotales *m;
    ECBProductosModel *ecbmprod;
    ECBProductosFilter *ecbfiltro;
};

#endif

