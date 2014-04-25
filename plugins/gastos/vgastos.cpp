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
#include "vgastos.h"

#include "mgasto.h"
#include "dgastos.h"
#include <QPushButton>
#include <QTableView>
#include <QSqlRelationalDelegate>
#include <QAction>
#include <QMessageBox>
#include <QSqlError>
#include <QHeaderView>
#include <QDebug>

VGastos::VGastos( QWidget* parent )
: EVLista( parent )
{
 setObjectName( "vistaGastos" );
 setWindowTitle( "Visor de Gastos" );
 setWindowIcon( QIcon(":/imagenes/gasto.jpg" ) );

 // Redirijo la opcion de agregar al plugin
 ActAgregar->disconnect();
 connect( ActAgregar, SIGNAL( triggered() ), this, SLOT( agregarGasto() ) );

 modelo = new MGasto( this, true );
 modelo->setSort( modelo->fieldIndex( "fecha" ), Qt::DescendingOrder );

 vista->setModel( modelo );
 vista->hideColumn( 0 );
 vista->hideColumn( 5 );
 vista->setItemDelegate( new DGastos( vista ) );
 vista->setSortingEnabled( true );
 vista->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
 vista->horizontalHeader()->setResizeMode( 2, QHeaderView::Stretch );
 vista->horizontalHeader()->setMinimumSectionSize( 60 );

 vista->resizeColumnsToContents();

 QAction *ActCategorias = new QAction( this );
 ActCategorias->setText( "Categorias" );
 ActCategorias->setIcon( QIcon( ":/imagenes/categorias.png" ) );
 ActCategorias->setStatusTip( "Muestra las distintas categorias de gastos disponibles" );
 connect( ActCategorias, SIGNAL( triggered() ), this, SLOT( mostrarCategorias() ) );

 agregarFiltroBusqueda( QString::fromUtf8( "Descripción" ), " `gastos`.`descripcion` LIKE '%%%1%' " );
 agregarFiltroBusqueda( "Costo mayor a", " `gastos`.`costo` >= %1" );
 agregarFiltroBusqueda( "Costo menor a", " `gastos`.`costo` <= %1" );
 /// @TODO: Agregar Filtro de fecha
 habilitarBusqueda();

 addAction( ActAgregar );
 addAction( ActEliminar );
 addAction( ActCategorias );
 addAction( ActImprimir );
 addAction( ActPdf );
 addAction( ActBuscar );
 addAction( ActVerTodos );
 addAction( ActCerrar );
 actualizarVista();
}

#include "formagregargasto.h"
#include "mcategoriasgastos.h"
#include <QMessageBox>
/*!
    \fn VGastos::agregarGasto()
    Muestra la ventana de agregar gastos
 */
void VGastos::agregarGasto()
{
    MCategoriasGastos *mc = new MCategoriasGastos();
    if( mc->vacio() ) {
        QMessageBox::warning( this, "Faltan datos", QString::fromUtf8( "No existe ninguna categoría de gastos definida. Por favor cree una. \n Se abrira la ventana de categorías de gastos para que pueda realizar la accion solicitada." ) );
        mostrarCategorias();
        delete mc;
        return;
    }
    delete mc;
    FormAgregarGasto *f = new FormAgregarGasto();
    connect( f, SIGNAL( actualizarVista() ), this, SLOT( actualizarVista() ) );
    emit agregarVentana( f );
}

#include "vcategoriasgastos.h"
/*!
    \fn VGastos::mostrarCategorias()
    Muestra la ventana de cateogiras de gastos
 */
void VGastos::mostrarCategorias()
{ emit agregarVentana( new VCategoriasGastos() ); }


void VGastos::actualizarVista() {
  this->modelo->select();
}

void VGastos::eliminar() {
    MGasto *m = qobject_cast<MGasto *>(vista->model());
    //Preguntar al usuario si esta seguro
    QItemSelectionModel *selectionModel = vista->selectionModel();
    QModelIndexList indices = selectionModel->selectedRows();
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
                   {
                           if( m->eliminarFila( indice.row() ) )
                           { return; }
                           else
                           { qWarning() << "Error al eliminar el registro" << m->lastError().text(); }
                   }
           }
    }
    return;
}

#include "EReporte.h"
void VGastos::imprimir()
{
    if( modelo->rowCount() <= 0 ) {
        QMessageBox::information( this, "Sin datos", QString::fromUtf8( "No existe ningún gasto declarado. No se podrá imprimir el reporte" ) );
        return;
    }
    EReporte *rep = new EReporte( this );
    rep->especial( "ListadoGastos", ParameterList() );
    if( !rep->hacer() ) {
        qWarning( "Error al intentar imprimir el listado de gastos" );
    }
}

void VGastos::aPdf()
{
    if( modelo->rowCount() <= 0 ) {
        QMessageBox::information( this, "Sin datos", QString::fromUtf8( "No existe ningún gasto declarado. No se podrá imprimir el reporte" ) );
        return;
    }
    EReporte *rep = new EReporte( this );
    rep->especial( "ListadoGastos", ParameterList() );
    if( !rep->hacerPDF( ParameterList(), "Listado de Gastos" ) ) {
        qWarning( "Error al pasar a pdf el listado de gastos" );
    }
}
