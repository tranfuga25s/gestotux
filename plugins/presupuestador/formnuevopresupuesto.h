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

#ifndef FORMNUEVOPRESUPUESTO_H
#define FORMNUEVOPRESUPUESTO_H

#include "eventana.h"
#include "ui_FormNuevoPresupuestoBase.h"
class EEditor;
class EMAutos;

class FormNuevoPresupuesto : public EVentana, private Ui::FormNuevoPresupuestoBase
{
  Q_OBJECT

public:
  FormNuevoPresupuesto(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~FormNuevoPresupuesto();

protected slots:
    void agregar();

private:
	EEditor *editor;
	EMAutos *modeloautos;

private slots:
    void filtrarPorCliente( int indice );

signals:
    void agregarVentana( QWidget * );
};

#endif

