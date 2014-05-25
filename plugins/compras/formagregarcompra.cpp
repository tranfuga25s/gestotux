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
#include "formagregarcompra.h"

#include "../proveedor/mproveedor.h"
#include "eactcerrar.h"
#include "eactguardar.h"
#include "mcompra.h"
#include "mproductostotales.h"
#include "dproductostotales.h"
#include "mcompraproducto.h"
#include "eregistroplugins.h"
#include "mproductos.h"
#include "formagregarproducto.h"
#include "preferencias.h"
#include "einputdialog.h"

#include <QMessageBox>
#include <QTableView>
#include <QCompleter>
#include <QDate>
#include <QtSql>
#include <QLineEdit>
#include <QInputDialog>


FormAgregarCompra::FormAgregarCompra( MCompra *m, QWidget* parent )
: EVentana( parent ), Ui::FormAgregarCompraBase()
{
    setupUi(this);
    setObjectName( "agregar_compra" );
    setWindowTitle( "Agregar nueva compra" );
    setWindowIcon( QIcon( ":/imagenes/agregar_compras.png" ) );

    if( m == 0 ) {
      this->modelo = new MCompra( this, false );
    } else {  this->modelo = m; }

    DEFecha->setMaximumDate( QDate::currentDate() );
    DEFecha->setDate( QDate::currentDate() );

    PBAgregarProducto->setIcon( QIcon( ":/imagenes/add.png" ) );
    PBEliminarProducto->setIcon( QIcon( ":/imagenes/eliminar.png" ) );

    connect( PBAgregarProducto, SIGNAL( clicked() ), this, SLOT( agregarProducto() ) );
    connect( PBEliminarProducto, SIGNAL( clicked() ), this, SLOT( eliminarProducto() ) );

    // Rellenar los items de productos
    connect( CBProducto, SIGNAL( agregarProducto() ), PBAgregarProducto, SIGNAL( clicked() ) );

    ecbmproducto = new ECBProductosModel( this );
    ecbmproducto->inicializar();

    CBProducto->setearListado( ecbmproducto );

    mcp = new MProductosTotales( this, ecbmproducto );
    mcp->calcularTotales( true );
    mcp->buscarPrecios( true );
    mcp->setearTipoPrecioBuscar( MProductosTotales::Costo );
    TVLista->setModel( mcp );
    TVLista->setAlternatingRowColors( true );
    TVLista->setItemDelegate( new DProductosTotales( TVLista ) );
    TVLista->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVLista->horizontalHeader()->setResizeMode( QHeaderView::Stretch );

    this->addAction( new EActGuardar( this ) );
    this->addAction( new EActCerrar( this ) );

    if( !ERegistroPlugins::getInstancia()->existePluginExterno( "caja" ) ) {
        RBContado->setEnabled( false );
        RBOtro->setChecked( true );
        RBContado->setVisible( false );
        RBOtro->setVisible( false );
        LFormaPago->setVisible( false );
    }

    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Ventas" );
    if( p->value("filtrarProveedor").toBool() ) {
        connect( CBProveedor, SIGNAL( cambioIdProveedor( int ) ), this, SLOT( cambioProveedor( int ) ) );
    }
    p->endGroup();
    p->endGroup();
    p=0;

    DSBPrecioUnitario->setVisible( false );

    CBProducto->setearMostrarSinStock( true );

    _id_proveedor_anterior = -1;
}


/*!
    \fn FormAgregarCompra::guardar()
 */
void FormAgregarCompra::guardar()
{
 if( CBProveedor->currentIndex() == -1 )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese un proveedor para esta compra" );
  return;
 }
 if( !DEFecha->date().isValid() )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese una fecha valida para esta compra" );
  return;
 }
 if( !RBContado->isChecked() && !RBOtro->isChecked() ) {
     QMessageBox::warning( this, "Faltan Datos", "Por favor, ingrese una forma de pago para la compra" );
     return;
 }
 mcp->calcularTotales( false );
 if( mcp->rowCount() < 1 )
 {
  QMessageBox::warning( this, "Faltan Datos" , "Por favor, ingrese productos para esta compra" );
  mcp->calcularTotales( true );
  return;
 }
 //Inicio una transaccion
 QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).transaction();
 //seteo el modelo para que no calcule totales y subtotales
 mcp->calcularTotales( false );
 // Genero la compra
 MCompra *compra = new MCompra( this, false );
 if( compra->agregarCompra( DEFecha->date(), CBProveedor->idProveedorActual(), mcp->total(), RBContado->isChecked() ) == false )
 {
     QSqlDatabase::database().rollback();
     mcp->calcularTotales( true );
     return;
 }
 // Busco el ultimo id de compra
 int id_compra = compra->ultimoId();

 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Compras" );
 bool auto_agregar_productos = p->value( "auto-agregar-productos", false ).toBool();
 p->endGroup();
 p->beginGroup( "Productos" );
 bool marca_proveedor = p->value( "marca_proveedor", false ).toBool();
 double ganancia = p->value( "ganancia", 10.0 ).toDouble();
 p->endGroup();
 p->endGroup();
 p=0;

 // recorro el modelo y guardo los datos
 MCompraProducto *m = new MCompraProducto( this );
 bool siATodo = false;
 bool noATodo = false;
 bool parar = false;
 for( int i= 0; i<mcp->rowCount(); i++ )
 {
     if( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt() <= -1 ) {
         // El producto no existe
         // Pregunto si lo quiere agregar
         int ret = -1;
         if( noATodo )
             // No quiere agregar ningun producto, siempre salto al siguiente
             continue;
         if( siATodo || auto_agregar_productos )
                 ret = QMessageBox::Yes;
            else
                ret = QMessageBox::question( this,
                                             QString::fromUtf8( "¿Agregar?" ),
                                             QString::fromUtf8( "Desea agregar el producto %1?" ).arg( mcp->data( mcp->index( i, 1 ), Qt::DisplayRole ).toString() ),
                                             "Si", "No" );
         switch( ret ) {
            case QMessageBox::YesToAll:
            {
                siATodo = true;
                // No pongo break para que agrege el producto
                parar = true;
            }
            case QMessageBox::Ok:
            case QMessageBox::Yes:
            case 0:
            {
                // Agrego el producto
                 QString marca = QString();
                 if( marca_proveedor ) { // Veo si decidió utilizar marca como proveedor
                     marca = CBProveedor->currentText();
                 }
                 if( !auto_agregar_productos ) {
                    FormAgregarProducto *f = new FormAgregarProducto();
                    f->setearNombre( mcp->data( mcp->index( i, 1 ), Qt::DisplayRole ).toString() );
                    //f->setearStockInicial( mcp->data( mcp->index( i, 0 ), Qt::EditRole ).toInt() );
                    f->setearStockInicial( 0 );
                    f->setearPrecioCosto( mcp->data( mcp->index( i, 2 ), Qt::EditRole ).toDouble() );
                    f->setearNumeroAnterior( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt() );
                    f->setearDesdeCompra( true );
                    f->setearProveedor( marca );
                    connect( f, SIGNAL( agregarProducto( int, int ) ), this, SLOT( arreglarProductoAgregado( int, int ) ) );
                    emit agregarVentana( f );
                    parar = true;

                 } else {
                     MProductos *mp = new MProductos();
                     double precio_compra = mcp->data( mcp->index( i, 2 ), Qt::EditRole ).toDouble();
                     double precio_venta = precio_compra * ( 1.0 + ( ganancia / 100.0 ) );
                     int id_producto_nuevo = mp->agregarProducto(
                                 QString(),
                                 mcp->data( mcp->index( i, 1 ), Qt::DisplayRole ).toString(), // Nombre
                                 precio_compra, // Costo
                                 precio_venta, // Venta
                                 0, // Stock inicial cero
                                 -1, // Categoria indefinida
                                 QString(), // Descripcion vacía
                                 marca ); // Marca designada
                     if( id_producto_nuevo > 0 ) {
                         arreglarProductoAgregado( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt(),
                                                   id_producto_nuevo );
                        i--; // vuelvo a repetir el proceso para que continue con los datos actualizados
                        //qDebug() << "Producto agregado automaticamente" << id_producto_nuevo;
                        continue;
                     } else {
                         qWarning() << "Error al insertar el nuevo producto. Hagalo manualmente"; abort();
                     }
                     delete mp;
                }
                break;
            }
            case QMessageBox::NoToAll:
            {
                noATodo = true;
                break;
            }
            case QMessageBox::No:
            default:
            {
                // Desconocido, salteo al siguiente producto
                continue;
                break;
            }

         }
     } else {
         // El MCompraItem ajusta automaticamente el precio de compra pero no el de venta.
         double precio_anterior = MProductos::buscarPrecioCompra( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt() );
         if( precio_anterior > 0.0 ) {
             if( ( precio_anterior - mcp->data( mcp->index( i, 2 ), Qt::EditRole ).toDouble() ) != 0.0 ) {
                 // Actualizo el precio de venta
                 preferencias *p = preferencias::getInstancia();
                 p->beginGroup( "Preferencias" );
                 p->beginGroup( "Productos" );
                 double ganancia = ( p->value( "ganancia", 10 ).toDouble() ) / 100.0;
                 p->endGroup();p->endGroup(); p = 0;
                 double precio_calculado = precio_anterior * ( 1 + ganancia );
                 bool ok = false;
                 double precio_venta = EInputDialog::getImporte( this,
                                                                "Nuevo precio de venta",
                                                                QString( "Ingrese el nuevo precio de venta para %1:" ).arg( mcp->data( mcp->index( i, 1 ), Qt::DisplayRole ).toString() ),
                                                                precio_calculado,
                                                                0.01, 2147483647, 2, &ok );
                 if( ok ) {
                    if( !MProductos::actualizarPrecioVenta( mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt(), precio_venta ) ) {
                         qWarning() << "No se pudo actualizar el precio de venta del producto "
                                    << mcp->data( mcp->index( i, 1 ), Qt::DisplayRole ).toString();
                    }
                 }
             }
         }
     }
     if( parar ) {
         // Paro el agregar para que le de tiempo al cliente para llenar los datos del producto
         QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
         return;
     }
     preferencias *p = preferencias::getInstancia();
     p->inicio();
     p->beginGroup( "Preferencias" );
     p->beginGroup( "Productos" );
     bool stock = p->value( "stock", false ).toBool();
     p->endGroup(); p->endGroup(); p=0;
     if( !m->agregarCompraProducto( id_compra,
                                   mcp->data( mcp->index( i, 1 ), Qt::EditRole ).toInt(), // id_producto
                                   mcp->data( mcp->index( i, 2 ), Qt::EditRole ).toDouble(), // precio compra
                                   mcp->data( mcp->index( i, 0 ), Qt::EditRole ).toInt(), // cantidad
                                   stock ) ) {

         qWarning() << "FormAgregarCompra::guardar()::No se pudo agregar el producto a la compra";
         QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).rollback();
         mcp->calcularTotales( true );
         return;
     }

  } // fin del for
  // Si llegue hasta aca sin problema, hago el submit
  // listo
  if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).commit() )
  {
   if( this->isVisible() )
   { QMessageBox::information( this, "Correcto" , "La compra se ha registrado correctamente" ); }
   emit actualizarVista();
   this->close();
   return;
  }
  else
  {
   QMessageBox::information( this, "Incorrecto" , "La compra no se pudo guardar correctamente" );
   return;
  }
}


/*!
    \fn FormAgregarCompra::agregarProducto()
 */
void FormAgregarCompra::agregarProducto()
{

 if( SBCant->value() == 0 )
 { QMessageBox::information( this, "Error de datos", "La cantidad a agregar debe ser mayor que cero", QMessageBox::Ok ); return; }

 if( CBProducto->currentText().isEmpty() )
 { QMessageBox::information( this, "Error de datos", "Ingrese un producto a agregar", QMessageBox::Ok ); return; }

 /*if( DSBPrecioUnitario->value() <= 0.0 )
 { QMessageBox::Information( this, "Error de datos", "Ingrese un precio unitario", QMessageBox::Ok ); return; } */

 CBProducto->verificarExiste();

 mcp->agregarNuevoProducto( SBCant->value(), CBProducto->idActual(), DSBPrecioUnitario->value() );

 SBCant->setValue( 1.0 );
 CBProducto->setCurrentIndex( -1 );
 SBCant->setFocus();
}


/*!
    \fn FormAgregarCompra::eliminarProducto()
 */
void FormAgregarCompra::eliminarProducto()
{
 //Preguntar al usuario si esta seguro
 QModelIndexList indices = TVLista->selectionModel()->selectedRows();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para eliminar",
                   QMessageBox::Ok );
   return;
 }
 //Hacer dialogo de confirmacion..
 int ret;
 ret = QMessageBox::warning( this, "Esta seguro?",
                   QString( "Esta seguro de eliminar %1 item?").arg( indices.size() ),
                   "Si", "No" );
 if ( ret == 0 )
 {
	QModelIndex indice;
	foreach( indice, indices )
	{
		if( indice.isValid() )
		{  mcp->removeRow( indice.row() ); }
	}
 }
 return;
}

/*!
 * \brief FormAgregarCompra::arreglarProductoAgregado
 * \param anterior
 * \param nuevo
 */
void FormAgregarCompra::arreglarProductoAgregado( int anterior, int nuevo )
{

    ecbmproducto->arreglarItemTemporal( anterior, nuevo );

    mcp->arreglarIdProductoAgregado( anterior, nuevo );

    // Como mcp esta trabajando con un puntero a la lista anterior no tengo que actualizar nada mas


}

/*!
 * \brief FormAgregarCompra::cambioProveedor
 * \param id_proveedor
 */
void FormAgregarCompra::cambioProveedor( int id_proveedor )
{
    if( id_proveedor != _id_proveedor_anterior ) {
        if( mcp->rowCount() > 1 ) {
            int ret = QMessageBox::question( this,
                                             QString::fromUtf8( "¿Esta seguro?" ),
                                             QString::fromUtf8( "Esta seguro que desea cambiar de proveedor? Se borraran todos los productos ingresados" ),
                                             QMessageBox::Yes,
                                             QMessageBox::No );
            if( ret == QMessageBox::Yes ) {
                mcp->vaciarProductos();
                CBProducto->filtrarPorProveedor( id_proveedor );
                _id_proveedor_anterior = id_proveedor;
            } else {
                CBProveedor->setearId( _id_proveedor_anterior );
            }
        } else {
            _id_proveedor_anterior = id_proveedor;
        }
    }
}
