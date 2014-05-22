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
#include "mproductostotales.h"

#include "../plugins/productos/mproductos.h"

#include "preferencias.h"

#include <QColor>
#include <QSize>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

MProductosTotales::MProductosTotales(QObject *parent, ECBProductosModel *_m )
 : QAbstractTableModel(parent)
{
 // Inicializo los sistemas
 Total = 0.0;
 totalItems = 0.0;
 _calcularTotal = false;
 _buscarPrecio = false;
 cantidades = new QHash<int, double>();
 precio_unitario = new QHash<int, double>();
 subtotales = new QHash<int, double>();
 productos = new QHash<int, int>();
 texto_descuentos = new QHash<int, QString>();
 descuentos = new QHash<int, double>();
 if( _m != 0 ) {
    prods = _m;
 } else {
     prods = new ECBProductosModel( this );
     prods->inicializar();
 }
 cantidades->clear();
 _tipoPrecio = MProductosTotales::Venta;
 _admite_duplicados = false;
 _solo_lectura = false;
}

/*!
 * \brief MProductosTotales::~MProductosTotales
 */
MProductosTotales::~MProductosTotales()
{
    delete cantidades;
    delete precio_unitario;
    delete subtotales;
    delete productos;
    delete texto_descuentos;
    delete descuentos;
    cantidades = 0;
    precio_unitario = 0;
    subtotales = 0;
    productos = 0;
    prods = 0;
    texto_descuentos = 0;
    descuentos = 0;
}

/*!
 * \brief MProductosTotales::insertRow
 * \param row
 * \param parent
 * \return
 */
bool MProductosTotales::insertRow( int row, const QModelIndex& parent )
{
 if( row == -1 )
 { row = cantidades->size(); }
 beginInsertRows( parent, row, row );
 cantidades->insert( row, 0 );
 precio_unitario->insert( row, 0 );
 subtotales->insert( row, 0 );
 productos->insert( row, -1 );
 //qDebug( qPrintable( "Insertada fila: " + QString::number( row ) ) );
 endInsertRows();
 emit dataChanged( this->index( row, 0 ), this->index( row, this->columnCount() ) );
 emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() ) );
 return true;
}

/*!
 * \brief MProductosTotales::removeRow
 * \param row
 * \param parent
 * \return
 */
bool MProductosTotales::removeRow( int row, const QModelIndex& parent )
{
  //qDebug( QString( "Eliminando fila: %1" ).arg( row ).toLocal8Bit() );
  beginRemoveRows( parent, row, row );
  // Actualizo los indices de todos los que están después de la fila que estamos eliminando.
  for( int i = row; i < cantidades->count(); i++ ) {
      productos->      insert( i, productos->      value( i + 1 ) );
      precio_unitario->insert( i, precio_unitario->value( i + 1 ) );
      cantidades->     insert( i, cantidades->     value( i + 1 ) );
      subtotales->     insert( i, subtotales->     value( i + 1 ) );
  }
  // Las posiciones estan basadas en indice base 0
  productos->      remove( productos->size()-1       );
  precio_unitario->remove( precio_unitario->size()-1 );
  cantidades->     remove( cantidades->size()-1      );
  subtotales->     remove( subtotales->size()-1      );
  recalcularTotalItems();
  endRemoveRows();
  emit dataChanged( this->index( row, 0 ), this->index( this->rowCount(), this->columnCount() ) );
  return true;
}

/*!
 * \brief MProductosTotales::setData
 * \param index
 * \param value
 * \param role
 * \return
 */
bool MProductosTotales::setData(const QModelIndex& index, const QVariant& value, int role)
{
 if( !index.isValid() )
 {
   //qDebug( QString( "Indice invalido Dueños: col=%1, row=%2, role=%3").arg( index.column() ).arg( index.row() ).arg( role ).toLocal8Bit() );
   return false;
 }
 switch( role )
 {
        case Qt::EditRole:
        {
                switch( index.column() )
                {
                        // Cantidad
                        case 0:
                        {
                                // Veo si tengo que verificar el maximo posible y el producto es valido y no estamos en buscar precio de compra ( ingresando compra no controla stock )
                                preferencias *p = preferencias::getInstancia();
                                p->beginGroup( "Preferencias" );
                                p->beginGroup( "Productos" );
                                if( p->value( "stock", false ).toBool() ) {
                                    p->beginGroup( "Stock" );
                                    if( p->value( "limitar" ).toBool() &&
                                           this->data( this->index( index.row(), 1 ), Qt::EditRole ).toInt() == 0 &&
                                          _tipoPrecio != MProductosTotales::Costo )
                                    {
                                            // Busco si el stock actual menos la cantidad es <= 0
                                           qDebug() << "Stock del producto: " << MProductos::stock( productos->value( index.row() ) );
                                          if( ( MProductos::stock( productos->value( index.row() ) ) - value.toDouble() ) <= 0 )
                                         {
                                                   qWarning() << "No hay suficientes unidades del producto para vender la cantidad pedida";
                                                   qDebug() << "Stock del producto (cantidad): " << MProductos::stock( productos->value( index.row() ) );
                                                  return false;
                                         }
                                    }
                                    p->endGroup();
                                }
                                p->endGroup(); p->endGroup(); p = 0;
                                cantidades->insert( index.row(), value.toDouble() );
                                if( _calcularTotal )
                                {
                                        subtotales->insert( index.row(), precio_unitario->value( index.row() ) * value.toDouble() );
                                        recalcularTotalItems();
                                }
                                emit dataChanged( index , this->index( index.row(), 3 ) );
                                return true;
                                break;
                        }
                        // Producto
                        case 1:
                        {
                                //qDebug( qPrintable( QString( "insert: size: %1, index.row(): %2" ).arg( this->productos->size() ).arg( index.row() ) ) );
                                if( !value.isValid() ) { qDebug( "El valor es invalido!" ); return false; }
                                if( !prods->existeID( value.toInt() ) ) {
                                    qDebug( "Indice no encontrado en la lista de productos" );
                                    return false;
                                }
                                // Veo si tengo que verificar el maximo posible en stock, el producto es valido o estamos en modo compra
                                preferencias *p = preferencias::getInstancia();
                                p->beginGroup( "Preferencias" );
                                p->beginGroup( "Productos" );
                                if( p->value( "stock", false ).toBool() ) {
                                    p->beginGroup( "Stock" );
                                    if( p->value( "limitar" ).toBool()  &&
                                        this->data( this->index( index.row(), 1 ), Qt::EditRole ).toInt() == 0 &&
                                        _tipoPrecio != MProductosTotales::Costo )
                                    {
                                            // Busco si el stock actual menos la cantidad es <= 0
                                            if( ( MProductos::stock( productos->value( index.row() ) ) - this->data( this->index( index.row(), 0 ), Qt::EditRole ).toDouble() ) <= 0 )
                                            {
                                                    qWarning( "No hay suficientes unidades del producto para vender la cantidad pedida" );
                                                    qDebug() << "Stock del producto(producto): " << MProductos::stock( productos->value( index.row() ) );
                                                    return false;
                                            }
                                    }
                                    p->endGroup();
                                }
                                p->endGroup(); p->endGroup(); p = 0;
                                productos->insert( index.row(), value.toInt() );
                                //qDebug( QString( "Valor insertado en productos %1!").arg( value.toInt() ).toLocal8Bit() );
                                if( _buscarPrecio && value.toInt() > 0 )
                                {
                                        // Busco el precio de venta este producto
                                        double precio = 0.0;
                                        if( this->_tipoPrecio ==  MProductosTotales::Costo ) {
                                                precio = buscarPrecioCompra( value.toInt() );
                                        } else if( this->_tipoPrecio == MProductosTotales::Venta ){
                                                precio = buscarPrecioVenta( value.toInt() );
                                        }
                                        this->setData( this->index( index.row(), 2 ), QVariant::fromValue( precio ), Qt::EditRole );
                                        //qDebug( qPrintable( QString( "buscando precio para id: %1 en row %2" ).arg( value.toInt() ).arg( index.row() ) ) );
                                }
                                return true;
                                break;
                        }
                        // Precio Unitario
                        case 2:
                        {
                                precio_unitario->insert( index.row(), value.toDouble() );
                                if( _calcularTotal )
                                {
                                        subtotales->insert( index.row(), cantidades->value( index.row() ) * value.toDouble() );
                                        recalcularTotalItems();
                                }
                                emit dataChanged( index , this->index( index.row(), 3 ) );
                                return true;
                                break;
                        }

                        // Subtotal
                        case 3:
                        {
                                return true;
                        }
                        default:
                        {
                                return false;
                        }
                }
                break;
        }
        default:
        { return false; break; }
  }
}

/*!
 * \brief MProductosTotales::columnCount
 * \return
 */
int MProductosTotales::columnCount(const QModelIndex& /*parent*/) const
{
 if( _calcularTotal )
 { return 4; }
 else
 { return 3; }
}

/*!
 * \brief MProductosTotales::rowCount
 * \return
 */
int MProductosTotales::rowCount(const QModelIndex& /*parent*/) const
{
 if( _calcularTotal )
 { return cantidades->size() + descuentos->size() + 1; }
 else
 { return cantidades->size() + descuentos->size(); }
}

/*!
 * \brief MProductosTotales::flags
 * \param index
 * \return
 */
Qt::ItemFlags MProductosTotales::flags(const QModelIndex& index) const
{
 if( _solo_lectura ) {
  return QFlags<Qt::ItemFlag>( Qt::ItemIsSelectable | !Qt::ItemIsEditable | Qt::ItemIsEnabled );
 }
 if( index.row() >= this->cantidades->size() + descuentos->size() )
 {
  return QFlags<Qt::ItemFlag>(!Qt::ItemIsSelectable | !Qt::ItemIsEditable | Qt::ItemIsEnabled );
 }
 else if( index.row() >= this->cantidades->size() )
 {
     if( index.column() > 0 ) {
        return QFlags<Qt::ItemFlag>( Qt::ItemIsEditable |  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
     } else {
        return QFlags<Qt::ItemFlag>( !Qt::ItemIsEditable |  Qt::ItemIsSelectable | Qt::ItemIsEnabled );
     }
 }
 else
 {
  if( index.column() == 3  || index.column() == 1 )
  { return QFlags<Qt::ItemFlag>( !Qt::ItemIsEditable |  Qt::ItemIsSelectable | Qt::ItemIsEnabled ); }
  else
  { return QFlags<Qt::ItemFlag>( Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled ); }
 }
}

/*!
 * \brief MProductosTotales::data
 * \param idx
 * \param role
 * \return
 */
QVariant MProductosTotales::data(const QModelIndex& idx, int role) const
{
 if( !idx.isValid() )
 { return( QVariant() ); }
 ////////////////////////////////////////////////////////////////////
 // Fila de total
 if( idx.row() == this->rowCount()-1 && _calcularTotal )
 {
  switch( idx.column() )
  {
        case 2:
        {
                if( role == Qt::DisplayRole )
                {
                    return "Total:";
                } else if( role == Qt::TextAlignmentRole ) {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                } else {
                    return QVariant();
                }
                break;
        }
        case 3:
        {
                if( role == Qt::DisplayRole ) {
                    return QString( "$ %L1" ).arg( Total, 10, 'f', 2 );
                } else if( role == Qt::TextAlignmentRole ) {
                    return int( Qt::AlignRight | Qt::AlignVCenter );
                } else {
                    return QVariant();
                }
                break;
        }
        default:
        {
                return QVariant();
                break;
        }
  }
 } // Fin ultima fila
 ////////////////////////////////////////////////////////////////////////////////////////////////////
 // Descuentos
 else if( idx.row() > ( cantidades->size()-1 ) && !descuentos->isEmpty() ) {
     switch( idx.column() )
     {
           // Filacero esta vacia
           case 1:  // Fila del texto del subtotal
           {
                   if( role == Qt::DisplayRole || role == Qt::EditRole )
                   {
                        return texto_descuentos->value( idx.row() );
                   } else  if( role == Qt::TextAlignmentRole ) {
                        return int( Qt::AlignRight | Qt::AlignVCenter );
                   } else { return QVariant(); }
                   break;
           }
           case 2: // Porcentaje de descuento
           {
                   if( role == Qt::DisplayRole )
                   {
                       return QString( " %L1 %" ).arg( descuentos->value( idx.row() ) );
                   } else if( role == Qt::TextAlignmentRole ) {
                       return int( Qt::AlignCenter | Qt::AlignVCenter );
                   } else if( role == Qt::EditRole ) {
                       return descuentos->value( idx.row() );
                   } else {  return QVariant(); }
                   break;
           }
           case 3: // Sutotales ---> tambien se guardan en subtotales
           {
                   if( role == Qt::DisplayRole ) {
                       return QString( "$ %L1" ).arg( subtotales->value( idx.row() ), 10, 'f', 2 );
                   } else if( role == Qt::TextAlignmentRole ) {
                       return int( Qt::AlignRight | Qt::AlignVCenter );
                   } else {
                       return QVariant();
                   }
                   break;
           }
           default:
           {
                   return QVariant();
                   break;
           }
     }

 } // FIN DESCUENTOS
 else
 {

 switch( role )
 {
        case Qt::DisplayRole:
        {
                switch( idx.column() )
                {
                        // Cantidades
                        case 0:
                        {
                                return QString( "%L1" ).arg( cantidades->value( idx.row() ) );
                                break;
                        }
                        // Producto
                        case 1:
                        {
                                // Devuelvo el mapeo idfila->productos->prods
                                if( prods->existeID( productos->value( idx.row() ) ) ) {
                                    return prods->nombreProductoSegunID( productos->value( idx.row() ) );
                                } else {
                                    qDebug() << "No se encontro el articulo en el data. Row= " << idx.row() << ", indice=" << productos->value( idx.row());
                                    return prods->rowCount();/*" error al buscar el prod en prods ";*/
                                }
                                break;
                        }
                        // precio unitario
                        case 2:
                        {
                                return QString( "$ %L1" ).arg( precio_unitario->value( idx.row() ), 10, 'f', 2 );
                                break;
                        }

                        // Subtotal
                        case 3:
                        {
                                return QString( "$ %L1" ).arg( subtotales->value( idx.row() ), 10, 'f', 2 );
                                break;
                        }
                        default:
                        {
                                return QVariant();
                                break;
                        }
                }
                break;
        }
        case Qt::TextColorRole:
        {
                switch ( idx.column() )
                {
                        case 0:
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
                switch( idx.column() )
                {
                        // Cantidad
                        case 0:
                        {
                                return cantidades->value( idx.row() );
                                break;
                        }
                        //Producto
                        case 1:
                        {
                                // tengo que devolver el Id de producto de la lista de general
                                return productos->value( idx.row() );
                                // Si el item no existe, devuelve cero....esto proboca que no se verifique el stock si esta habilitado
                                break;
                        }
                        // precio unitario
                        case 2:
                        {
                                return precio_unitario->value( idx.row() );
                                break;
                        }

                        default:
                        {
                                return false;
                                break;
                        }
                }
                break;
        }
        case Qt::TextAlignmentRole:
        {
                switch ( idx.column() )
                {
                        case 0:
                        case 2:
                        {
                                return int( Qt::AlignHCenter | Qt::AlignVCenter );
                                break;
                        }
                        case 1:
                        {
                                return int( Qt::AlignLeft | Qt::AlignVCenter );
                                break;
                        }
                        default:
                        {
                                return int( Qt::AlignRight | Qt::AlignVCenter );
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
        default:
        { return QVariant(); break; }
 }
 }// Fin fila de total else
}

/*!
    \fn MProductosTotales::recalcularTotalItems()
 */
void MProductosTotales::recalcularTotalItems()
{
 totalItems = 0;
 for( QHash<int, double>::const_iterator i = subtotales->constBegin(); i != subtotales->constEnd(); ++i )
 {
  totalItems += i.value();
 }
 // Emito la senal de que cambio el valor
 emit dataChanged( this->index( this->cantidades->size(), 0 ), this->index( this->cantidades->size(), 3 ) );
 recalcularTotal();
}

/*!
 * \brief MProductosTotales::recalcularTotal
 */
void MProductosTotales::recalcularTotal()
{
    Total = totalItems;
    int n = cantidades->size();
    // Recalculo todos los subtotales según los descuentos
    for( QHash<int, double>::const_iterator i = descuentos->constBegin(); i != descuentos->constEnd(); ++i ) {
        n++;
        Total *= ( 1 - ( i.value() / 100 ) );
        subtotales->insert( n, Total );
    }
    emit dataChanged( this->index( cantidades->size()+1, 4 ), index( rowCount(), columnCount() ) );
}

/*!
    \fn MProductosTotales::total()
    Devuelve el total de los items incluyendo descuentos si existen
 */
double MProductosTotales::total()
{ return Total; }

/*!
    \fn MProductosTotales::totalitems()
    Devuelve el total de los items incluyendo descuentos si existen
 */
double MProductosTotales::totalitems()
{ return totalItems; }

/*!
    \fn MProductosTotales::headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
 */
QVariant MProductosTotales::headerData ( int section, Qt::Orientation orientation, int role ) const
{
 if( orientation == Qt::Horizontal  &&  role == Qt::DisplayRole )
 {
  switch( section )
  {
        case 0:
        { return "Cantidad"; break; }
        case 1:
        { return "Producto"; break; }
        case 2:
        { return "Precio Unitario"; break; }
        case 3:
        { return "Subtotal"; break; }
        default:
        { return section; }
  }
 }
 else
 {
  return QAbstractTableModel::headerData( section, orientation, role );
 }
}

/*!
 * \fn MProductosTotales::agregarItem( const int cant, const QString texto, const double pu )
 * Funcion especial que ingresara los elementos sin hacer las verificaciones normales como si fuera un agregado desde la lista de ventas.
 * Util para mostrar elementos con subtotales y totales en listas estaticas para mostrar por ejemplo elementos de compras.
 * \param cant Cantidad del item.
 * \param texto Texto del item.
 * \param pu Precio unitario del item
 */
void MProductosTotales::agregarItem( const int cant, const QString texto, double pu )
{
    int pos = this->cantidades->size();
    this->insertRow( -1 );

    this->cantidades->insert( pos, cant );
    this->precio_unitario->insert( pos, pu );
    this->subtotales->insert( pos, cant * pu );

    // inserto el texto en la lista de nombre de productos
    int pos2 = this->prods->agregarItem( texto );
    // inserto el indice de lo anterior en el mapa de productos
    this->productos->insert( pos, pos2 );

    if( _calcularTotal )
        recalcularTotalItems(); recalcularTotal();


    emit dataChanged( this->index( pos, 0 ), this->index( pos, this->columnCount() ) );
    emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() ) );
}


/*!
 * \fn MProductosTotales::agregarItem( const int cant, const int id_producto, const double pu )
 * Funcion especial que ingresara los elementos sin hacer las verificaciones normales como si fuera un agregado desde la lista de ventas.
 * Util para mostrar elementos con subtotales y totales en listas estaticas para mostrar por ejemplo elementos de compras.
 * \param cant Cantidad del item.
 * \param id_producto Identificador del producto
 * \param pu Precio unitario del item
 */
void MProductosTotales::agregarItem( const int cant, const int id_producto, double pu )
{
    int pos = this->cantidades->size();
    this->insertRow( -1 );

    this->cantidades->insert( pos, cant );
    this->precio_unitario->insert( pos, pu );
    this->subtotales->insert( pos, cant * pu );

    // inserto el indice de lo anterior en el mapa de productos
    this->productos->insert( pos, id_producto );

    if( _calcularTotal )
        recalcularTotalItems(); recalcularTotal();

    emit dataChanged( this->index( pos, 0 ), this->index( pos, this->columnCount() ) );
    emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() ) );
}

/*!
    \fn MProductosTotales::calcularTotales( bool sino  )
    Setea la propiedad de que se caclulen o no los totales.
    Este valor debe de ser seteado antes de asignar el modelo a una vista, ya que modifica la cantidad de columnas.
 */
void MProductosTotales::calcularTotales( bool sino  )
{ _calcularTotal = sino; }


/*!
    \fn MProductosTotales::buscaPrecios()
 */
bool MProductosTotales::buscaPrecios()
{ return _buscarPrecio; }


/*!
    \fn MProductosTotales::buscarPrecios( bool activado )
 */
void MProductosTotales::buscarPrecios( bool activado )
{ _buscarPrecio = activado; /*qDebug( "busqueda de precios activada" );*/ }

/*!
    \fn MProductosTotales::buscarPrecioCompra( int id_producto )
    Busca el precio de costo del producto
 */
double MProductosTotales::buscarPrecioCompra( int id_producto )
{
 if( id_producto > 0 ) {
     QSqlQuery cola;
     if( cola.exec( QString( "SELECT precio_costo FROM producto WHERE id = %1 LIMIT 1" ).arg( id_producto ) ) )
     {
      cola.next();
      return cola.record().value(0).toDouble();
     }
     else
     {
      qDebug() << "No se encontro el precio de compra";
      return 0.0;
     }
  } else {
      // El precio se va a ingresar a mano, porque no esta en la base de datos de productos
     return 0.0;
  }
}

/*!
    \fn MProductosTotales::buscarPrecioVenta( int id_producto )
    Busca el precio de costo del producto
 */
double MProductosTotales::buscarPrecioVenta( int id_producto )
{
 if( id_producto > 0 ) {
     QSqlQuery cola;
     if( cola.exec( QString( "SELECT precio_venta FROM producto WHERE id = %1 LIMIT 1" ).arg( id_producto ) ) )
     {
      cola.next();
      return cola.record().value(0).toDouble();
     }
     else
     {
      qDebug() << "No se encontro el precio de compra";
      return 0.0;
     }
  } else {
      // El precio se va a ingresar a mano, porque no esta en la base de datos de productos
     return 0.0;
  }
}

#include <QInputDialog>
#include "einputdialog.h"
/*!
 * \brief MProductosTotales::agregarNuevoProducto
 * \param cantidad
 * \param Id
 * \param precio_unitario
 */
void MProductosTotales::agregarNuevoProducto( int cantidad, int Id, double precio_unitario )
{
  // Veo si existe y lo agrego a la lista si no existe....
  bool ok = false;

  if( Id > 0 ) {
      if( this->_buscarPrecio ) {
          if( this->_tipoPrecio == MProductosTotales::Costo ) {
            precio_unitario = buscarPrecioCompra( Id );
          } else {
            precio_unitario = buscarPrecioVenta( Id );
          }
          ok = true;
      }
      // Es un producto valido
      preferencias *p = preferencias::getInstancia();
      p->beginGroup( "Preferencias" );
      p->beginGroup( "Productos" );
      if( p->value( "stock", false ).toBool() && _tipoPrecio != MProductosTotales::Costo ) {
        if( ( MProductos::stock( Id ) - cantidad ) < 0 ) {
              qDebug( "-> Error, stock negativo" );
              qWarning( "-> El stock de este producto es insuficiente para la cantidad que intenta vender." );
              return;
        }
      }
      p->endGroup(); p->endGroup(); p=0;
  }

  if( precio_unitario <= 0.0 ) {
      // Como no busca el precio, inserto el dialogo
      precio_unitario = EInputDialog::getImporte( 0, "Falta precio", "Ingrese el precio unitario", 0.0, 0.0, 2147483647, 2, &ok );
  } else {
      // Ingreso el precio antes
      ok = true;
  }

  // Inserto el dato con la cantidad si fue buscado el precio o insertado
  if( ok ) {
     // Verifico si no existe ya el producto
      if( !_admite_duplicados ) {
        //qWarning( "Buscando productos iguales" );
        // Busco si existe en la lista
          int pos = this->productos->values().indexOf( Id );
          if( pos != -1 ) {
              // El producto ya existe.... Solo actualizo la cantidad
              // Segun la documentacion this->productos->values() y this->productos->keys() tienen el mismo orden
              int id_fila = this->productos->keys().at( pos );
              double cant = this->cantidades->value( id_fila );
              double ncant = cant + cantidad;
              this->cantidades->insert( id_fila, ncant );
              this->subtotales->insert( id_fila, ncant * precio_unitario );

              // Actualizo el subtotal
              emit dataChanged( this->index( id_fila, 0 ), this->index( id_fila, this->columnCount() ) );

              if( _calcularTotal )
                  recalcularTotalItems();
                  emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() )  );

              return;
          }
      }
    if( this->insertRow( -1 ) ) {

        int id_fila = this->rowCount()-1;

        if( _calcularTotal )
            id_fila--;

        this->cantidades->insert     ( id_fila, cantidad                   );
        this->productos->insert      ( id_fila, Id                         );
        this->precio_unitario->insert( id_fila, precio_unitario            );
        this->subtotales->insert     ( id_fila, precio_unitario * cantidad );

        recalcularTotalItems();

        emit dataChanged( this->index( id_fila, 0 ), this->index( id_fila, this->columnCount() ) );

        if( _calcularTotal )
            emit dataChanged( this->index( this->rowCount(), 0 ), this->index( this->rowCount(), this->columnCount() )  );

    } else {
        // error al insertar el articulo
        qDebug( "Error al insertar la fila al ingresar un producto nuevo" );
        return;
    }
  } else {
        // No quiso ingresar un precio unitario
      qDebug( "No quiso ingresar el precio unitario. No ingreso el producto" );
      return;
  }
  return;
}

/*!
 * \brief MProductosTotales::setearTipoPrecioBuscar
 * \param t
 */
void MProductosTotales::setearTipoPrecioBuscar( int t )
{
    if( t == MProductosTotales::Costo ) {
        this->_tipoPrecio = MProductosTotales::Costo;
    } else if( t == MProductosTotales::Venta ) {
        this->_tipoPrecio = MProductosTotales::Venta;
    }
}

/*!
 * \brief MProductosTotales::tipoPrecioBuscar
 * \return
 */
int  MProductosTotales::tipoPrecioBuscar()
{ return this->_tipoPrecio; }

/*!
 * \brief MProductosTotales::agregarDescuento
 * \param texto
 * \param porcentaje
 */
void MProductosTotales::agregarDescuento( QString texto, double porcentaje )
{
    if( texto_descuentos->values().contains( texto ) ) {
        qWarning( "El descuento ya existe" );
        return;
    }
    int pos = cantidades->size()+descuentos->size();
    emit beginInsertRows( QModelIndex(), pos, pos );

    texto_descuentos->insert( pos, texto );
    descuentos->insert( pos, porcentaje );

    // Poner el subtotal en subtotales
    double ant = 0.0;
    if( descuentos->size() == 1 ) {
        ant = totalItems;
    } else {
        ant = subtotales->value( subtotales->size() - 1 );
    }
    double sub = ant * ( 1 - ( porcentaje / 100 ) );
    subtotales->insert( pos, sub  );

    // Calcular el total
    Total = sub;

    emit endInsertRows();
    emit dataChanged( index( cantidades->size()-1, 0 ), index( rowCount(), columnCount() ) );
    recalcularTotal();
}

/*!
 * \brief MProductosTotales::esDescuento
 * \param idx
 * \return
 */
bool MProductosTotales::esDescuento( QModelIndex idx )
{
    if( !idx.isValid() )
        return false;

    if( (idx.row()-cantidades->size()) <= descuentos->size() ) {
        return true;
    } else {
        return false;
    }
}

/*!
 * \brief MProductosTotales::eliminarDescuento
 * \param idx
 * \return
 */
bool MProductosTotales::eliminarDescuento( QModelIndex idx )
{
    if( !idx.isValid() )
        return false;
    if( !esDescuento( idx ) ) {
        qWarning( "Lo que estás intentando eliminar no es un descuento!" );
        return false;
    }

    beginRemoveRows( QModelIndex(), idx.row(), idx.row() );

    // Actualizo los datos siguientes al que elimino
    for( int i = idx.row(); i < idx.row()+descuentos->count(); i++ ) {
        descuentos      ->insert( i, descuentos      ->value( i+1 ) );
        texto_descuentos->insert( i, texto_descuentos->value( i+1 ) );
    }
    descuentos      ->remove( cantidades->size() + descuentos->size() - 1 );
    texto_descuentos->remove( cantidades->size() + texto_descuentos->size() - 1 );

    // Actualizo el subtotales
    for( int i = idx.row(); i < subtotales->count(); i++ ) {
        subtotales->insert( i, subtotales->value( i + 1 ) );
    }

    // Las posiciones estan basadas en indice base 0
    subtotales->remove( subtotales->size() - 1 );

    endRemoveRows();
    emit dataChanged( this->index( idx.row(), 0 ), this->index( this->rowCount(), this->columnCount() ) );

    recalcularTotal();
    return true;
}

/*!
 * \brief MProductosTotales::setearListaProductos
 * \param _mapa_id_prod
 */
void MProductosTotales::setearListaProductos( ECBProductosModel *_m )
{
    this->prods = _m;
}

/*!
 * \brief MProductosTotales::vaciarProductos
 * Elimina todos los productos que hay en el listado.
 */
void MProductosTotales::vaciarProductos()
{
    while( this->cantidades->size() > 0 ) {
        this->removeRow( 0 );
    }
}
