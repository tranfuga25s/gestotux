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
#include "vproveedor.h"
#include "mproveedor.h"
#include "dproveedor.h"
#include "EReporte.h"

#include <QTableView>
#include <QAction>
#include <QDate>
#include <QMessageBox>

VProveedor::VProveedor( QWidget *parent )
 : EVLista( parent )
{
 setObjectName( "ListaProveedores" );
 setWindowTitle( "Proveedores" );
 setWindowIcon( QIcon( ":/imagenes/proveedores.jpg" ) );
 modelo = new MProveedor( this );
 vista->setModel( modelo );
 vista->hideColumn( 0 ); // oculta el ID
 vista->setItemDelegate( new DProveedor( vista ) );
 modelo->sort( 2, Qt::AscendingOrder );
 modelo->select();
 vista->setSortingEnabled( true );

 QAction *ActListado = new QAction( this );
 ActListado->setText( "Listado" );
 ActListado->setStatusTip( "Imprime la lista de todos los proveedores" );
 ActListado->setIcon( QIcon( ":/imagenes/impresora.png" ) );
 connect( ActListado, SIGNAL(triggered() ), this, SLOT( listado() ) );

 QAction *ActListadoPDF = new QAction( this );
 ActListadoPDF->setText( "Listado" );
 ActListadoPDF->setStatusTip( "Genera una lista de todos los proveedores en formato pdf" );
 ActListadoPDF->setIcon( QIcon( ":/imagenes/acroread.png" ) );
 connect( ActListadoPDF, SIGNAL(triggered() ), this, SLOT( listadoPDF() ) );

 agregarFiltroBusqueda( "Nombre", " `nombre` LIKE '%%%1%' " );
 agregarFiltroBusqueda( QString::fromUtf8( "Código" ), " `id` LIKE '%%1%' " );
 agregarFiltroBusqueda( "CUIT/CUIL", " `CUIT/CUIL`  LIKE '%%1%' ");
 habilitarBusqueda();

 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActEliminar );
 addAction( ActListado );
 addAction( ActListadoPDF );
 addAction( ActBuscar );
 addAction( ActCerrar );
}

/*!
    \fn VProveedor::eliminar()
 */
void VProveedor::eliminar()
{
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor seleccione un proveedor para eliminar" );
        return;
    }
    QModelIndexList filas = this->vista->selectionModel()->selectedRows();
    foreach( QModelIndex indice, filas ) {
        int id_proveedor = this->modelo->data( this->modelo->index( indice.row(), 0 ), Qt::EditRole ).toInt();
        if( MProveedor::tieneDatosRelacionados( id_proveedor ) ) {
            QMessageBox::warning( this, "Error", "El proveedor que ha seleccionado contiene datos relacionados que podrían quedar en estado inconsistente si lo elimina.\n No se eliminará el proveedor" );
        } else {
            if( this->modelo->removeRow( indice.row() ) ) {
                QMessageBox::information( this, "Correcto", "El proveedor se eliminó correctamente" );
            } else {
                QMessageBox::information( this, "Error", "Existió un error al intentar eliminar la fila seleccionada" );
            }
        }
    }
    return;
}

/*!
 * \fn VProveedor::listado()
 * Muestra el listado de proveedores
 */
void VProveedor::listado()
{
    if( this->modelo->rowCount() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("No existe ningun proveedor para listar. No se imprimirá ningun listado" ) );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    rep->especial( "ListadoProveedores", ParameterList() );
    if( !rep->hacer( ParameterList(), true ) ) {
        qDebug( "Hubo un error al intentar hacer el reporte" );
    }
    delete rep;
}

void VProveedor::listadoPDF()
{
    if( this->modelo->rowCount() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("No existe ningun proveedor para listar. No se imprimirá ningun listado" ) );
        return;
    }
    EReporte *rep = new EReporte( 0 );
    rep->especial( "ListadoProveedores", ParameterList() );
    if( !rep->hacerPDF( ParameterList(), QString( "Listado de proveedores al %1" ).arg( QDate::currentDate().toString( Qt::SystemLocaleShortDate ) ) ) ) {
        qDebug( "Hubo un error al intentar hacer el reporte" );
    }
    delete rep;
}

#include "formagregarproveedor.h"
/*!
 * \fn VProveedor::agregar( bool autoeliminarid )
 * Muestra la pantalla de agregado
 */
void VProveedor::agregar( bool /*autoeliminarid*/ )
{
    FormAgregarProveedor *f = new FormAgregarProveedor( qobject_cast<MProveedor*>(this->modelo), this );
    emit agregarVentana( f );
}


#include "formmodificarproveedor.h"
/*!
 * \fn VProveedor::modificar()
 * Muestra la pantalla de edicion de un proveedor
 */
void VProveedor::modificar()
{
    if( this->modelo->rowCount() <= 0 ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("No existe ningun proveedor." ) );
        return;
    }
    //Busco el primer proveedor
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this,"No hay seleccion", "Por favor, elija un registro de proveedor para modificar" );
        return;
    }
    QModelIndex indice = this->vista->selectionModel()->selectedRows().first();
    FormModificarProveedor *f = new FormModificarProveedor( qobject_cast<MProveedor*>(this->modelo), this );
    emit agregarVentana( f );
    f->setearItem( indice.row() );
}