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
#include "mcompraproducto.h"
#include <QSqlRecord>
#include <QDebug>

MCompraProducto::MCompraProducto(QObject *parent)
 : QSqlRelationalTableModel(parent)
{
 setTable( "compras_productos" );
 setHeaderData( 0, Qt::Horizontal, "#item" );
 setHeaderData( 1, Qt::Horizontal, "#Compra" );
 setHeaderData( 2, Qt::Horizontal, "#Producto" );
 setHeaderData( 3, Qt::Horizontal, "Precio Compra" );
 setHeaderData( 4, Qt::Horizontal, "Cantidad" );
}

/*
  CREATE TABLE `compras_productos` (
    `id` int(1) NOT NULL auto_increment,
    `id_compra` int(1) NOT NULL,
    `id_producto` int(1) NOT NULL,
    `precio_compra` decimal(20,3) NOT NULL,
    `cantidad` decimal(20,3) NOT NULL,
    PRIMARY KEY  (`id`)
 ) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;
 */
#include <QSqlError>
#include <QSqlQuery>
#include "mproductos.h"
#include "preferencias.h"
/*!
 * \fn MCompraProducto::agregarCompraProducto( const int id_compra, const int id_producto, const double precio_compra, const int cantidad )
 * Agrega la compra de un producto para una ceirta compra.
 * \param id_compra Identificador de compra
 * \param id_producto Identificador del producto
 * \param precio_compra Precio al cual se lo compro
 * \param cantidad Cantidad comprada
 * \param stock_habilitado Verifica que se esté utilizando el sistema de ontrol de stock
 * \return Verdader si se pudo registrar
 */
bool MCompraProducto::agregarCompraProducto( const int id_compra, const int id_producto, const double precio_compra, const int cantidad, bool stock_habilitado )
{
    if( id_compra <= 0 )
        return false;
    if( id_producto <= 0 )
        return false;
    if( cantidad <= 0 )
        return false;
    if( precio_compra <= 0 )
        return false;

    // Veo si existe el producto ( recordar que viene del mproductostotales )
    if( id_producto > 0 ) {
        // El producto existe -  Ajusto el stock
        if( stock_habilitado ) {
            if( !MProductos::modificarStock( id_producto, cantidad )  ) {
                qWarning( "Existió un error al intentar ajustar el stock del producto" );
                return false;
            }
        }
        // Actualizo el precio de compra
        if( !MProductos::actualizarPrecioCompra( id_producto, cantidad ) ) {
            qWarning( "Existió un error al intentar ajustar el precio de compra del producto" );
            return false;
        }
        // Registro la compra
        QSqlQuery cola;
        if( !cola.prepare( "INSERT INTO compras_productos( id_compra, id_producto, precio_compra, cantidad ) VALUES ( :id_compra, :id_producto, :precio_compra, :cantidad )" ) ) {
            qWarning( "Error al preparar la cola de inserción del registro de compra de un producto especifico" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
            return false;
        }
        cola.bindValue( ":id_compra", id_compra );
        cola.bindValue( ":id_producto", id_producto );
        cola.bindValue( ":precio_compra", precio_compra );
        cola.bindValue( ":cantidad", cantidad );
        if( !cola.exec() )  {
            qWarning( "Error al insertar los datos de la compra de un producto" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
            return false;
        }
        return true;
    }
    return false;
}

void MCompraProducto::setearFiltro()
{
    if( _id_compra > 0 ) {
        this->setFilter( QString( "id_compra = %1" ).arg( _id_compra ) );
    } else {
        this->setFilter( "" );
    }
    this->select();
}
