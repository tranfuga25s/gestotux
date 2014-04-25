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
#include "vcompras.h"

#include "mcompra.h"
#include "formagregarcompra.h"

#include "dsino.h"

#include <QTableView>
#include <QPushButton>
#include <QAction>
#include <QMessageBox>

VCompras::VCompras(QWidget *parent)
 : EVLista( parent )
{
 setObjectName( "lista_compras" );
 setWindowTitle( "Lista de compras realizadas" );
 setWindowIcon( QIcon(":/imagenes/compras.png" ) );

 rmodelo = new MCompra( this, true );
 vista->setModel( rmodelo );
 vista->hideColumn( 0 );
 vista->hideColumn( 4 );
 vista->setItemDelegateForColumn( 5, new DSiNo( vista ) );
 rmodelo->select();

 ActAgregar->setIcon( QIcon( ":/imagenes/agregar_compras.png" ) );
 ActModificar->setIcon( QIcon( ":/imagenes/editar_compras.png" ) );

 QAction *ActVerLista = new QAction( this );
 ActVerLista->setText( "Ver Productos" );
 ActVerLista->setIcon( QIcon( ":/imagenes/verprod.png" ) );
 ActVerLista->setStatusTip( "Muestra el listado de productos que se compró en esa compra" );
 connect( ActVerLista, SIGNAL( triggered() ), this, SLOT( verLista() ) );

 agregarFiltroBusqueda( "A contado", "`contado` = 1" );
 agregarFiltroBusqueda( "Proveedor", "`nombre` LIKE '%%%1%'" );
 agregarFiltroBusqueda( "Mayor a ", "`total` >= %1" );
 agregarFiltroBusqueda( "Menor a", "`total` <= %1" );
 habilitarBusqueda();

 addAction( ActAgregar );
 addAction( ActEliminar );
 addAction( ActVerLista );
 addAction( ActVerTodos );
 addAction( ActBuscar );
 addAction( ActCerrar );
}

/*!
 * \fn VCompras::agregar( bool autoeliminarid )
 * Muestra la ventana de agregar una nueva compra
 */
void VCompras::agregar( bool /*autoeliminarid*/ )
{
    FormAgregarCompra *f = new FormAgregarCompra( qobject_cast<MCompra *>( this->rmodelo ) );
    connect( f, SIGNAL( actualizarVista() ), this, SLOT( mostrarTodos() ) );
    emit agregarVentana( f );
}

#include "vlistacompra.h"
/*!
 * \fn VCompras::verLista( const QModelIndex &indice )
 * Muestra el listado de productos comprados en esa compra especifica.
 * \param indice Indice de la compra
 */
void VCompras::verLista( const QModelIndex &indice )
{
    // Busco el id de compra segun el indice
    int id_compra = this->rmodelo->data( this->rmodelo->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    //Muestro el formulario
    VListaCompra *f = new VListaCompra( this );
    f->setearIdCompra( id_compra );
    emit agregarVentana( f );
    return;
}

/*!
 * \fn VCompras::verLista()
 * Muestra el listado de productos comprados en la compra que este seleccionada en la lista.
 */
void VCompras::verLista()
{
 // Busco el primer indice seleccionado.
 if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::information( this, "Falta seleccion", "Por favor, seleccione una compra para ver sus productos." );
        return;
 }
 verLista( this->vista->selectionModel()->selectedRows().first() );
}

/*!
 * \fn VCompras::eliminar()
 * Muestra el dialogo de confirmación de eliminacion de una compra
 */
void VCompras::eliminar()
{
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::information( this, "Nada seleccionado", "Por favor, seleccione una compra para eliminarla." );
        return;
    }
    int ret = QMessageBox::question( this, "¿Esta seguro?", QString::fromUtf8( "Esta seguro que desea eliminar las compras seleccionadas?\n Se descontará el stock de productos relacionados y se ajustará el saldo de caja."), QMessageBox::Ok, QMessageBox::Cancel );
    if( ret != QMessageBox::Ok ) { return; }
    MCompra *mc = new MCompra();
    QString mensaje;
    foreach( QModelIndex indice, this->vista->selectionModel()->selectedRows() ) {
        // Busco los datos de la compra
        int id_compra = this->rmodelo->data( this->rmodelo->index( indice.row(), 0 ), Qt::EditRole ).toInt();
        if( mc->eliminarCompra( id_compra ) ) {
            mensaje.append( QString::fromUtf8("\n Compra nº") ).append( id_compra ).append( " eliminada." );
            rmodelo->select();
        } else {
            mensaje.append( "\n <b> no se pudo eliminar la compra " ).append( id_compra ).append( " correctamente." );
        }
    }
    QMessageBox::information( this, "Resultado", "El resultado de la operacion fue: " + mensaje );
    delete mc;
}
