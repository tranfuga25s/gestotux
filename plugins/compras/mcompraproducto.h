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
#ifndef MCOMPRAPRODUCTO_H
#define MCOMPRAPRODUCTO_H

#include <QSqlRelationalTableModel>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MCompraProducto : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MCompraProducto( QObject *parent = 0 );
    bool agregarCompraProducto( const int id_compra, const int id_producto, const double precio_compra, const int cantidad, bool stock_habilitado );
    void setearId( const int id_compra ) { _id_compra = id_compra; setearFiltro(); }


private:
    int _id_compra;
    void setearFiltro();
};

#endif
