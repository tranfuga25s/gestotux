 /***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   	   			   *
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
#include "formventa.h"
#include "emovimiento.h"
#include "mcaravanadueno.h"

#include <QSqlQueryModel>
#include <QCompleter>
#include <QSqlQuery>
#include <QSqlError>
#include <QProgressDialog>
#include <QSqlRecord>
#include <QMessageBox>

FormVenta::FormVenta(QWidget* parent, Qt::WFlags fl ):
FormMovimiento(parent, fl, venta )
{
 LTitulo->setText( "Agregar Venta de Caravanas" );
 // Verificar si las caravanas provienen del mismo establecimiento siempre - SI
 // si es asi, verificar que no se coloquen caravanas que no existen en ese establecimiento
 // Creo el autocompletar con los datos
 CBEstablecimientoOrigen->setCurrentIndex( -1 );
 //connect( CBEstablecimientoOrigen, SIGNAL( currentIndexChanged( int ) ), this, SLOT( filtrarPorEstablecimiento( int ) ) );
}


FormVenta::~FormVenta()
{
}

/*!
    \fn FormVenta::cerrar()
 */
void FormVenta::cerrar()
{
 close();
}


/*!
    \fn FormVenta::guardar()
 */
void FormVenta::guardar()
{
 if( !verificar() )
 {
  return;
 }
 QProgressDialog *dialogo = new QProgressDialog( this );
 dialogo->setLabelText( "Guardando datos del TRI" );
 dialogo->setMinimum( 0 );
 dialogo->setMaximum( 7 );
 dialogo->setValue( 0 );
 dialogo->setMinimumDuration( 0 );
 dialogo->show();
 EMovimiento *movimiento = new EMovimiento( this );
 // Tipo de Movimiento
 movimiento->setTipoMov( venta );
 dialogo->setValue( dialogo->value() + 1 );
 // DTA
 if( !movimiento->setDTA( LEDTA->text() ) )
 {
	dialogo->close();
	return;
 }
 dialogo->setValue( dialogo->value() + 1 );
 movimiento->setNumGuia( LEGuia->text() );
 dialogo->setValue( dialogo->value() + 1 );
 // Fecha
 movimiento->setFecha( dEFecha->date() );
 dialogo->setValue( dialogo->value() + 1 );
 // Categoria
 movimiento->setCategoria( CBCategoria->model()->data( CBCategoria->model()->index( CBCategoria->currentIndex(), 0 ), Qt::EditRole ).toInt() );
 dialogo->setValue( dialogo->value() + 1 );
 // Establecimiento de origen
 movimiento->setEstablecimientoOrigen( CBEstablecimientoOrigen->model()->data( CBEstablecimientoOrigen->model()->index( CBEstablecimientoOrigen->currentIndex(), 0 ), Qt::EditRole ).toInt() );
 dialogo->setValue( dialogo->value() + 1 );
 // Comprador
 movimiento->setComprador( CBCliente->model()->data( CBCliente->model()->index( CBCliente->currentIndex(), 0 ), Qt::EditRole ).toInt() );
 dialogo->setValue( dialogo->value() + 1 );
 if( _especial )
 {
   movimiento->setCantidadAnimales( SBCantidadAnimales->value() );
 }
 else
 {
	// Chequear que no existan los numeros de caravanas ya en la tabla
	// Busco todos los codigos dados de alta en la tabla de caravanas
	dialogo->setLabelText( "Comprobando caravanas..." );
	QStringList lista = model->listaCaravanas();
	dialogo->setRange( 0, lista.size() );
	dialogo->setValue( 0 );
	QString cadena;
	foreach( cadena, lista )
	{
		movimiento->agregarCaravana( cadena );
	}
 }// fin de si es especial o no
 // Lista la comprobacion de caravanas
 // comienzo a guardar todo
 dialogo->setLabelText( "Guardando datos..." );
 int id = movimiento->guardar( dialogo );
 if( id < 0 )
 {
	QMessageBox::critical( this, "Error al guardar los datos", "No se ha podido guardar los datos de esta compra" );
	dialogo->close();
	return;
 }
 else
 {
  QMessageBox::information( this, "Correcto", "La informacion se ha guardado correctamente");
 }
 dialogo->close();
 this->close();
}


/*!
    \fn FormVenta::filtrarPorEstablecimiento( int idCombo )
 */
void FormVenta::filtrarPorEstablecimiento( int idCombo )
{
 // Busco el id del establecimiento del combo
 qDebug( "id del combo: %i", idCombo );
 QSqlQuery cola( QString("SELECT id_establecimiento FROM car_establecimientos WHERE nombre = '%1'" ).arg( CBEstablecimientoOrigen->itemText( idCombo ) ) );
 if( !cola.next() )
 {
  qDebug( "Error al ejecutar la cola de numero de estableceimiento" );
  return;
 }
 int id_establecimiento = cola.record().value(0).toInt();
 LENumCar->setCompleter( 0 );
 if( completador == 0 )
 {
  completador = new QCompleter( LENumCar );
 }
 if( modelo == 0 )
 {
  modelo = new QSqlQueryModel( this );
 }
 // Selecciono todos los tri que tuvieron movimientos hacia ese establecimiento
 // SELECT id_tri FROM car_tri WHERE id_estab_destino = '%1'
 // Despues busco todos las caravanas que estuvieron en esos tri
 // SELECT id_caravana FROM car_carav_tri WHERE id_tri IN (  )
 // Selecciono los codigos que se encuentran en los establecimientos
 modelo->setQuery( QString( "SELECT codigo FROM car_caravana WHERE id_caravana IN ( SELECT id_caravana FROM car_carv_tri WHERE id_tri IN ( SELECT id_tri FROM car_tri WHERE id_estab_destino = '%1' ) )" ).arg( id_establecimiento ) );
 /*if ( !modelo->lastError().isValid() )
 {
   qWarning( QString( "Error al colocar el modelo de autocompletado. Error: %1" ).arg( modelo->lastError().text() ).toLocal8Bit() );
   return;
 }*/
 completador->setModel( modelo );
 completador->setCompletionMode( QCompleter::UnfilteredPopupCompletion );
 LENumCar->setCompleter( completador );
 qDebug( "Seteado el autocompletado" );
}
