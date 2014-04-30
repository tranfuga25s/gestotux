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
#ifndef MPRODUCTOS_H
#define MPRODUCTOS_H

#include <QSqlRelationalTableModel>

/**
 * \brief Modelo de productos
 *
 * Clase que maneja el modelo de productos
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MProductos : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MProductos(QObject *parent = 0);
    QVariant data(const QModelIndex& item, int role) const;
    bool setData( const QModelIndex &item, const QVariant &value, int role = Qt::EditRole );
    Qt::ItemFlags flags(const QModelIndex &index) const;

    static bool agregarProducto( const QString codigo, const QString nombre, const double costo, const double venta, int stock = 0, int categoria = -1, QString descripcion = QString(), QString marca = QString(), QString modelo = QString() );

    static double stock( const int id_producto );
    static bool modificarStock( const int id_producto, const double cantidad );

    static bool actualizarPrecioCompra( const int id_producto, const double precio );
    static bool actualizarPrecioVenta( const int id_producto, const double precio );

    static double buscarPrecioCompra( const int id_producto );
    static double buscarPrecioVenta( const int id_producto );

    static double buscarPrecioCompra( const QString codigo );
    static double buscarPrecioVenta( const QString codigo );

    static bool existeCodigo( const QString codigo );
    static bool existeNombre( const QString nombre );

    static bool habilitado( const int id_producto );
    static bool habilitar( const int id_producto );
    static bool deshabilitar( const int id_producto );

    static bool remarcarFijo( const int id_producto, double cantidad );
    static bool remarcarPorcentaje( const int id_producto, double porcentaje );

    static QVector<int> idsSegunCategoria( const int id_categoria );

    bool habilitar( const QModelIndex idx );
    bool deshabilitar( const QModelIndex idx );

    bool tieneDatosRelacionados( const int id_producto );

    int idProductoPorCodigo( const QString codigo );

private:
    static bool cambiarHabilitado( const int id_producto, bool estado );
    bool cambiarHabilitado( const QModelIndex idx, bool estado );

};

#endif
