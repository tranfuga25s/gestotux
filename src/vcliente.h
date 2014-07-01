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
#ifndef VCLIENTE_H
#define VCLIENTE_H

#include <evlista.h>
class MClientes;
class QModelIndex;

/**
 * @brief Vista de la lista de Clientes
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class VCliente : public EVLista
{
  Q_OBJECT
public:
    VCliente( QWidget *parent = 0 );

protected slots:
    void agregar( bool autoeliminarid = true);
    void modificar();
    void eliminar();
    void listadoClientes();
    void mostrarTodos();
    void menuContextual(const QModelIndex &indice, QMenu *menu);

private slots:
    void modificarIndice( QModelIndex m );

private:
    MClientes *mc;

};

#endif
