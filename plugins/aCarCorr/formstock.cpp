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
#include "formstock.h"

#include <QLabel>
#include <QSqlQuery>
#include <QProgressDialog>
#include "emovimiento.h"
#include <QMessageBox>
#include "mcaravanadueno.h"
#include <QSqlError>
#include "TipoMovs.h"

FormStock::FormStock( QWidget *parent, Qt::WFlags f )
 : FormMovimiento( parent, f, stock )
{
  LTitulo->setText( "Agregar nuevas caravanas desde Stock" );
}


FormStock::~FormStock()
{
}


void FormStock::cerrar()
{
 this->close();
}

void FormStock::guardar()
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
 dialogo->show();
 EMovimiento *movimiento = new EMovimiento( this );
 // Tipo de Movimiento
 movimiento->setTipoMov( stock );
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

