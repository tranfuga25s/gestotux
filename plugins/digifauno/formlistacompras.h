/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller   *
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

#ifndef FORMLISTACOMPRAS_H
#define FORMLISTACOMPRAS_H

#include "eventana.h"
#include "ui_FormListaComprasBase.h"

class MCompra;

class FormListaCompras : public EVentana, private Ui::FormListaComprasBase
{
  Q_OBJECT

public:
  FormListaCompras( QWidget* parent = 0  );
  ~FormListaCompras();

  protected:
    MCompra * modelo;

protected slots:
    void cambioEstadoFecha( int estado );
    void cambioEstadoProveedor( int estado );
    void cambioEstadoProducto( int estado );
    void cambioEstadoGrupo( bool estado );
    void cambioIndiceProveedor( int indice );
    void cambioIndiceProducto( int indice );
    void cambioFecha( const QDate &fecha );
    void borrar();

private:
    void crear_filtro();
};

#endif

