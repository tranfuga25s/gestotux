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
#include "formagregar.h"

#include "emovimiento.h"
#include "TipoMovs.h"
#include "mcaravanadueno.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QProgressDialog>
#include <QMessageBox>
#include <QLabel>

FormAgregar::FormAgregar(QWidget* parent, Qt::WFlags fl)
: FormMovimiento( parent, fl, compra )
{
 // titulo
 this->setObjectName( "agregarTri" );
 this->setWindowTitle( "Agregar Compra" );
 this->setWindowIcon( QIcon( ":/imagenes/add.png" ) );
 LTitulo->setText( "Agregar nuevas caravanas mediante una compra" );
}

FormAgregar::~FormAgregar()
{
}


/*!
    \fn FormAgregar::guardar()
 */
void FormAgregar::guardar()
{
 QProgressDialog *dialogo = new QProgressDialog( this );
 dialogo->setLabelText( "Guardando datos del TRI" );
 dialogo->setMinimum( 0 );
 dialogo->setMaximum( 7 );
 dialogo->setValue( 0 );
 //dialogo->setMinimumDuration( 0 );
 dialogo->show();
 EMovimiento *movimiento = new EMovimiento( this );
 // Tipo de Movimiento
 movimiento->setTipoMov( compra );
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
 // Establecimiento  de destino
 movimiento->setEstablecimientoDestino( CBEstablecimientoDestino->model()->data( CBEstablecimientoDestino->model()->index( CBEstablecimientoDestino->currentIndex(), 0 ), Qt::EditRole ).toInt() );
 dialogo->setValue( dialogo->value() + 1 );
 // Vendedor
 movimiento->setVendedor( CBCliente->model()->data( CBCliente->model()->index( CBCliente->currentIndex(), 0 ), Qt::EditRole ).toInt() );
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
    \fn FormAgregar::cerrar()
 */
void FormAgregar::cerrar()
{
 close();
}
