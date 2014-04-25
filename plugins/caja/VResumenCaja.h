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

#ifndef VRESUMENCAJA_H
#define VRESUMENCAJA_H

#include "evlista.h"
class ECBTabla;
class MCajas;
class MMovimientosCaja;
class QDateEdit;
class QGroupBox;
class QLabel;
class QLineEdit;

class VResumenCaja : public EVLista
{
    Q_OBJECT
public:
    VResumenCaja( QWidget *parent = 0 );
    void setearCaja( int id_caja );

public slots:
    void cambioCaja( int id_caja );
    void retiro();
    void ingreso();
    void transferir();

protected slots:
    void imprimir();
    void aPdf();
    void filtrar();
    void actualizarFiltro();
    void hacerCierre();

private:
    ECBTabla *CBCajas;
    MMovimientosCaja *modelo;
    MCajas *cmodelo;
    QAction *ActFiltrar;
    QGroupBox *GBFiltrado;
    QDateEdit *DTEInicio;
    QDateEdit *DTEFin;
    QAction *ActCierre;
    QLabel *LSaldo;
    QLabel *LTexto;
    QLineEdit *LETexto;
};

#endif // VRESUMENCAJA_H
