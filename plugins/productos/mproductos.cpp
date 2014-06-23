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
#include "mproductos.h"
#include "preferencias.h"
#include <QColor>
#include <QBrush>
#include <QDebug>

MProductos::MProductos(QObject *parent)
: QSqlRelationalTableModel(parent)
{
 setTable( "producto" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Categoria" );
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Productos" );
 if( p->value( "categorias" ).toBool() )
 {
  setRelation( 1, QSqlRelation( "categoria_producto", "id", "nombre" ) );
 }
 setHeaderData( 2, Qt::Horizontal, QString::fromUtf8( "#Código" ) );
 setHeaderData( 3, Qt::Horizontal, "Nombre" );
 setHeaderData( 4, Qt::Horizontal, "Precio de Costo" );
 setHeaderData( 5, Qt::Horizontal, "Precio de venta" );
 setHeaderData( 6, Qt::Horizontal, "Descripcion" );
 if( p->value( "marca_proveedor", false ).toBool() ) {
     setHeaderData( 7, Qt::Horizontal, "Proveedor" );
 } else {
    setHeaderData( 7, Qt::Horizontal, "Marca" );
 }
 setHeaderData( 8, Qt::Horizontal, "Stock" );
 setHeaderData( 9, Qt::Horizontal, "Habilitado" );
 setSort( 0, Qt::AscendingOrder );
 // Cantidad de decimales
 if( p->value("mostrar-decimales", false ).toBool() ) {
     this->_cantidad_decimales = p->value( "cantidad-decimales", 4 ).toInt();
 } else {
     this->_cantidad_decimales = 0;
 }
 p->endGroup();
 p->endGroup();
 p=0;
}


QVariant MProductos::data( const QModelIndex& item, int role ) const
{
 if( !item.isValid() )
 { return( QVariant() ); }
 switch( role )
 {
        case Qt::DisplayRole:
        {
                switch( item.column() )
                {
                        case 4:
                        case 5:
                        {
                                return QString( "$ %1" ).arg( QString::number( QSqlRelationalTableModel::data(item, role).toDouble(), 'f', 2 ) );
                                break;
                        }
                        case 9:
                        {
                                if( QSqlRelationalTableModel::data( item, role ).toBool() )
                                { return "Si"; }
                                else
                                { return "No"; }
                                break;
                        }
                        case 8:
                        {
                                return QString( "%L1" ).arg( QSqlRelationalTableModel::data( item, role ).toDouble(), 0, 'f', this->_cantidad_decimales );
                                break;
                        }
                        default:
                        {
                                return QSqlRelationalTableModel::data( item, role );
                                break;
                        }
                }
                break;
        }
        case Qt::TextColorRole:
        {
                switch ( item.column() )
                {
                        case 5:
                        case 4:
                        {
                                return QColor(Qt::blue);
                                break;
                        }
                        default:
                        {
                                return QColor(Qt::black);
                                break;
                        }
                }
                break;
        }
        case Qt::EditRole:
        {
                switch( item.column() )
                {
                        case 5:
                        case 4:
                        {
                                return QSqlRelationalTableModel::data( item, role ).toDouble();
                                break;
                        }
                        default:
                        {
                                return QSqlRelationalTableModel::data( item, role );
                                break;
                        }
                }
                break;
        }
        case Qt::TextAlignmentRole:
        {
                switch( item.column() )
                {
                        case 5:
                        case 4:
                        case 6:
                        case 7:
                        case 8:
                        {
                                return int( Qt::AlignHCenter | Qt::AlignVCenter );
                                break;
                        }
                        default:
                        {
                                return int( Qt::AlignLeft | Qt::AlignVCenter );
                                break;
                        }
                }
                break;
        }
        case Qt::ToolTipRole:
        case Qt::StatusTipRole:
        {
                return QVariant( "Haga doble click o seleccione y F2 para editar" );
                break;
        }
        case Qt::UserRole:
        {
                return QSqlTableModel::data( item, Qt::DisplayRole );
                break;
        }
        default:
        {
                return QSqlRelationalTableModel::data( item, role );
                break;
        }
 }
}

bool MProductos::setData(const QModelIndex &item, const QVariant &value, int role)
{
    if( item.column() == 1 && role == Qt::UserRole ) {
        return QSqlTableModel::setData( item, value, Qt::EditRole );
    } else {
        return QSqlRelationalTableModel::setData( item, value, role );
    }
}

Qt::ItemFlags MProductos::flags(const QModelIndex &/*index*/) const
{
    return QFlags<Qt::ItemFlag>( !Qt::ItemIsEditable |  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
/*!
    \fn MProductos::stock( const int id_producto )
        Devuelve la cantidad de stock que existe de un producto si el control de stock esta habilitado
 */
double MProductos::stock( const int id_producto )
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Productos" );
 bool stock = p->value( "stock" ).toBool();
 p->endGroup();p->endGroup();p=0;
 if( !stock )
 { return 10000000; }
 if( id_producto <= 0 ) {
     qDebug( "Id de producto erroneo" );
     return -1.0;
 }
 QSqlQuery cola( QString( "SELECT stock FROM producto WHERE id = %1 LIMIT 1" ).arg( id_producto ) );
 if( cola.next() )
 {
  return cola.record().value(0).toDouble();
 }
 else
 {
  qWarning( "Error al intentar obtener el stock del producto" );
  qDebug() << cola.lastError().text();
  qDebug() << cola.executedQuery();
  return -1.0;
 }
}


/*!
    \fn MProductos::modificarStock( const int id_producto, const double cantidad )
    Ajusta el stock del producto segun la cantidad aplicada como parametro.
    Si la cantidad es negativa se disminuye el stock del producto.
    Si la cantidad es positiva se aumenta el stock del producto.
    \param id_producto Identificador del producto
    \param cantidad Cantidad a aplicar
    \return Verdadero si se pudo actualizar el stock
 */
bool MProductos::modificarStock( const int id_producto, const double cantidad )
{
 QSqlQuery cola( QString( "SELECT stock FROM producto WHERE id = %2" ).arg( id_producto ) );
 if( cola.next() )
 {
        double anterior = cola.record().value(0).toDouble();
        anterior += cantidad;
        if(  cola.exec( QString( "UPDATE producto SET stock = %1 WHERE id = %2" ).arg( anterior ).arg( id_producto ) ) )
        {
                //qDebug( "Stock actualizado correctamente" );
                return true;
        }
        else
        {
                qWarning( "Error al actualizar el stock del producto" );
                qDebug() << cola.lastError().text();
                qDebug() << cola.executedQuery();
                return false;
        }
 }
 else
 {
  qWarning( "Error al intentar actualizar el stock del producto solicitado" );
  qDebug() << cola.lastError().text();
  qDebug() << cola.executedQuery();
  return false;
 }
}

/*!
 * \fn MProductos::agregarProducto( const QString codigo, const QString nombre, const double costo, const double venta, int stock, int categoria, QString descripcion, QString marca, QString modelo )
 * Agrega un nuevo producto a la base de datos con los parametros pasados.
 * \param codigo Codigo del producto a agregar
 * \param nombre Nombre del producto a agregar
 * \param costo Costo del producto a agregar
 * \param venta Precio de venta del producto
 * \param stock Stock inicial del producto
 * \param categoria Categoría a la que pertenece el producto
 * \param descripcion Descripción del producto
 * \param marca Marca del producto
 * \param modelo Modelo del producto
 * \returns Verdadero si se pudo ingresar correctamente el producto a la base de datos, falso en caso contrario.
 */
int MProductos::agregarProducto(const QString codigo, const QString nombre, const double costo, const double venta, double stock, int categoria, QString descripcion, QString marca, QString modelo) {
    QSqlQuery cola;
    if( !cola.prepare( "INSERT INTO producto ( codigo, nombre, precio_costo, precio_venta, stock, id_categoria, descripcion, marca, modelo, habilitado ) VALUES( :codigo, :nombre, :precio_costo, :precio_venta, :stock, :categoria, :descripcion, :marca, :modelo, :habilitado )" ) ) {
        qDebug() <<  cola.lastError().text();
        return -1;
    }
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Productos" );
    bool pmarcas = p->value( "marcas", false ).toBool();
    bool pmodelo = p->value( "modelo", false ).toBool();
    bool pcategorias = p->value( "categorias", false ).toBool();
    bool pstock = p->value( "stock", false ).toBool();
    bool ocodigo = p->value( "ocultar_codigo", false ).toBool();
    p->endGroup(); p->endGroup(); p=0;
    if( ocodigo && ( codigo.isEmpty() || codigo == QString() ) ) {
        // busco el ultimo ID y le sumo un valor
        QSqlQuery cola2;
        if( !cola2.exec( "SELECT MAX( id ) FROM producto;" ) ) {
            qDebug() << "Error al intentar buscar el nuevo codigo";
            qDebug() << cola2.lastError().text();
            return -2;
        }
        if( cola2.next() ) {
            int cod = cola2.record().value(0).toInt() + 1;
            while( existeCodigo( QString::number( cod ) ) ) {
                cod++;
            }
            cola.bindValue( ":codigo", cod );
        } else {
            cola.bindValue( ":codigo", 1 );
        }
    } else {
        cola.bindValue( ":codigo", codigo );
    }
    cola.bindValue( ":nombre", nombre );
    if( descripcion == "" )
    { cola.bindValue( ":descripcion", QVariant() );  }
    else
    { cola.bindValue( ":descripcion", descripcion ); }

    if( marca == "" || !pmarcas )
    { cola.bindValue( ":marca", QVariant() ); }
    else
    { cola.bindValue( ":marca", marca ); }

    if( modelo == "" || !pmodelo )
    { cola.bindValue( ":modelo", QVariant() ); }
    else
    { cola.bindValue( ":modelo", modelo); }

    if( categoria == -1 || !pcategorias )
    { cola.bindValue( ":categoria", 0 ); } // Evita el problema con mostrar cuando hay una relacion
    else
    { cola.bindValue( ":categoria", categoria ); }

    if( stock ==  0 || !pstock ) {
        cola.bindValue( ":stock", QVariant() );
    } else {
        cola.bindValue( ":stock", stock );
    }

    if( costo == 0.0 ) {
        cola.bindValue( ":precio_costo", QVariant() );
    } else {
        cola.bindValue( ":precio_costo", costo );
    }

    cola.bindValue( ":precio_venta", venta );
    cola.bindValue( ":habilitado", true );
    if( cola.exec() ) {
        return cola.lastInsertId().toInt();
    } else {
        qWarning( "Error al intentar insertar el producto." );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
        return -1;
    }
}

/*!
    \fn MProductos::actualizarPrecioCompra( const int id_producto, const double precio )
    Actualiza el precio de costo del producto indicado.
    ATENCION: ¡ No actualiza el precio de venta !
    \param id_producto identificador del producto.
    \param precio Precio de costo a aplicar.
    \return Verdadero si se pudo actualizar, falso en caso contrario.
 */
bool MProductos::actualizarPrecioCompra( const int id_producto, const double precio )
{
 QSqlQuery cola;
 if( cola.exec( QString( "UPDATE producto SET precio_costo = %1 WHERE id = %2" ).arg( precio ).arg( id_producto ) ) )
 {
        //qDebug( "Precio de compra actualizado correctamente" );
        return true;
 }
 else
 {
  qWarning( "Error al intentar actualizar el precio de compra del producto solicitado" );
  qDebug() << cola.lastError().text();
  qDebug() << cola.lastQuery();
  return false;
 }
}

/*!
    \fn MProductos::actualizarPrecioVenta( const int id_producto, const double precio )
    Actualiza el precio de venta del producto indicado.
    \param id_producto identificador del producto.
    \param precio Precio de venta a aplicar.
    \return Verdadero si se pudo actualizar, falso en caso contrario.
 */
bool MProductos::actualizarPrecioVenta( const int id_producto, const double precio )
{
 QSqlQuery cola;
 if( cola.exec( QString( "UPDATE producto SET precio_venta = %1 WHERE id = %2" ).arg( precio ).arg( id_producto ) ) )
 {
        //qDebug( "Precio de venta actualizado correctamente" );
        return true;
 }
 else
 {
  qWarning( "Error al intentar actualizar el precio de venta del producto solicitado" );
  qDebug() << cola.lastError().text();
  qDebug() << cola.lastQuery();
  return false;
 }
}

/*!
    \fn MProductos::buscarPrecioCompra( const int id_producto )
    Busca el precio de compra del producto dado por id.
    \param id_producto Identificador del producto.
    \returns Precio de venta del producto o -1 si hubo error.
 */
double MProductos::buscarPrecioCompra( const int id_producto )
{
 if( id_producto <= 0 ) { return -2.0; }
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT precio_costo FROM producto WHERE id = %1" ).arg( id_producto ) ) )
 {
     if( cola.next() ) {
         return cola.record().value(0).toDouble();
     } else {
         qWarning() << "MProductos::buscarPrecioCompra()::Error al hacer next la cola de busqueda del precio de compra del producto solicitado";
     }
 }
 else
 {
  qWarning() << "MProductos::buscarPrecioCompra()::Error al ejecutar la cola de busqueda del precio de compra del producto solicitado";
 }
 qDebug() << cola.lastError().text();
 qDebug() << cola.lastQuery();
 return -1.0;
}

/*!
 * \brief MProductos::buscarPrecioVenta
 * Busca el precio de venta del producto pedido como parametro
 * \param id_producto Identificador Unico de producto
 * \return Precio de venta del producto o -1 en caso de error
 */
double MProductos::buscarPrecioVenta( const int id_producto )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT precio_venta FROM producto WHERE id = %1" ).arg( id_producto ) ) )
    {
        if( cola.next() ) {
            return cola.record().value(0).toDouble();
        } else {
            qWarning() << "Error al hacer next la cola de busqueda del precio de compra del producto solicitado";
        }
    }
    else
    {
     qWarning( "Error al ejecutar la cola de busqueda del precio de compra del producto solicitado" );
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return -1.0;
}

/*!
    \fn MProductos::buscarPrecioCompra( const QString codigo )
    Busca el precio de compra del producto dado por codigo del producto.
    \param codigo Codigo del producto.
    \returns Precio de venta del producto o -1 si hubo error o el codigo no existe.
 */
double MProductos::buscarPrecioCompra( const QString codigo )
{
    if( !MProductos::existeCodigo( codigo ) ) {
        return -1.0;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT precio_costo FROM producto WHERE codigo = '%1'" ).arg( codigo ) ) )
    {
        if( cola.next() ) {
            return cola.record().value(0).toDouble();
        } else {
            qWarning( "MProductos::buscarPrecioCompra()::Error al hacer next la cola de busqueda del precio de compra del producto solicitado - codigo" );
        }
    }
    else
    {
     qWarning( "Error al ejecutar la cola de busqueda del precio de compra del producto solicitado - codigo" );
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return -1.0;

}

/*!
 * \brief MProductos::buscarPrecioVenta
 * Devuelve el precio de venta del producto cuyo codigo es pasado como parametro.
 * \param codigo Codigo del producto buscado
 * \return Precio de venta del producto o -1 en caso de error o si el codigo no existe.
 */
double MProductos::buscarPrecioVenta( const QString codigo )
{
    if( !MProductos::existeCodigo( codigo ) ) {
        return -1.0;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT precio_venta FROM producto WHERE codigo = %1" ).arg( codigo ) ) )
    {
        if( cola.next() ) {
            return cola.record().value(0).toDouble();
        } else {
            qWarning( "Error al hacer next la cola de busqueda del precio de compra del producto solicitado - codigo" );
        }
    }
    else
    {
     qWarning( "Error al ejecutar la cola de busqueda del precio de compra del producto solicitado - codigo" );
    }
    qDebug() << cola.lastError().text();
    qDebug() << cola.lastQuery();
    return -1.0;
}

/*!
 * \fn MProductos::existeCodigo( const QString codigo )
 * Deuelve verdadero si existe un producto con el codigo pasado como parametro.
 * \param codigo Codigo a buscar
 */
bool MProductos::existeCodigo( const QString codigo )
{
    if( codigo.isNull() || codigo.isEmpty() ) {  return false;  }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id) FROM producto WHERE codigo = '%1'" ).arg( codigo ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        }
    } else {
        qDebug( "Error de ejecucion de la cola de averiguacion de codigo de producto" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MProductos::existeNombre( const QString nombre )
 * Devuelve verdadero si existe un producto con el nombre pasado como parametro
 * \param nombre Nombre del producto a buscar
 */
bool MProductos::existeNombre( const QString nombre )
{
    if( nombre.isNull() || nombre.isEmpty() ) {  return false;  }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id) FROM producto WHERE nombre = '%1' OR nombre = '%2'" ).arg( nombre ).arg( nombre.toUpper() ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        }
    } else {
        qDebug( "Error de ejecucion de la cola de averiguacion de codigo de producto" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MProductos::cambiarHabilitado( const int id_producto, bool estado )
 *
 */
bool MProductos::cambiarHabilitado( int id_producto, bool estado )
{
    QSqlQuery cola;
    if( cola.exec( QString( "UPDATE producto SET habilitado = %2 WHERE id = %1" ).arg( id_producto ).arg( estado ) ) ) {
        return true;
    } else {
        qDebug( "Error de ejecucion de la cola de cambio de estado de habilitacion del producto" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MProductos::habilitar( int id_producto )
 * Habilita el producto pasado como parametro
 * \param id_producto Identificador del producto
 * \returns Verdaderos si se pudo habilitar
 */
bool MProductos::habilitar( int id_producto )
{  return cambiarHabilitado( id_producto, true ); }

/*!
 * \fn MProductos::deshabilitar( int id_producto )
 * Deshabilita el producto pasado como parametro
 * \param id_producto Identificador del producto
 * \returns Verdaderos si se pudo deshabilitar
 */
bool MProductos::deshabilitar( int id_producto )
{  return cambiarHabilitado( id_producto, false ); }

/*!
 * \brief MProductos::remarcarFijo
 * Remarca el precio de venta de un producto según la cantidad fija especificada
 * \param id_producto Identificador del producto
 * \param cantidad Cantidad financiaera a agregar o quitar ( <0 )
 * \return verdadero si se pudo remarcar el precio del producto
 */
bool MProductos::remarcarFijo( const int id_producto, double cantidad )
{
    double precio_anterior = buscarPrecioVenta( id_producto );
    if( precio_anterior == -1.0 )
        return false;
    double precio_nuevo = precio_anterior + cantidad;
    return actualizarPrecioVenta( id_producto, precio_nuevo );
}

/*!
 * \brief MProductos::remarcarPorcentaje
 * Remarca el precio de venta de un producto según la cantidad porcentual
 * \param id_producto Identificador del producto
 * \param cantidad Cantidad a remarcar precio_final = precio_inicial * ( 1 + cantidad )
 * \return verdadero si se pudo remarcar el precio del producto
 */
bool MProductos::remarcarPorcentaje( const int id_producto, double porcentaje )
{
    double precio_anterior =  buscarPrecioVenta( id_producto );
    //qDebug() << QString( "Precio anterior: $%1" ).arg( precio_anterior );
    if( precio_anterior == -1.0 ) {
        qDebug( "Precio anterior incorrecto" );
        return false;
    }
    double precio_nuevo = precio_anterior * ( 1.0 + porcentaje );
    //qDebug() << QString( "Precio nuevo: $%1" ).arg( precio_nuevo );
    return actualizarPrecioVenta( id_producto, precio_nuevo );
}

/*!
 * \brief MProductos::idsSegunCategoria
 * Devuelve el listado de todos los productos que pertenecen a una categoria especifica
 * \param id_categoria Identificador de categoria
 * \return QVector<int> con el listado - Si no hay ningun producto estará vacio
 */
QVector<int> MProductos::idsSegunCategoria(const int id_categoria)
{
    QVector<int> retorno;
    retorno.clear();
    if( id_categoria < 0 ) {
        return retorno;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id FROM producto WHERE id_categoria = %1" ).arg( id_categoria ) ) ) {
        while( cola.next() ) {
            retorno.append( cola.record().value(0).toInt() );
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguacione de Ids de productos de una categoria";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return retorno;
}

/*!
 * \brief MProductos::idsSegunProveedor
 * Devuelve el listado de todos los productos que pertenecen a un proveedor especifica
 * \param id_categoria Identificador de proveedor
 * \return QVector<int> con el listado - Si no hay ningun producto estará vacio
 */
QVector<int> MProductos::idsSegunProveedor( const int id_proveedor )
{
    QVector<int> retorno;
    retorno.clear();
    if( id_proveedor <= 0 ) {
        return retorno;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id FROM producto  "
                            " WHERE id IN (  "
                            " SELECT DISTINCT( id_producto ) "
                            "  FROM compras_productos  "
                            "  WHERE id_compra IN (    "
                            "      SELECT id           "
                            "      FROM compras        "
                            "      WHERE id_proveedor = %1"
                            "  )"
                            " ) " ).arg( id_proveedor ) ) ) {
        while( cola.next() ) {
            retorno.append( cola.record().value(0).toInt() );
        }
    } else {
        qDebug() << "Error al ejecutar la cola de averiguacione de Ids de productos de un proveedor";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return retorno;
}

/**
 * @brief MProductos::tieneDatosRelacionados
 * Verifica que algún producto tenga datos relacionados
 * @param id_producto Identificador del producto.
 * @return Verdadero si tiene datos relacionados en la base de datos
 */
bool MProductos::tieneDatosRelacionados( const int id_producto )
{
     QSqlQuery cola;
     if( cola.exec( QString( "SELECT COUNT(id_producto) FROM compras_productos WHERE id_producto = %1" ).arg( id_producto ) ) ) {
         cola.next();
         if( cola.record().value(0).toInt() > 0 ) {
             qWarning( "El producto esta utilizado en una compra" );
             return true;
         }
     }
     if( cola.exec( QString( "SELECT COUNT(id_producto) FROM item_factura WHERE id_producto = %1" ).arg( id_producto ) ) ) {
         cola.next();
         if( cola.record().value(0).toInt() > 0 ) {
             qWarning( "El producto esta utilizado en una factura" );
             return true;
         }
     }
     if( cola.exec( QString( "SELECT COUNT(id_producto) FROM item_presupuesto WHERE id_producto = %1" ).arg( id_producto ) ) ) {
         cola.next();
         if( cola.record().value(0).toInt() > 0 ) {
             qWarning( "El producto esta utilizado en un presupuesto" );
             return true;
         }
     }
     if( cola.exec( QString( "SELECT COUNT(id_producto) FROM item_remito WHERE id_producto = %1" ).arg( id_producto ) ) ) {
         cola.next();
         if( cola.record().value(0).toInt() > 0 ) {
             qWarning( "El producto esta utilizado en un remito" );
             return true;
         }
     }
     return false;
}

/*!
 * \fn MProductos::habilitado( int id_producto )
 * Devuelve el estado de habilitacion del producto pasado como parametro
 * \param id_producto Identificador del producto
 * \returns Verdaderos si se encuentra habilitado
 */
bool MProductos::habilitado( int id_producto )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT habilitado FROM producto WHERE id = %1" ).arg( id_producto ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toBool();
        } else {
            qDebug( "Error de next de la cola de averiguacion de habilitacion de producto" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug( "Error de ejecucion de la cola de averiguacion de habilitacion de producto" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MProductos::cambiarHabilitado( const QModelIndex idx, bool estado )
 *
 */
bool MProductos::cambiarHabilitado( QModelIndex idx, bool estado )
{
    setData( index( idx.row(), fieldIndex( "habilitado" ) ), estado );
    return submit();
}

/*!
 * \fn MProductos::habilitar( QModelIndex idx )
 * Habilita el producto pasado como parametro
 * \param idx Identificador del producto
 * \returns Verdaderos si se pudo habilitar
 */
bool MProductos::habilitar( QModelIndex idx )
{  return cambiarHabilitado( idx, true ); }

/*!
 * \fn MProductos::deshabilitar( QModelIndex idx )
 * Deshabilita el producto pasado como parametro
 * \param idx Identificador del producto
 * \returns Verdaderos si se pudo deshabilitar
 */
bool MProductos::deshabilitar( QModelIndex idx )
{  return cambiarHabilitado( idx, false ); }

/*!
 * \fn MProductos::idProductoPorCodigo( const QString codigo )
 * Obtiene el ID de producto del codigo pasado como parametro
 * \param codigo Codigo del producto a buscar
 * \return ID del codigo o 0 si no existe
 */
int MProductos::idProductoPorCodigo( const QString codigo )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT id FROM producto WHERE codigo = '%1'" ).arg( codigo ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qDebug( "Error de next de la cola de averiguacion de id de producto x codigo" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug( "Error de ejecucion de la cola de averiguacion de id de producto x codigo" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return 0;
}
