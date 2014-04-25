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

#include "FormClientesAdheridos.h"

#include "mservicios.h"
#include "eactimprimir.h"
#include "eactpdf.h"
#include "MClientesServicios.h"
#include "eactcerrar.h"
#include "EReporte.h"
#include "ecbtabla.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QTableView>

FormClientesAdheridos::FormClientesAdheridos(QWidget *parent) :
    EVentana(parent) {
    setupUi(this);

    setObjectName( "ClientesAdheridos" );
    setWindowTitle( "Clientes Adheridos" );
    setWindowIcon( QIcon( ":/imagenes/clientes_adheridos.png" ) );

    CBServicios->setearTabla( "servicios" );
    CBServicios->setearCampoId( "id_servicio" );
    CBServicios->setearCampoTexto( "nombre" );
    CBServicios->setearCampoOrden( "nombre" );
    connect( CBServicios, SIGNAL( cambioId( int ) ), this, SLOT( cambioServicio( int ) ) );

    modelo = new MClientesServicios( this );
    TVAdheridos->setModel( modelo );
    TVAdheridos->hideColumn( 0 );
    TVAdheridos->hideColumn( 3 );
    TVAdheridos->hideColumn( 4 );
    TVAdheridos->hideColumn( modelo->fieldIndex( "idcliente" ) );
    TVAdheridos->hideColumn( modelo->fieldIndex( "idservicio" ) );
    TVAdheridos->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVAdheridos->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    TVAdheridos->setSortingEnabled( true );
    //modelo->select();

    // Acciones
    //// Dar de baja
    QAction *ActDarDeBaja = new QAction( this );
    ActDarDeBaja->setText( "Dar de Baja" );
    connect( ActDarDeBaja, SIGNAL( triggered() ), this, SLOT( darDeBaja() ) );
    this->addAction( ActDarDeBaja );

    //// Eliminar adhesion
    QAction *ActEliminar = new QAction( this );
    ActEliminar->setText( "Eliminar" );
    ActEliminar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
    connect( ActEliminar, SIGNAL( triggered() ), this, SLOT( eliminar() ) );
    this->addAction( ActEliminar );

    QAction *ActVerBaja = new QAction( this );
    ActVerBaja->setText( "Ver solo baja" );
    ActVerBaja->setStatusTip( "Muestra solamente los clientes que han sido dados de baja del servicio" );
    ActVerBaja->setCheckable( true );
    ActVerBaja->setChecked( false );
    connect( ActVerBaja, SIGNAL( toggled( bool ) ), this, SLOT( verBaja( bool ) ) );
    this->addAction( ActVerBaja );

    this->addAction( new EActImprimir( this ) );
    this->addAction( new EActPdf( this ) );

    this->addAction( new EActCerrar( this ) );

}

void FormClientesAdheridos::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/*!
 * \brief FormClientesAdheridos::setServicioInicial
 * \param id_servicio
 */
void FormClientesAdheridos::setServicioInicial( int id_servicio )
{
    if( id_servicio <= 0 )
        return;

    CBServicios->setearId( id_servicio );
    modelo->filtrarPorServicio( id_servicio );
    modelo->select();

}

/*!
 * \brief FormClientesAdheridos::cambioServicio
 * \param id_servicio
 */
void FormClientesAdheridos::cambioServicio( int id_servicio )
{
    if( id_servicio <= 0 )
        return;

    modelo->filtrarPorServicio( id_servicio );
    modelo->select();
}

/*!
 * \brief FormClientesAdheridos::darDeBaja
 */
void FormClientesAdheridos::darDeBaja()
{
 // Busco el ID que quiere dar de baja
 QModelIndexList lista = TVAdheridos->selectionModel()->selectedRows();
 if( lista.isEmpty() ) {
     QMessageBox::information( this, "Error", "Por favor, seleccione algun cliente adherido para darlo de baja" );
     return;
 }
  int id_servicio = CBServicios->idActual();
  foreach( QModelIndex item, lista ) {
      int id_cliente = modelo->data( modelo->index( item.row(), modelo->fieldIndex( "idcliente" ) ), Qt::EditRole ).toInt();
      bool ok = false;
      QString razon = QInputDialog::getText( this, "Razon de baja", "Ingrese la razon de baja:", QLineEdit::Normal, QString(), &ok );
      if( ok )
        modelo->darDeBaja( id_cliente, id_servicio, razon );
  }
  modelo->select();
}

/*!
 * \brief FormClientesAdheridos::eliminar
 * Elimina la relación de algun cliente que ha sido dado de baja de un servicio
 */
void FormClientesAdheridos::eliminar() {
    // Busco el ID que quiere dar de baja
    QModelIndexList lista = TVAdheridos->selectionModel()->selectedRows();
    if( lista.isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, seleccione algun cliente dado de baja para eliminarlo" );
        return;
    }
     int id_servicio = CBServicios->idActual();
     foreach( QModelIndex item, lista ) {
         int id_cliente = modelo->data( modelo->index( item.row(), modelo->fieldIndex( "idcliente" ) ), Qt::EditRole ).toInt();
         QDate fecha_baja = item.model()->data( item.model()->index( item.row(), modelo->fieldIndex( "fecha_baja" ) ), Qt::EditRole ).toDate();
         if( !fecha_baja.isValid() ) {
             QMessageBox::information( this, "Error", QString::fromUtf8( "El cliente que está intentando eliminar no ha sido dado de baja todavía. Delo de baja antes de eliminar la asociación." ) );
         } else {
             if( modelo->eliminarRelacion( id_cliente, id_servicio ) ) {
                 QMessageBox::information( this, "Correcto", QString::fromUtf8( "La asociacion del cliente al servicio ha sido eliminada. No así sus datos historicos" ) );
             } else {
                 QMessageBox::information( this, "Error", QString::fromUtf8( "No se pudo eliminar la asociacion del cliente al servicio." ) );
             }
         }
     }
     modelo->select();
}

/*!
 * \brief FormClientesAdheridos::imprimir
 */
void FormClientesAdheridos::imprimir()
{
    EReporte *rep = new EReporte( 0 );
    ParameterList lista;

    lista.append( Parameter( "id_servicio", CBServicios->idActual() ) );
    if( !modelo->filter().isEmpty() )
        lista.append( Parameter( "filtro", modelo->filter() ) );
    rep->especial( "ListadoClientesServicio", lista );

    rep->hacer();
    delete rep;
}

/*!
 * \brief FormClientesAdheridos::aPdf
 */
void FormClientesAdheridos::aPdf()
{
   EReporte *rep = new EReporte( 0 );
   ParameterList lista;
   lista.append( Parameter( "id_servicio", CBServicios->idActual() ) );

   if( !modelo->filter().isEmpty() )
    lista.append( Parameter( "filtro", modelo->filter() ) );

   rep->especial( "ListadoClientesServicio", lista );
   rep->hacerPDF( lista, QString( "Listado de clientes adheridos a %1 al %2 " ).arg( CBServicios->currentText() ).arg( QDate::currentDate().toString( Qt::SystemLocaleShortDate ) ) );

   delete rep;
}

/*!
 * \brief FormClientesAdheridos::verBaja
 * \param estado
 */
void FormClientesAdheridos::verBaja( bool estado )
{
    if( estado ) {
        TVAdheridos->showColumn( modelo->fieldIndex( "fecha_baja" ) );
        TVAdheridos->showColumn( modelo->fieldIndex( "razon" ) );
    } else {
        TVAdheridos->hideColumn( modelo->fieldIndex( "fecha_baja" ) );
        TVAdheridos->hideColumn( modelo->fieldIndex( "razon" ) );
    }
    modelo->setearVerBaja( estado );
    modelo->select();
}
