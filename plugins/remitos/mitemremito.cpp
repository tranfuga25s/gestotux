#include "mitemremito.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

MItemRemito::MItemRemito(QObject *parent) :
    QSqlRelationalTableModel(parent) {
    inicializar();
    relacionar();
    _orden = 1;
}

void MItemRemito::inicializar() {
  setTable( "item_remito" );
  setHeaderData( 0, Qt::Horizontal, "#Orden" );
  setHeaderData( 1, Qt::Horizontal, "#Factura" );
  setHeaderData( 2, Qt::Horizontal, "Cantidad" );
  setHeaderData( 3, Qt::Horizontal, "Item" );
  setHeaderData( 4, Qt::Horizontal, "Precio Unitario" );
  setHeaderData( 5, Qt::Horizontal, "#Producto" );
}

void MItemRemito::relacionar() {
  setRelation( 1, QSqlRelation( "remito", "id_remito", "serie" ) ); /// @TODO: Crear una vista para esto
}

/*!
 * \fn MItemRemigo::agregarItemFactura( const int id_venta, const double cantidad, const QString texto, const double precio_unitario, const int id_producto )
 * Agrega un item de factura para la venta indicada.
 * @param id_venta Clave foranea en la tabla factura.
 * @param cantidad Cantidad del item.
 * @param texto Texto que va a aparecer en el item de la factura.
 * @param precio_unitario Precio unitario del item de la factura.
 * @param id_producto Identificador del producto.
 * @returns Verdadero si se pudo agregar correctamente.
 */
bool MItemRemito::agregarItemRemito( const int id_venta, const double cantidad, const QString texto, const double precio_unitario, const int id_producto ) {
    QSqlQuery cola;
    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "SQLITE" ) {
        if( cola.exec( QString( "SELECT COUNT(id_remito) FROM remito WHERE id_remito = %1").arg( id_venta ) ) ) {
            if( cola.next() ) {
                if( cola.record().value(0).toInt() < 1 ) {
                    qDebug( "No existe la factura que se paso como parametro para el item de remito" );
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
    cola.prepare( "INSERT INTO item_remito( id_remito, cantidad, texto, precio_unitario, id_producto ) VALUES ( :id_venta, :cantidad, :texto, :precio_unitario, :id_producto );" );
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
        qDebug( "Error al intentar insertar valor de item de remito" );
        qDebug() << "Detalles: tipo: " << cola.lastError().type() << " errno: " << cola.lastError().number() << ", descripcion: " << cola.lastError().text();
        return false;
    }


}

/*!
 * \fn MItemRemito::cargarItems()
 * Selecciona los elementos que corresponden al remito pasado como parametro
 * \param id_remito Identificador del remito
 */
void MItemRemito::cargarItems(const int id_remito)
{
    this->setFilter( QString( " id_rmeito = %1" ).arg( id_remito ) );
    this->select();
}
