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
#ifndef MCOMPRA_H
#define MCOMPRA_H

#include <QSqlRelationalTableModel>

/**
 * \brief Modelo de compra
 *
 * Clase que maneja el modelo de las compras
 *  \author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MCompra : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MCompra( QObject *parent = 0, bool relaciones = false );
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex &index, int role ) const;
    void inicializar();
    void relacionar();

    bool agregarCompra( QVariant fecha, QVariant proveedor, double total, bool contado );
    bool modificarCompra( const int id_compra, QDate fecha, int proveedor, double total, bool contado, QModelIndex indice );
    int ultimoId();
    bool eliminarCompra( int id_compra );

    static QDate obtenerFechaMinimaCompra();
    static QDate obtenerFechaMaximaCompra();


};

#endif
