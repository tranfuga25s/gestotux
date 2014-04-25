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
#ifndef MCLIENTES_H
#define MCLIENTES_H

#include <QSqlRelationalTableModel>

/**
 * @brief Modelo de Clientes
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MClientes : public QSqlRelationalTableModel
{
public:
    MClientes( QObject *parent = 0 );

    Qt::ItemFlags flags( const QModelIndex &index ) const;

    static QString direccionEntera( int id_cliente );
    static QString getRazonSocial( const int id_cliente );

    static bool existe( QString razon_social, QString nombre );
    static bool tieneDatosRelacionados( const int id_cliente );

    static int agregarClientePredeterminado( const QString razon_social,
                                             QString nombre = QString(),
                                             QString apellido = QString(),
                                             QString direccion = QString(),
                                             QString telefono = QString(),
                                             int inscripcion = -1 );


};

#endif
