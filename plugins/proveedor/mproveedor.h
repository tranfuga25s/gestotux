/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller & Daniel Sequeira		   *
 *   juiraze@yahoo.com.ar  - daniels@hotmail.com			   *
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
#ifndef MPROVEEDOR_H
#define MPROVEEDOR_H

#include <QSqlTableModel>

/**
Modelo para mantener los datos de los proveedores

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MProveedor : public QSqlTableModel
{
Q_OBJECT
public:
    MProveedor( QObject *parent = 0 );
    QVariant data(const QModelIndex& idx, int role) const;
    static bool tieneDatosRelacionados( const int id_proveedor );
    static bool existenProveedores();
    static bool existeProveedor( QString razon_social );
    static bool existeCuitCuil( QString cuit );
};

#endif
