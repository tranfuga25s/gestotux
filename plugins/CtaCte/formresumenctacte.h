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

#ifndef FORMRESUMENCTACTE_H
#define FORMRESUMENCTACTE_H

#include "eventana.h"
#include "ui_FormResumenCtaCteBase.h"
class MItemCuentaCorriente;

/*!
 * \brief Formulario de listado de cuenta corriente
 *
 *
 * \author Esteban Zeller
 */
class FormResumenCtaCte : public EVentana, private Ui::FormResumenCtaCteBase
{
Q_OBJECT

public:
	FormResumenCtaCte ( QWidget* parent = 0, Qt::WFlags fl = 0 );
        void setNumeroCuenta( const QString &numero_cuenta );

protected slots:
    void cambioCtaCte( int numero_cuenta );
    void imprimir();
    void aPdf();
    void menuContextual( const QModelIndex &indice );
    void pagarTodo();
    void verFactura();
    void verRecibo();
    void verRemito();
    void pagarFactura();

protected:
    MItemCuentaCorriente *modeloItem;

private slots:
    void filtrar();

signals:
    void emitirRecibo( int id_cliente, QString texto_recibo, double total );
    void mostrarRecibo( int id_recibo );
    void mostrarFactura( int id_factura );
    void mostrarRemito( int id_remito );

private:
    QAction *ActFiltrar;
    QString _numero_cuenta;
    QString _filtro;

};

#endif

