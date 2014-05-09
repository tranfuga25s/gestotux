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
#include "vproductos.h"

#include <QTableView>
#include <QSqlRecord>
#include <QHeaderView>
#include <QAction>
#include <QMessageBox>
#include <QSqlRelationalDelegate>

#include "productos.h"
#include "mcategorias.h"
#include "vcategorias.h"
#include "preferencias.h"
#include "mproductos.h"
#include "dproductos.h"
#include "dsino.h"
#include "formagregarproducto.h"

VProductos::VProductos(QWidget *parent)
 : EVLista(parent)
{
 this->setObjectName( "listaproductos" );
 this->setWindowTitle( "Lista de Productos");
 this->setWindowIcon( QIcon( ":/imagenes/productos.png" ) );

 modelo = 0;
 rmodelo = new MProductos( vista );

 vista->setModel( rmodelo );

 vista->setItemDelegateForColumn( rmodelo->fieldIndex( "habilitado" ), new DSiNo( vista ) );

 vista->hideColumn( 0 );
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Productos" );
 if( !p->value( "categorias" ).toBool() )
 { vista->hideColumn( rmodelo->fieldIndex( "id_categoria" ) ); }
 if( !p->value( "descripcion" ).toBool() )
 { vista->hideColumn( rmodelo->fieldIndex( "descripcion" ) ); }
 if( !p->value( "marcas" ).toBool() )
 { vista->hideColumn( rmodelo->fieldIndex( "marca" ) ); }
 bool hab_stock = p->value( "stock" ).toBool();
 if( !hab_stock )
 { vista->hideColumn( rmodelo->fieldIndex( "stock" ) ); }
 if( !p->value( "modelo" ).toBool() )
 { vista->hideColumn( rmodelo->fieldIndex( "modelo" ) ); }
 if( !p->value( "mostrar-costo", true ).toBool() )
 { vista->hideColumn( rmodelo->fieldIndex( "precio_costo" ) ); } else { vista->showColumn( rmodelo->fieldIndex( "precio_costo" ) ); }
 if( p->value( "ocultar_codigo", false ).toBool() )
 { vista->hideColumn( rmodelo->fieldIndex( "codigo" ) ); }
 vista->resizeColumnsToContents();
 vista->verticalHeader()->setResizeMode( QHeaderView::ResizeToContents );
 vista->horizontalHeader()->setResizeMode( rmodelo->fieldIndex( "nombre" ), QHeaderView::ResizeToContents );
 vista->horizontalHeader()->setResizeMode( rmodelo->fieldIndex( "codigo" ), QHeaderView::ResizeToContents );
 vista->setAlternatingRowColors( true );
 vista->setSortingEnabled( true );

 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActEliminar );

 ActHabilitar = new QAction( this );
 ActHabilitar->setText( "Habilitar" );
 ActHabilitar->setIcon( QIcon( ":/imagenes/producto-habilitar.png" ) );
 connect( ActHabilitar, SIGNAL( triggered() ), this, SLOT( habilitarProducto() ) );
 addAction( ActHabilitar );

 ActDeshabilitar = new QAction( this );
 ActDeshabilitar->setText( "Deshabilitar" );
 ActDeshabilitar->setIcon( QIcon( ":/imagenes/producto-habilitar.png" ) );
 connect( ActDeshabilitar, SIGNAL( triggered() ), this, SLOT( deshabilitarProducto() ) );
 addAction( ActDeshabilitar );

 QAction *ActSep = new QAction( this );
 ActSep->setSeparator( true );
 addAction( ActSep );

 if( p->value( "categorias" ).toBool() )
 {
         ActCategorias = new QAction( "Categorias" , this );
         ActCategorias->setIcon( QIcon( ":/imagenes/categorias.png" ) );
         ActCategorias->setStatusTip( "Ver y administrar las categorias de productos" );
         ActCategorias->setShortcut( QKeySequence( "Ctrl + c" ) );
         connect( ActCategorias, SIGNAL( triggered() ), this, SLOT( verCategorias() ) );

         addAction( ActCategorias );
 }


 ActListadoVenta = new QAction( this );
 ActListadoVenta->setText( "Listado de venta" );
 ActListadoVenta->setIcon( QIcon( ":/imagenes/listado.png" ) );
 ActListadoVenta->setStatusTip( "Muestra el listado de productos con su precio de venta" );
 //ActListadoVenta->setIcon( QIcon( ":/imagenes/listaventa.png" ) );
 connect( ActListadoVenta, SIGNAL( triggered() ), this, SLOT( listaVenta() ) );

 ActListadoStock = new QAction( this );
 ActListadoStock->setText( "Listado stock" );
 ActListadoStock->setIcon( QIcon( ":/imagenes/listado.png" ) );
 ActListadoStock->setStatusTip( "Muestra el listado de productos con su stock actual" );
 //ActListadoStock->setIcon( QIcon( ":/imagenes/listastock.png" ) );
 connect( ActListadoStock, SIGNAL( triggered() ), this, SLOT( listaStock() ) );

 ActVerDeshabilitados = new QAction( this );
 ActVerDeshabilitados->setText( "Deshabilitados" );
 ActVerDeshabilitados->setIcon( QIcon( ":/imagenes/filtro.png" ) );
 ActVerDeshabilitados->setStatusTip( "Mestra los productos que estan deshabilitados" );
 ActVerDeshabilitados->setCheckable( true );
 connect( ActVerDeshabilitados, SIGNAL( toggled( bool ) ), this, SLOT( mostrarDeshabilitados( bool ) ) );

 addAction( ActListadoVenta );
 if( hab_stock == true ) {
    addAction( ActListadoStock );
 }
 addAction( ActBuscar );
 addAction( ActVerDeshabilitados );

 ActVerCosto = new QAction( this );
 ActVerCosto->setText( "Costo" );
 ActVerCosto->setIcon( QIcon( ":/imagenes/productos-costos.png" ) );
 ActVerCosto->setStatusTip( "Muestra o oculta la columna de precio de costo" );
 ActVerCosto->setCheckable( true );
 ActVerCosto->setChecked( p->value( "mostrar-costo", true ).toBool() );
 connect( ActVerCosto, SIGNAL( toggled( bool ) ), this, SLOT( mostrarOcultarCosto( bool ) ) );

 addAction( ActVerCosto );
 addAction( ActVerTodos );
 addAction( ActCerrar );

 agregarFiltroBusqueda( "Todo", "`producto`.`codigo` LIKE '%%%1%' OR  `producto`.`nombre` LIKE '%%%1%'  OR `producto`.`descripcion` LIKE '%%%1%'" );
 agregarFiltroBusqueda( QString::fromUtf8("Código"), " `producto`.`codigo` LIKE '%%%1%' " );
 agregarFiltroBusqueda( "Nombre", " `producto`.`nombre` LIKE '%%%1%' " );
 if( p->value( "descripcion", false ).toBool() ) {
    agregarFiltroBusqueda( QString::fromUtf8("Descripción"), " `producto`.`descripcion` LIKE '%%%1%' " );
 }
 if( hab_stock == true ) {
    agregarFiltroBusqueda( "Stock mayor o igual a ", " `stock` >= %1" );
    agregarFiltroBusqueda( "Stock menor a ", " `stock` < %1" );
 }
 habilitarBusqueda();

 p->endGroup();p->endGroup(); p=0;
}

/*!
    \fn VProductos::antes_de_insertar(  int row, QSqlRecord &registro )
        Funcion que coloca en el valor predeterminado los valores del registro. Esto evita la falla al insertar el registro.
        @param row Numero de fila a insertar
        @param registro Registro a insertar
 */
void VProductos::antes_de_insertar(  int /* row */, QSqlRecord &registro )
{
 registro.setValue( "descripcion", "" );
 registro.setValue( "marca", "" );
 registro.setValue( "modelo", "" );
}

/*!
    \fn VProductos::verCategorias()
        Slot llamado para ver la lista de categorias
 */
void VProductos::verCategorias()
{ emit agregarVentana( new VCategorias() ); }


/*!
    \fn VProductos::agregar( bool autoeliminarid )
    Slot llamado para agregar un producto.
    \param autoeliminarid No utilizado
 */
void VProductos::agregar( bool /*autoeliminarid*/ )
{
 // Ver si existe alguna categoria primero
 if( preferencias::getInstancia()->value( "Preferencias/Productos/categorias" ).toBool() )
 {
         qDebug( "Verificando que existan categorias" );
         MCategorias *m = new MCategorias();
         int cantidad_categorias = m->rowCount();
         delete m;
         if( cantidad_categorias <= 0 )
         {
          qWarning( "Por favor, primero ingrese al menos una categoria de productos" );
          return;
         }
 }
 // Muestro el formulario
 FormAgregarProducto *f = new FormAgregarProducto();
 connect( f, SIGNAL( accepted() ), this, SLOT( actualizar() ) );
 emit agregarVentana( f );
}

#include <QMessageBox>
#include "EReporte.h"
/*!
 * \fn VProductos::listaVenta()
 * Muestra el listado de productos con sus codigos y precio de venta segun el filtro actual
 */
void VProductos::listaVenta()
{
    if( this->rmodelo->rowCount() <= 0 ) {
        QMessageBox::information( this, "Error", "No hay ningun producto declarado como para  dar un listado." );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    ParameterList lista;
    lista.append( "filtro", this->rmodelo->filter() );
    rep->especial( "ListadoProductosPrecio", lista );
    rep->hacer();
    delete rep;
}

/**
 * @brief VProductos::listaStock
 * Imprime el listado de stock
 */
void VProductos::listaStock()
{
    if( this->rmodelo->rowCount() <= 0 ) {
        QMessageBox::information( this, "Error", "No hay ningun producto declarado como para  dar un listado." );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    ParameterList lista;
    lista.append( "filtro", this->rmodelo->filter() );
    rep->especial( "ListadoProductosStock", lista );
    rep->hacer();
    delete rep;
}

/**
 * @brief VProductos::actualizar
 * Slot llamado para actualizar la vista despues de agegar un producto.
 */
void VProductos::actualizar() {
    this->rmodelo->select();
}

#include "formmodificarproducto.h"
/**
 * @brief VProductos::modificar
 * Slot llamado para modificar un producto
 */
void VProductos::modificar()
{
    // busco el que esta seleccionado
    QItemSelectionModel *selectionModel = vista->selectionModel();
    QModelIndexList indices = selectionModel->selectedRows();
    if( indices.size() < 1 )
    {
      QMessageBox::warning( this, "Seleccione un item",
                      "Por favor, seleccione un item para modificar",
                      QMessageBox::Ok );
      return;
    }
    int fila = indices.first().row();
    FormModificarProducto *f = new FormModificarProducto( qobject_cast<MProductos *>( this->rmodelo ) );
    f->setearProducto( fila );
    connect( f, SIGNAL( accepted() ), this, SLOT( actualizar() ) );
    f->exec();
}

/**
 * @brief VProductos::habilitarProducto
 * Slot llamado para habilitar un producto de la lista
 */
void VProductos::habilitarProducto()
{
  // Busco el item
  QModelIndex indice = this->vista->selectionModel()->selectedRows().first();
  int id_producto = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
  if( qobject_cast<MProductos *>(rmodelo)->habilitado( id_producto ) ) {
      QMessageBox::information( this, "Habilitacion", "El producto ya se encuentra habilitado" );
      return;
  }
  int ret = QMessageBox::question( this, QString::fromUtf8( "¿Seguro?" ), QString::fromUtf8("¿Está seguro que desea habilitar este producto?" ), QMessageBox::Yes, QMessageBox::No );
  if( ret == QMessageBox::Yes ) {
      if( qobject_cast<MProductos *>(rmodelo)->habilitar( indice ) ) {
          // Actualizar los datos
          QMessageBox::information( this, "Correcto", "El producto fue habilitado correctamente" );
      } else {
          QMessageBox::warning( this, "Incorrecto", "Existio un error al intentar habilitar el producto" );
      }
  }
  return;
}

/**
 * @brief VProductos::deshabilitarProducto
 * Slot llamado para deshabilitar un producto
 */
void VProductos::deshabilitarProducto()
{
    // Busco el item
    QModelIndex indice = this->vista->selectionModel()->selectedRows().first();
    int id_producto = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    if( !qobject_cast<MProductos *>(rmodelo)->habilitado( id_producto ) ) {
        QMessageBox::information( this, "Deshabilitacion", "El producto ya se encuentra deshabilitado" );
        return;
    }
    int ret = QMessageBox::question( this, QString::fromUtf8( "¿Seguro?" ), QString::fromUtf8("¿Está seguro que desea deshabilitar este producto? \n No se podrán realizar mas ventas de este producto si se lo deshabilita." ), QMessageBox::Yes, QMessageBox::No );
    if( ret == QMessageBox::Yes ) {
        if( qobject_cast<MProductos *>(rmodelo)->deshabilitar( indice ) ) {
            QMessageBox::information( this, "Correcto", "El producto fue deshabilitado correctamente" );
        } else {
            QMessageBox::warning( this, "Incorrecto", "Existio un error al intentar deshabilitar el producto" );
        }
    }
    return;
}

/**
 * @brief VProductos::mostrarDeshabilitados
 * Slot llamado para mostrar o sacar los elementos deshabilitados
 */
void VProductos::mostrarDeshabilitados( bool /*mostrar*/ )
{
  qWarning( "No implementado todavía" );
}

/**
 * @brief VProductos::mostrarOcultarCosto
 * Muestra o oculta la columna de precio de costo en el listado de productos
 * @param mostrar Muestra o oculta la columna
 */
void VProductos::mostrarOcultarCosto( bool mostrar )
{
    if( mostrar ) {
      vista->showColumn( rmodelo->fieldIndex( "precio_costo" ) );
    } else {
      vista->hideColumn( rmodelo->fieldIndex( "precio_costo" ) );
    }
}

/**
 * @brief VProductos::eliminar
 * Slot llamado cuando se desea eliminar un produto de la lista.
 */
void VProductos::eliminar()
{
    // Busco el item
    QModelIndexList indices = this->vista->selectionModel()->selectedRows();
    if( indices.size() < 1 )
    {
      QMessageBox::warning( this, "Seleccione un item",
                      "Por favor, seleccione un item para eliminar",
                      QMessageBox::Ok );
      return;
    }
    int ret = QMessageBox::question( this,
                                     QString::fromUtf8( "¿Está seguro?"),
                                     QString::fromUtf8("¿Está seguro que desea eliminar %1 producto(s)?").arg( indices.size() ) );
    if( ret == QMessageBox::Yes ) {
        int neliminados = 0; int eliminados = 0;
        foreach( QModelIndex idx, indices ) {
            int id = this->rmodelo->data( this->rmodelo->index( idx.row(), 0 ), Qt::EditRole ).toInt();
            if( !qobject_cast<MProductos *>(this->rmodelo)->tieneDatosRelacionados( id ) ) {
                this->rmodelo->removeRow( idx.row() );
                eliminados++;
            } else {
                neliminados++;
            }
        }
        if( neliminados > 0 ) {
            QMessageBox::information( this, "Listo", QString::fromUtf8( "Se eliminaron %1 elementos. Falló eliminar %2 elementos.<br />Si el producto fue utilizado alguna vez, tiene datos relacionados y no podrá ser eliminado. Intente deshabilitarlo.").arg( eliminados ).arg( neliminados ) );
        } else {
            QMessageBox::information( this, "Listo", QString::fromUtf8( "Se eliminaron %1 productos.").arg( eliminados ) );
        }
    }
}
