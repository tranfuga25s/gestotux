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
#ifndef MPRODUCTOSTOTALES_H
#define MPRODUCTOSTOTALES_H


#include <QAbstractTableModel>

#include "ecbproductosmodel.h"

/**
 * \brief Modelo que calcula totales segun modelo de venta, compra y presupuesto
 *
 * Modelo que calcula totales segun modelo de venta, compra y presupuesto
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MProductosTotales : public QAbstractTableModel
{
Q_OBJECT
public:
    MProductosTotales(QObject *parent = 0, ECBProductosModel *_m = 0 );
    ~MProductosTotales();

    bool insertRow(int row, const QModelIndex& parent = QModelIndex() );
    bool removeRow(int row, const QModelIndex& parent = QModelIndex() );
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    int columnCount(const QModelIndex& parent = QModelIndex() ) const;
    int rowCount(const QModelIndex& parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex& idx, int role) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    void agregarItem( const double cant, const QString texto, double pu );
    void agregarItem( const double cant, const int id_producto, double pu );

    double totalitems();
    double total();
    void calcularTotales( bool sino = true );

    bool buscaPrecios();
    void buscarPrecios( bool activado = true );

    void agregarNuevoProducto( const double cantidad, const int Id, double precio_unitario = -1.1 );
   /*!
    * \enum TipoPrecio
    * Indica que tipo de precio se buscara cuando se realize una busqueda
    */
    enum TipoPrecio {
        Costo = 0, /** Busqueda del precio de costo del producto */
        Venta = 1  /** Busqueda del precio de venta del producto */
    };
    void setearTipoPrecioBuscar( int t );
    int tipoPrecioBuscar();

    bool aceptaDuplicados() { return _admite_duplicados; }
    void setearAceptaDuplicados( bool param ) { _admite_duplicados = param; }

    void setearSoloLectura( bool param ) { _solo_lectura = param; }

    bool esDescuento( QModelIndex idx );
    bool eliminarDescuento( QModelIndex idx );
    int conteoDescuentos() { return descuentos->size(); }
    int conteoItems() { return cantidades->size(); }

    void setearListaProductos( ECBProductosModel *_m );
    ECBProductosModel *listaProductos() { return prods; }

    void vaciarProductos();

    void arreglarIdProductoAgregado( const int anterior, const int nuevo );

public slots:
    void agregarDescuento( QString texto, double porcentaje );

private:
        /*!
         * Contiene el listado de subtotales por definicion de fila
         */
        QHash<int, double> *subtotales;
        /*!
         * Contiene el listado de cantidades por definicion de fila
         */
        QHash<int, double> *cantidades;
        /*!
         * Contiene el listado de precios unitarios por definicion de fila
         */
        QHash<int, double> *precio_unitario;
        /*!
         * Contiene el mapeo de posicion de fila a id de productos
         */
        QHash<int, int> *productos;
        /*!
         * Contiene el listado de los productos en un modelo
         */
        ECBProductosModel *prods;
        /*!
         * Contiene el listado de subtotales por definicion de fila
         */
        QHash<int, double> *descuentos;
        /*!
         * Contiene el listado de subtotales por definicion de fila
         */
        QHash<int, QString> *texto_descuentos;
        /*!
         * Contiene el total de la factura calculado
         */
        double Total;
        /*!
         * Contiene el sutotal antes de los descuentos
         */
        double totalItems;
        /*!
         * Define si se calculará el total
         */
        bool _calcularTotal;
        /*!
         * Define si se hará la busqueda de precio del producto
         */
        bool _buscarPrecio;
        /*!
         * Define si permite que existan duplicados de los items dentro del producto
         */
        bool _admite_duplicados;
       /*!
        * Define si se permite edicion de los items
        */
        bool _solo_lectura;
        /*!
         * Define que tipo de precio Costo o Venta se buscará
         */
        TipoPrecio _tipoPrecio;

        double buscarPrecioVenta( int id_producto );
        double buscarPrecioCompra( int id_producto );
        void recalcularTotalItems();
        void recalcularTotal();
};

#endif
