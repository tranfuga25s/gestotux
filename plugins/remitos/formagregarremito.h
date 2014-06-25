/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
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

#ifndef FORMAGREGARREMITO_H
#define FORMAGREGARREMITO_H

#include "eventana.h"
#include "ui_FormAgregarRemitoBase.h"
#include "../cuotas/mplancuota.h"
class MProductosTotales;

/*!
 * \brief Formulario para agregar una venta
 *
 * Clase que permite agregar una nueva venta al sistema
 * \author Esteban Zeller
 */
class FormAgregarRemito : public EVentana, public Ui::FormAgregarRemitoBase
{
Q_OBJECT
public:
    FormAgregarRemito( QWidget* parent = 0, Qt::WFlags fl = 0 );
    void setearCliente( int id_cliente );
    void setearFecha( QDate fecha );
    void setearItems( MProductosTotales *m );

public slots:
    void setearIdPlanCuota( int id_cuota );

protected slots:
    void agregarProducto();
    void eliminarProducto();
    void guardar();
    void cambioCliente( int id_combo );
    void eliminarTodo();
    void agregarDescuento();
    void eliminarDescuento();

signals:
    void emitirPlanCuota( int, double, MPlanCuota::TipoComprobante );
    void emitirPlanCuotaSetIdFactura( int, int );
    void actualizarListado();

private:
    MProductosTotales *mcp;
    int id_plan_cuota;
};

#endif

