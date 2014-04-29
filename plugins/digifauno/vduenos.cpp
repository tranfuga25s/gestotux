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
#include "vduenos.h"
#include "mdueno.h"
#include "dmiembros.h"
#include <QTableView>
#include <QSqlRecord>
#include <QPushButton>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QAction>
#include <QHBoxLayout>
#include <QSplitter>
#include <QFrame>

VDuenos::VDuenos( QWidget *parent )
 : EVLista( parent )
{
 setObjectName( "duenos" );
 setWindowIcon( QIcon( ":/imagenes/duenos.png" ) );
 setWindowTitle( "Listado de Dueños" );
 modelo = new MDueno( this );
 vista->setModel( modelo );
 vista->hideColumn( 0 ); // oculta el ID
 modelo->select();
 while( modelo->canFetchMore() )
 {
  modelo->fetchMore();
 }
 vista->setItemDelegate( new DMiembros( vista ) );
 vista->setSortingEnabled( true );

 ActAgregar->setIcon( QIcon( ":/imagenes/add_user.png" ) );
 ActModificar->setIcon( QIcon( ":/imagenes/edit_user.png" ) );
 ActEliminar->setIcon( QIcon( ":/imagenes/delete_user.png" ) );

 addAction( ActAgregar );
 addAction( ActModificar );
 addAction( ActEliminar );


 forden = new QFrame( this );

 QLabel *etiqueta = new QLabel( "Ordenar por:", forden );
 QHBoxLayout *orden = new QHBoxLayout( forden );
 orden->addWidget( etiqueta );
 CBOrden = new QComboBox( this );
 CBOrden->insertItem( 0, "Nombre", 1 );
 CBOrden->insertItem( 1, "Apellido", 2 );
 connect( CBOrden, SIGNAL( currentIndexChanged ( const QString & ) ), this, SLOT( cambioOrden( const QString & ) ) );
 orden->addWidget( CBOrden );
 ChBAscendente = new QCheckBox( "Descendente", this );
 connect( ChBAscendente, SIGNAL( stateChanged( int ) ), this, SLOT( cambioOrdenSentido( int ) ) );
 orden->addWidget( ChBAscendente );
 forden->hide();
 this->layout()->addWidget( forden );

 // Accion para mostrar o ocultar el filtro
 ActFiltrar = new QAction( "Filtrar", this );
 ActFiltrar->setIcon( QIcon( ":/imagenes/search_user.png" ) );
 ActFiltrar->setStatusTip( "Muestra el formulario para filtrar los miembros" );
 ActFiltrar->setCheckable( true );
 ActFiltrar->setChecked( false );
 connect( ActFiltrar, SIGNAL( toggled ( bool ) ), this, SLOT( cambioFiltro( bool ) ) );

 addAction( ActFiltrar );
 addAction( ActCerrar );
}


VDuenos::~VDuenos()
{
}

/*!
    \fn VDuenos::antes_de_insertar( int row, QSqlRecord & record )
	Este metodo llamado antes de insertar los datos de un registro que setea los valores predeterminados.
	@param row Numero de registro
	@param record Objeto de registro
 */
void VDuenos::antes_de_insertar( int row, QSqlRecord & record )
{
  record.remove( 0 );
  for( int i = 0; i<record.count(); i++ )
  {
   record.setValue( i, QVariant( " " ) );
  }
}


/*!
    \fn VDuenos::eliminar()
 */
void VDuenos::eliminar()
{
 //Preguntar al usuario si esta seguro
 QItemSelectionModel *selectionModel = vista->selectionModel();
 QModelIndexList indices = selectionModel->selectedIndexes();
 QModelIndex indice;
 foreach( indice, indices )
 {
  QSqlQuery cola( QString( "SELECT COUNT(id) FROM mascotas WHERE id_dueno = %1" ).arg( modelo->data( modelo->index( indice.row(), 0 ), Qt::UserRole ).toInt() ) );
  if( cola.next() )
  {
   if( cola.record().value( 0 ).toInt() > 0 )
   {
    QMessageBox::warning( this, "No se puede eliminar", "No se puede eliminar este dueño, ya que tiene mascotas asociadas.\n Si desea eliminarlo cambie de dueño a las mascotas o eliminelas e intentelo nuevamente" );
    return;
   }
   else
   {
	return EVLista::eliminar();
   }
  }
  else
  {
 	qDebug( "Error, al generar la cola de obtencion de cantidad de mascotas" );
	qDebug( QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( cola.lastError().type() ).arg( cola.lastError().number() ).arg( cola.lastError().text() ).toLocal8Bit() );
	return;
  }
 }
}


/*!
    \fn VDuenos::cambioOrden( const QString &columna )
 */
void VDuenos::cambioOrden( const QString &columna )
{
 vista->setSortingEnabled( true );
 vista->sortByColumn( CBOrden->itemData( CBOrden->currentIndex(), Qt::EditRole ).toInt()-1,
                      ChBAscendente->isChecked() ? Qt::DescendingOrder : Qt::AscendingOrder );
}


/*!
    \fn VDuenos::cambioOrdenSentido( int estado )
 */
void VDuenos::cambioOrdenSentido( int estado )
{
 vista->setSortingEnabled( true );
 vista->sortByColumn( CBOrden->itemData( CBOrden->currentIndex(), Qt::EditRole ).toInt() -1,
                      ChBAscendente->isChecked() ? Qt::DescendingOrder : Qt::AscendingOrder );
}


/*!
    \fn VDuenos::cambioFiltro( bool estado )
 */
void VDuenos::cambioFiltro( bool estado )
{
 forden->setVisible( estado );
}


/*!
    \fn VDuenos::menuContextual( const QModelIndex &indice, QMenu *menu )
 */
void VDuenos::menuContextual( const QModelIndex &indice, QMenu *menu )
{
    /// @todo implement me
}
