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
#include "vpresupuestos.h"
#include "mpresupuestos.h"
#include "ereporte.h"
#include <QTableView>
#include <QSqlRelationalDelegate>


#include <QAction>

VPresupuestos::VPresupuestos(QWidget *parent)
 : EVLista( parent )
{
 setObjectName( "visorPresupuestos" );
 setWindowTitle( "Listado de Presupuestos" );
 setWindowIcon( QIcon( ":/imagenes/anteriores.png" ) );
 vista->setSelectionBehavior( QAbstractItemView::SelectRows );

 modelo = new MPresupuestos( this, true, true );
 vista->setModel( modelo );
 vista->setItemDelegateForColumn( 3, new QSqlRelationalDelegate( vista ) );
 //vista->hideColumn( 0 );
 vista->hideColumn( 6 );
 vista->hideColumn( 7 );
 vista->hideColumn( 8 );
 vista->setSortingEnabled( true );
 modelo->select();

 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActEliminar );
 addAction( ActImprimir );
 //addAction( ActEmail );
 addAction( ActCerrar );
}


VPresupuestos::~VPresupuestos()
{
}

/*!
    \fn VPresupuestos::antes_de_insertar( int row, QSqlRecord & record )
 */
void VPresupuestos::antes_de_insertar( int row, QSqlRecord & record )
{}

#include "formnuevopresupuesto.h"
/*!
    \fn VPresupuestos::agregar( bool autoeliminarid )
 */
void VPresupuestos::agregar(  bool autoeliminarid  )
{ emit agregarVentana( new FormNuevoPresupuesto() ); }


#include "formmodificarpresupuesto.h"
#include <QMessageBox>
/*!
    \fn VPresupuestos::modificar()
 */
void VPresupuestos::modificar()
{
 QItemSelectionModel *selectionModel = vista->selectionModel();
 QModelIndexList indices = selectionModel->selectedRows();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para modificar",
                   QMessageBox::Ok );
   return;
 }
 FormModificarPresupuesto *form = new FormModificarPresupuesto();
 form->setId( modelo->data( modelo->index( indices.at(0).row(), 0 ), Qt::EditRole ).toInt() );
 form->setModelo( this->modelo );
 // Consigo el numero de presupuesto que se quiere modificar
 emit agregarVentana( form );
}


/*!
    \fn VPresupuestos::imprimir()
 */
void VPresupuestos::imprimir()
{
 QItemSelectionModel *selectionModel = vista->selectionModel();
 QModelIndexList indices = selectionModel->selectedRows();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para imprimir",
                   QMessageBox::Ok );
   return;
 }
 EReporte *reporte = new EReporte( this );
 reporte->setArchivo( "plugins/presupuestos/informe-presupuestador.xml" );
 reporte->agregarParametro( "num_presupuesto", modelo->data( modelo->index( indices[0].row(), 0 ), Qt::EditRole ) );
 reporte->previsualizar();
}
