#include "mitemfactura.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

MItemFactura::MItemFactura(QObject *parent) :
    QSqlRelationalTableModel(parent) {
    inicializar();
    relacionar();
    _orden = 1;
}

/*
CREATE TABLE IF NOT EXISTS `item_factura` (
  `id_item_factura` bigint NOT NULL,
  `id_factura` bigint REFERENCES factura(id_factura),
  `cantidad` double NOT NULL,
  `texto` text NOT NULL,
  `precio_unitario` double NOT NULL,
  `id_producto` bigint NULL,
  PRIMARY KEY (`id_item_factura`, `id_factura` )
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;
*/

void MItemFactura::inicializar() {
  setTable( "item_factura" );
  setHeaderData( 0, Qt::Horizontal, "#Orden" );
  setHeaderData( 1, Qt::Horizontal, "#Factura" );
  setHeaderData( 2, Qt::Horizontal, "Cantidad" );
  setHeaderData( 3, Qt::Horizontal, "Item" );
  setHeaderData( 4, Qt::Horizontal, "Precio Unitario" );
  setHeaderData( 5, Qt::Horizontal, "#Producto" );
}

void MItemFactura::relacionar() {
  setRelation( 1, QSqlRelation( "factura", "id_factura", "serie" ) ); ///@TODO: Crear una vista para esto
}

/*!
 * \fn MItemFactura::agregarItemFactura( const int id_venta, const double cantidad, const QString texto, const double precio_unitario, const int id_producto )
 * Agrega un item de factura para la venta indicada.
 * @param id_venta Clave foranea en la tabla factura.
 * @param cantidad Cantidad del item.
 * @param texto Texto que va a aparecer en el item de la factura.
 * @param precio_unitario Precio unitario del item de la factura.
 * @param id_producto Identificador del producto.
 * @returns Verdadero si se pudo agregar correctamente.
 */
bool MItemFactura::agregarItemFactura( const int id_venta, const double cantidad, const QString texto, const double precio_unitario, const int id_producto ) {
    if( id_venta == 0 || cantidad == 0 || texto.isEmpty() ) {
        qWarning( "Datos incorrectos al intentar guardar el item de venta." );
        qDebug() << "id_venta = " << id_venta;
        qDebug() << "cantidad: " << cantidad;
        qDebug() << "Texto:" << texto;
        return false;
    }
    QSqlQuery cola;
    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "SQLITE" ) {
        if( cola.exec( QString( "SELECT COUNT(id_factura) FROM factura WHERE id_factura = %1").arg( id_venta ) ) ) {
            if( cola.next() ) {
                if( cola.record().value(0).toInt() < 1 ) {
                    qDebug( "No existe la factura que se paso como parametro para el item de factura" );
                    return false;
                }
            } else {
                qDebug( "Error al hacer next en la verificacion de que existe la factura para el item de factura" );
                qDebug() << cola.lastError().text();
                qDebug() << cola.lastQuery();
                return false;
            }
        } else {
            qDebug( "Error al hacer exec en la verificacion de que existe la factura para el item de factura" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
            return false;
        }
    }
    cola.prepare( "INSERT INTO item_factura( id_factura, cantidad, texto, precio_unitario, id_producto ) VALUES ( :id_venta, :cantidad, :texto, :precio_unitario, :id_producto );" );
    cola.bindValue( ":id_venta", id_venta );
    cola.bindValue( ":cantidad", cantidad );
    cola.bindValue( ":texto", texto );
    cola.bindValue( ":precio_unitario", precio_unitario );
    if( id_producto <= 0 ) {
        cola.bindValue( ":id_producto", QString() );
    } else {
        cola.bindValue( ":id_producto", id_producto );
    }
    if( cola.exec() ) {
        _orden++;
        return true;
    } else {
        qDebug( "Error al intentar insertar valor de item de factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return false;
    }


}

#include "mproductos.h"
#include "preferencias.h"
/*!
 * \fn MItemFactura::anularItemsFactura( const int id_factura )
 * Funcion encargada de poner en orden los elementos de la factura cuando esta es anulada. Basicameente restituye el stock si está relacionado con un producto.
 * \param id_factura ID de la factura que se desea anular
 * \return Verdadero si se realizaron los procesos correctamente
 */
bool MItemFactura::anularItemsFactura( const int id_factura )
{
    if( !preferencias::getInstancia()->value( "Preferencias/Productos/stock" ).toBool() ) {
        // No hay control de stock no pasa nada
        return true;
    }
    // La transacción viene desde la factura
    QSqlQuery cola;
    if( !cola.exec( QString( "SELECT id_producto, cantidad FROM item_factura WHERE id_factura = %1 AND id_producto IS NOT NULL" ).arg( id_factura ) ) ) {
        qDebug() << "Error al ejecutar la cola de averiguacion de IDs de productos de una factura para anular";
        qDebug() << cola.lastError().text();
        return false;
    }
    while( cola.next() ) {
        int id_producto = cola.record().value(0).toInt();
        int cantidad = cola.record().value(1).toDouble();
        // Se coloca la cantidad comprada porque hay que aumentar el stock - issue #70
        if( !MProductos::modificarStock( id_producto, cantidad ) ) {
            qDebug() << "Error al actualizar el stock del producto " << id_producto;
            return false;
        }
    }
    return true;
}
