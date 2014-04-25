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

#include "formrecargos.h"

#include <QMessageBox>
#include <QSqlError>
#include <QItemDelegate>
#include "edrecargos.h"

#include "eactcerrar.h"
#include "mservicios.h"
#include "mrecargos.h"
#include "edrecargos.h"

FormRecargos::FormRecargos( QWidget *parent, Qt::WFlags fl ) :
        EVentana( parent, fl ), Ui::FormRecargoBase()
{
    setupUi(this);
    setObjectName( "recargos" );
    setWindowTitle( "Recargos" );

    // Modelo de los servicios para el combobox
    mservicios = new MServicios( this );
    CBServicios->setModel( mservicios );
    CBServicios->setModelColumn( 1 );
    mservicios->select();
    connect( CBServicios, SIGNAL( currentIndexChanged(int) ), this, SLOT( cambioServicio(int) ) );
    CBServicios->setCurrentIndex( 0 );

    // Genero las acciones
    ActAgregar = new QAction( this );
    ActAgregar->setText( "Agregar" );
    ActAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
    this->addAction( ActAgregar );

    ActEliminar = new QAction( this );
    ActEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
    ActEliminar->setText( "Eliminar" );
    this->addAction( ActEliminar );

    ActCerrar = new EActCerrar( this );
    this->addAction( ActCerrar );

    // Conexiones para los botones
    connect( ActAgregar , SIGNAL( triggered() ), this, SLOT( agregarRecargo() ) );
    connect( ActEliminar, SIGNAL( triggered() ), this, SLOT( eliminarRecargo()) );

    // Inizializo el modelo de los recargos
    mrecargos = new MRecargos( this, false );
    TVRecargos->setModel( mrecargos );
    TVRecargos->hideColumn( 0 );
    TVRecargos->hideColumn( 1 );
    TVRecargos->setItemDelegate( new EDRecargos( TVRecargos ) );
    TVRecargos->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    //TVRecargos->setItemDelegate( new EDRecargos( TVRecargos ) );

    cambioServicio( CBServicios->currentIndex() );
}

void FormRecargos::cambioServicio( int /*servicio*/ )
{
  // Busco los datos de los recargos del servicio seleccionado
  int id_servicio = mservicios->data( mservicios->index( CBServicios->currentIndex(), 0 ), Qt::EditRole ).toInt();
  mrecargos->setearServicio( id_servicio );
}

void FormRecargos::agregarRecargo()
{
 int id_servicio = CBServicios->itemData( CBServicios->currentIndex() ).toInt();
 if( MServicios::dadoDeBaja( id_servicio ) ) {
     QMessageBox::information( this, "No se puede", "No se puede agregar un nuevo recargo a un servicio que fue dado de baja" );
     return;
 }
 // Agrega un nuevo recargo a la lista del servicio seleccionado
 mrecargos->agregarRecargo();
}

void FormRecargos::eliminarRecargo()
{
 QMessageBox::information( this, "this", "Todavia no implementado!" );
 return;
 // Busco que recargo esta seleccionado
 QModelIndexList lista = TVRecargos->selectionModel()->selectedIndexes();
 if( lista.size() <= 0 ) {
     QMessageBox::information( this, "Error", "Por favor seleccione un recargo para eliminar", QMessageBox::Ok );
     return;
 }
 // Selecciono las filas unicas
 int fila = -1;
 foreach( QModelIndex ind, lista ) {
     if( fila == ind.row() ) {
         lista.removeOne( ind );
     } else {
         fila = ind.row();
     }
 }
 // Pregunto si esta seguro
 if( QMessageBox::question( this, QString::fromUtf8( "¿Esta seguro?" ), QString::fromUtf8( "¿Esta seguro que desea eliminar los items seleccionados?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ) {
     foreach( QModelIndex idx, lista ) {
         mrecargos->removeRow( idx.row() );
     }
 }
}

void FormRecargos::changeEvent(QEvent *e)
{
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void FormRecargos::setearId( const int id )
{
    mrecargos->setearServicio( id );
    //actualizo el combo box
    // Para que esto funcione el Qt::UserRole tiene que devolver el ID
    this->CBServicios->setCurrentIndex( this->CBServicios->findData( id )  );
}
