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

#ifndef FORMAGREGARVENTA_H
#define FORMAGREGARVENTA_H

#include "eventana.h"
#include "ui_FormAgregarVentaBase.h"
#include "../cuotas/mplancuota.h"
class MProductosTotales;

/*!
 * \brief Formulario para agregar una venta
 *
 * Clase que permite agregar una nueva venta al sistema
 * \author Esteban Zeller
 */
class FormAgregarVenta : public EVentana, private Ui::FormAgregarVentaBase
{
Q_OBJECT
public:
        FormAgregarVenta ( QWidget* parent = 0, Qt::WFlags fl = 0 );
        void setearCliente( int id_cliente );
        void setearFecha( QDate fecha );
        void setearItems( MProductosTotales *m );

signals:
        /*!
         * \brief emitirPlanCuota
         * \param id_cliente Identificador de Cliente
         * \param total Total del plan
         */
        void emitirPlanCuota( int, double, MPlanCuota::TipoComprobante );
        /*!
         * \brief emitirPlanCuotaSetIdFactura
         * \param id_cliente Identificador del plan de cuota
         * \param id_factura Identificador de la factura
         */
        void emitirPlanCuotaSetIdFactura( int, int );
        /*!
         * \brief Actualiza el listado de ventas
         */
        void actualizarListado();
        /*!
         * \brief emitir Garantia
         * \param
         * \param
         * \param
         * \param
         */
        void emitirGarantia( int, int, QString, int );


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
        void cambioProveedor( int id_proveedor );

private:
        MProductosTotales *mcp;
        int id_plan_cuota;

};

#endif

