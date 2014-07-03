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
#include "vcliente.h"

#include "mclientes.h"
#include "eregistroplugins.h"

#include <QApplication>
#include <QTableView>
#include <QGridLayout>
#include <QAction>
#include <QTableView>
#include <QHeaderView>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QModelIndex>


VCliente::VCliente( QWidget *parent )
    : EVLista( parent )
{
 setObjectName( "lista_clientes" );
 this->setAttribute( Qt::WA_DeleteOnClose );
 setWindowTitle( "Listado de Clientes" );
 setWindowIcon( QIcon( ":/imagenes/clientes.png" ) );

 mc = new MClientes( vista );
 mc->setFilter( "id != 0" );
 modelo = 0;
 rmodelo = 0;
 modelo = mc;
 vista->setModel( mc );
 mc->query().setForwardOnly( true );
 vista->hideColumn( mc->fieldIndex( "id" ) );
 vista->hideColumn( mc->fieldIndex( "nombre" ) );
 vista->hideColumn( mc->fieldIndex( "apellido" ) );
 vista->hideColumn( mc->fieldIndex( "piso" ) );
 vista->hideColumn( mc->fieldIndex( "depto" ) );
 vista->hideColumn( mc->fieldIndex( "provincia" ) );
 vista->hideColumn( mc->fieldIndex( "codigo_postal" ) );
 vista->hideColumn( mc->fieldIndex( "pais" ) );
 vista->hideColumn( mc->fieldIndex( "fax" ) );
 vista->hideColumn( mc->fieldIndex( "email" ) );
 vista->hideColumn( mc->fieldIndex( "comprobante_email" ) );
 vista->hideColumn( mc->fieldIndex( "id_estado_fiscal" ) );
 vista->hideColumn( mc->fieldIndex( "ctacte" ) );
 vista->hideColumn( mc->fieldIndex( "CUIT/CUIL" ) );
 vista->setAlternatingRowColors( true );
 vista->setSortingEnabled( true );

 connect( vista, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( modificarIndice( QModelIndex ) ) );

 QAction *ActLista = new QAction( this );
 ActLista->setText( "Listado" );
 ActLista->setStatusTip( "Listado de clientes" );
 ActLista->setIcon( QIcon( ":/imagenes/listaclientes.png" ) );
 connect( ActLista, SIGNAL( triggered() ), this, SLOT( listadoClientes() ) );

 ActAgregar->setIcon( QIcon( ":/imagenes/add_user.png" ) );
 ActEliminar->setIcon( QIcon( ":/imagenes/delete_user.png" ) );

 agregarFiltroBusqueda( "Cualquiera", " `razon_social` LIKE '%%%1%' OR nombre LIKE '%%%1%' OR apellido LIKE '%%%1%' OR calle LIKE '%%%1%' OR numero LIKE '%%%1%' OR piso LIKE '%%%1%' OR depto LIKE '%%%1%' OR ciudad LIKE '%%%1%' OR `codigo_postal` LIKE '%%%1%' OR provincia LIKE '%%%1%' OR pais LIKE '%%%1%' OR `tel_fijo` LIKE '%%%1%' OR `tel_celular` LIKE '%%%1%' OR fax LIKE '%%%1%' OR email LIKE '%%%1%' OR `CUIT/CUIL` LIKE '%%%1%'" );
 agregarFiltroBusqueda( "Razon Social", " `razon_social` LIKE '%%%1%'" );
 agregarFiltroBusqueda( "Numero de Cliente", " id = %1" );
 agregarFiltroBusqueda( "CUIT/CUIL", " `CUIT/CUIL` = '%%%1%'" );
 habilitarBusqueda();

 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActEliminar );
 addAction( ActLista );
 addAction( ActBuscar );
 addAction( ActVerTodos );
 addAction( ActCerrar );
}

#include "formcliente.h"
/*!
    \fn VCliente::agregar( bool autoeliminarid )
 */
void VCliente::agregar( bool /*autoeliminarid*/ )
{
    FormCliente *f = new FormCliente( this, mc );
    f->agregar();
    emit agregarVentana( f );
}


/*!
  \fn VCliente::modificar()
 */
void VCliente::modificar()
{
    FormCliente *f = new FormCliente( this, mc );
    f->setearCliente( this->vista->selectionModel()->selectedRows().first() );
    f->setObjectName( QString( "visor-clientes-%1" ).arg( this->vista->selectionModel()->selectedRows().first().data( Qt::EditRole ).toInt() ) );
    emit agregarVentana( f );
}



/*!
  \fn VCliente::modificarIndice( QModelIndex m)
 */
void VCliente::modificarIndice( QModelIndex m )
{
    FormCliente *f = new FormCliente( this, mc );
    f->setearCliente( m );
    emit agregarVentana( f );
}

#include "EReporte.h"
/*!
 * \brief VCliente::listadoClientes
 */
void VCliente::listadoClientes() {
    if( this->mc->rowCount() <= 0 ) {
        QMessageBox::warning( this, "Error", "No existe ningun cliente para imprimir un listado. No se imprimira nada" );
        return;
    }
    // Hago el listado de clientes
    EReporte *rep = new EReporte( this );
    ParameterList lista;
    QString filtro = this->mc->filter();
    if( filtro.isEmpty() ) {
        filtro = " 1 = 1 ";
    }
    lista.append( Parameter( "fil", this->mc->filter() ) );
    rep->especial( "ListadoClientes", lista );
    rep->hacer();
    delete rep;
}

/*!
 * \brief VCliente::eliminar
 */
void VCliente::eliminar()
{
    QModelIndex idx = this->vista->selectionModel()->selectedRows().first();
    int id_cliente = this->mc->data( this->mc->index( idx.row(), 0 ), Qt::EditRole ).toInt();
    if( MClientes::tieneDatosRelacionados( id_cliente ) ) {
        QMessageBox::warning( this, "Error", QString::fromUtf8("No se pudo eliminar este cliente porque tiene datos relacionados en otras partes del programa y generará perdida de datos." ) );
        return;
    }
    else
    {
        if( this->mc->removeRow( idx.row() ) ) {
            QMessageBox::information( this, "Correcto", QString::fromUtf8( "El cliente ha sido eliminado correctamente" ) );
            return;
        } else {
            qWarning( "No se pudo eliminar el cliente" );
        }
    }
    return;
}

/*!
 * \brief VCliente::mostrarTodos
 */
void VCliente::mostrarTodos()
{
    this->mc->setFilter("");
    this->mc->select();
}

/*!
 * \brief VCliente::menuContextual
 * \param indice
 * \param menu
 */
void VCliente::menuContextual( const QModelIndex &indice, QMenu *menu )
{
    //int id_cliente = indice.model()->data( indice.model()->index( indice.row(), 0 ), Qt::EditRole ).toInt();
    menu->addAction( ActAgregar );
    menu->addAction( ActModificar );
    menu->addAction( ActEliminar );
    menu->addSeparator();
    // Veo si tiene cuenta corriente

    // Veo si está aderido a algún servicio


}
