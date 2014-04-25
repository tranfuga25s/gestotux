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
#include "formlistacompras.h"
#include <QPushButton>
#include <QKeySequence>
#include "digifauno.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include "mcompra.h"

FormListaCompras::FormListaCompras( QWidget* parent )
: EVentana( parent ), Ui::FormListaComprasBase()
{
	setupUi(this);
	nombre_ventana = "lista_compras";
	setNombreVentana( tr( "Listado de compras hechas" ) );
	setObjectName( "FormListaCompras" );

	PBCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
	PBCerrar->setText( tr("Cer&rar" ) );
	PBCerrar->setShortcut( QKeySequence( tr( "Ctrl+r" ) ) );
	connect( PBCerrar, SIGNAL( clicked() ), this, SLOT( close() ) );

	PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
	PBAgregar->setText( tr( "&Agregar Compra" ) );
	PBAgregar->setShortcut( QKeySequence( tr( "Ctrl+a" ) ) );

	PBBorrar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
	PBBorrar->setText( tr( "&Borrar" ) );
	PBBorrar->setShortcut( QKeySequence( tr( "Ctrl+b" ) ) );
	connect( PBBorrar, SIGNAL( clicked() ), this, SLOT( borrar() ) );

	PBProductos->setIcon( QIcon( ":/imagenes/productos.jpg" ) );
	PBProductos->setText( tr( "&Ver productos" ) );
	PBProductos->setShortcut( QKeySequence( tr( "Ctrl+v" ) ) );
	connect( PBProductos, SIGNAL( clicked() ), this, SLOT( verProductos() ) );

	modelo = new MCompra( TVCompras, true );
	modelo->setEditStrategy( QSqlTableModel::OnRowChange );
	TVCompras->setModel( modelo );
	modelo->select();
	TVCompras->setSelectionBehavior( QAbstractItemView::SelectRows );
	TVCompras->resizeColumnsToContents();
	TVCompras->hideColumn( 0 );

	GBFiltrado->setChecked( false );

	QSqlQueryModel *modeloProveedor = new QSqlQueryModel( CBProveedor );
	modeloProveedor->setQuery( "SELECT id, nombre FROM proveedor" );
	CBProveedor->setModel( modeloProveedor );
	CBProveedor->setModelColumn( 1 );
	CBProveedor->setCurrentIndex( -1 );

	connect( GBFiltrado  , SIGNAL( toggled ( bool )              ), this, SLOT( cambioEstadoGrupo( bool )    ) );
	connect( ChBProveedor, SIGNAL( stateChanged ( int )          ), this, SLOT( cambioEstadoProveedor( int ) ) );
	connect( ChBFecha    , SIGNAL( stateChanged ( int )          ), this, SLOT( cambioEstadoFecha( int )     ) );
	connect( ChBProducto , SIGNAL( stateChanged ( int )          ), this, SLOT( cambioEstadoProducto( int )  ) );
	connect( CBProveedor , SIGNAL( currentIndexChanged( int )    ), this, SLOT( cambioIndiceProveedor( int ) ) );
	connect( CBProducto  , SIGNAL( currentIndexChanged( int )    ), this, SLOT( cambioIndiceProducto( int )  ) );
	connect( DEfecha     , SIGNAL( dateChanged ( const QDate & ) ), this, SLOT( cambioFecha( const QDate & ) ) );

	DigiFauno *digifauno = qobject_cast<DigiFauno *>(parent);
	connect( PBAgregar, SIGNAL( clicked() ), digifauno, SLOT( agregarCompra() ) );
	///@todo Agregar calculo de fecha maxima y minima desde la tabla para limitar el filtrado
}

FormListaCompras::~FormListaCompras()
{
}


/*!
    \fn FormListaCompras::cambioEstadoFecha(  int estado  )
 */
void FormListaCompras::cambioEstadoFecha( int estado )
{
 switch( estado )
 {
  case Qt::Unchecked:
  {
   DEfecha->setEnabled( false );
   crear_filtro();
   break;
  }
  case Qt::Checked:
  {
   DEfecha->setEnabled( true );
   crear_filtro();
   break;
  }
 }
}


/*!
    \fn FormListaCompras::cambioEstadoProveedor(  int estado  )
 */
void FormListaCompras::cambioEstadoProveedor( int estado  )
{
 switch( estado )
 {
  case Qt::Unchecked:
  {
   CBProveedor->setEnabled( false );
   crear_filtro();
   break;
  }
  case Qt::Checked:
  {
   CBProveedor->setEnabled( true );
   crear_filtro();
   break;
  }
 }
}

/*!
    \fn FormListaCompras::cambioEstadoProducto( int estado )
 */
void FormListaCompras::cambioEstadoProducto( int estado )
{
 switch( estado )
 {
  case Qt::Unchecked:
  {
   CBProducto->setEnabled( false );
   crear_filtro();
   break;
  }
  case Qt::Checked:
  {
   CBProducto->setEnabled( true );
   crear_filtro();
   break;
  }
 }
}

/*!
    \fn FormListaCompras::cambioIndiceProveedor( int indice )
 */
void FormListaCompras::cambioIndiceProveedor( int indice  )
{
//  qDebug( "Filtrando por proveedor" );
 crear_filtro();
}

/*!
    \fn FormListaCompras::cambioIndiceProducto( int indice )
 */
void FormListaCompras::cambioIndiceProducto( int indice  )
{
//  qDebug( "Filtrando por producto" );
 crear_filtro();
}

/*!
    \fn FormListaCompras::cambioFecha(  const QDate  &fecha )
 */
void FormListaCompras::cambioFecha(  const QDate &fecha )
{
//  qDebug( "Filtrando por fecha" );
 crear_filtro();
}


/*!
    \fn FormListaCompras::borrar()
 */
void FormListaCompras::borrar()
{
  //Preguntar al usuario si esta seguro
 QItemSelectionModel *selectionModel = TVCompras->selectionModel();
 QModelIndexList indices = selectionModel->selectedRows();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, tr("Seleccione un item"),
                   tr("Por favor, seleccione un item para eliminar"),
                   QMessageBox::Ok );
   return;
 }
 //Hacer dialogo de confirmacion..
 int ret;
 ret = QMessageBox::warning( this, tr("Esta seguro?"),
                   tr("Esta seguro de eliminar %1 item? <b>Atencion: Se eliminaran los registros que se utilizan para calcular totales tambien. Esta seguro?</b>").arg( indices.size() ),
                   tr("Si"), tr("No") );
 if ( ret == 0 )
 {
	QModelIndex indice;
	foreach( indice, indices )
	{
		if( indice.isValid() )
		{
		        QSqlQuery cola;
		        cola.prepare( QString( "DELETE FROM compras_productos WHERE id_compra = '%1'").arg( indice.model()->data(  indice.model()->index( indice.row(), 0 ) ).toInt() ) );
		        if( cola.exec() )
		        {
		         //qDebug( cola.executedQuery().toLocal8Bit() );
		         //qDebug( "Datos de la compra eliminadas correctamente" );
		        }
		        else
		        {
		         qDebug( "HUbo un error al eliminar los datos de productos comprados" );
		         qDebug( QString( "%1 %2 %3" ).arg( cola.lastError().text() ).toLocal8Bit() );
		         return;
		        }
			modelo->removeRow( indice.row() );
		}
	}
 }
 return;
}


/*!
    \fn FormListaCompras::cambioEstadoGrupo( bool estado )
 */
void FormListaCompras::cambioEstadoGrupo( bool estado )
{
//  qDebug( "Cambiado estado grupo" );
 if( estado )
 {
  CBProveedor->setEnabled( ChBProveedor->isChecked() );
  DEfecha->setEnabled( ChBFecha->isChecked() );
 }
 else
 {
  modelo->setFilter( "" );
  modelo->select();
 }
}


/*!
    \fn FormListaCompras::crear_filtro()
 */
void FormListaCompras::crear_filtro()
{
 bool bandera = false;
 QString filtro;
 if( ChBProveedor->isChecked() )
 {
  int id_proveedor = CBProveedor->model()->data( CBProveedor->model()->index( CBProveedor->currentIndex(), 0 ) , Qt::EditRole ).toInt();
  filtro.append( "id_proveedor = '" ).append( QString::number( id_proveedor ) ).append( "'" );
  bandera = true;
 }
 if( ChBFecha->isChecked() )
 {
  if( bandera )
  {
    filtro.append( " AND " );
  }
  filtro.append( " fecha = '" ).append( DEfecha->date().toString( "yyyy-MM-dd" ) ).append( "'" );
 }
 modelo->setFilter( filtro );
 modelo->select();
//  qDebug( QString( "Cambiado filtro a: %1" ).arg( modelo->filter() ).toLocal8Bit() );
}
