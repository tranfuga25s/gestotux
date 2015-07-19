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

#ifndef DPAGARRECIBO_H
#define DPAGARRECIBO_H

#include "ui_DPagarReciboBase.h"
#include "NumeroComprobante.h"

/*!
 * \brief Clase para ingresar recibos a pagar
 *
 * Clase para el dialogo que permite ingresar un recibo emitido pero no pagado para marcarlo como pagado
 */
class DPagarRecibo : public QDialog, public Ui::DPagarRecibo
{
    Q_OBJECT

public:
    explicit DPagarRecibo( QWidget *parent = 0 );
    bool setearIdRecibo( const int id_recibo );

public slots:
    void accept();
    void cambioNumeroRecibo();
    void cambioAPagar( double cantidad );

private:
    NumeroComprobante _num_recibo;

};

#endif // DPAGARRECIBO_H
