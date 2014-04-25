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

#ifndef FORMMOVIMIENTO_H
#define FORMMOVIMIENTO_H

#include "eventana.h"
#include "ui_FormMovimientoBase.h"
#include "TipoMovs.h"
class QAction;
class MCaravanaDueno;
class EActCerrar;

class FormMovimiento : public EVentana, public Ui::FormMovimientoBase
{
  Q_OBJECT

public:
  FormMovimiento(QWidget* parent = 0, Qt::WFlags fl = 0, int accion = invalido );
  ~FormMovimiento();
    void setearNumeroTri();
    virtual void hacerInformeSenasa();


protected:
	EActCerrar *ActCerrar;
	QAction *ActGuardar;
	MCaravanaDueno *model;
	bool _especial;


protected slots:
    virtual void cerrar();
    void eliminarCaravana();
    void agregarCaravana();
    virtual void guardar() = 0;

public slots:
    void agregarCategoria();
    void agregarCliente();
    void agregarEstablecimientoOrigen();
    void agregarEstablecimientoDestino();

protected slots:
    void cargarDesdeArchivo();
    bool verificar();
    void habilitarCantidadAnimales( QString categoria );

private:
	int _accion;
};

#endif

