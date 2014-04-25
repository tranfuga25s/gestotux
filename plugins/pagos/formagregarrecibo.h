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

#ifndef FORMAGREGARRECIBO_H
#define FORMAGREGARRECIBO_H

#include "eventana.h"
#include "ui_FormReciboBase.h"

#include "mpagos.h"
/*!
 * \brief Formulario para agregar Recibo
 *
 * Formulario con el codigo de validación necesario para ingresar un nuevo recibo en el sistema
 */
class FormAgregarRecibo : public EVentana, private Ui::FormReciboBase
{
Q_OBJECT

public:
        FormAgregarRecibo ( QWidget* parent = 0, Qt::WFlags fl = 0 );
        void setearModelo( MPagos *m );
        void setearDatos( const int id_cliente, const QString texto, const double cantidad );

public slots:
        void guardar();

protected slots:
        void cambioPagado( double valor );
        void cambioCliente( int id_combo );

private:
        void recalcularTotal();

        MPagos *_modelo;

        bool _hicomp;
        bool _ctacte_habilitada;
};

#endif

