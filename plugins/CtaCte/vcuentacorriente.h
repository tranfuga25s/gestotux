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
#ifndef VCUENTACORRIENTE_H
#define VCUENTACORRIENTE_H

#include <evlista.h>
#include "mcuentacorriente.h"

/**
        @author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class VCuentaCorriente : public EVLista
{
Q_OBJECT
public:
    VCuentaCorriente(QWidget *parent = 0);

public slots:
    void agregar( bool autoeliminarid );

protected:
    void menuContextual( const QModelIndex &indice, QMenu *menu );

signals:
    void emitirRecibo( int, QString, double );
    void mostrarRecibo( int );
    void mostrarFactura( int );
    void mostrarRemito( int );

protected slots:
    void modificarLimite();
    void darBaja();
    void verResumen();
    void mostrarDeudoras( bool estado );
    void verSuspendidas( bool estado );
    void suspenderDesuspender();
    void listadoDeudor();
    void listadoDeudorPDF();
    void recalcularSaldo();

private:
    MCuentaCorriente *crmodelo;
   /*!
    * Accion para modificar el limite de la cuenta corriente elegida
    */
    QAction *ActModificarLimite;
   /*!
    * Accion para recalcular manualmente el saldo de una cuenta corriente
    */
    QAction *ActRecalcularSaldo;
   /*!
    * Accion que permite filtrar solamente por las cuentas corrientes que tienen saldo positivo
    */
    QAction *ActVerDeudoras;
    /*!
     * Acción que muestra el resumen de una cuenta corriente especifica
     */
     QAction *ActResumen;
    /*!
     * Accion que permite suspender una cuenta corriente
     */
     QAction *ActSuspender;
    /*!
     * Accion que permite filtrar solamente por las cuentas corrientes suspendidas
     */
     QAction *ActVerSuspendidas;
    /*!
     * Accion que permite imprimir las cuentas corrientes con saldo deudor
     */
     QAction *ActListadoDeudor;
    /*!
     * Accion que permite pasar a pdf las cuentas corrientes con saldo deudor
     */
     QAction *ActListadoDeudorPDF;
};

#endif
