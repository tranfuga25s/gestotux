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

#include "VResumenCaja.h"

#include <QIcon>
#include <QTableView>
#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>
#include <QDateEdit>
#include <QLineEdit>

#include "mmovimientoscaja.h"
#include "mcajas.h"
#include "actingresocaja.h"
#include "actretirocaja.h"
#include "acttransferir.h"
#include "FormRetiroIngreso.h"
#include "formtransferir.h"
#include "FormCierreCaja.h"

#include "EReporte.h"
#include "ecbtabla.h"

VResumenCaja::VResumenCaja( QWidget *parent )
: EVLista( parent )
{
  this->setObjectName( "resumenCaja" );
  this->setWindowTitle( "Resumen de caja" );
  this->setWindowIcon( QIcon( ":/imagenes/cajaresumen.png" ) );

  // Inicializo el modelo de la vista
  modelo = new MMovimientosCaja( this );
  vista->setModel( modelo );
  vista->hideColumn( 1 );
  vista->hideColumn( 0 );
  vista->hideColumn( modelo->fieldIndex( "cierre" ) );
  vista->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
  vista->horizontalHeader()->setResizeMode( 5, QHeaderView::Stretch );
  vista->horizontalHeader()->setMinimumSectionSize( 60 );
  LSaldo = new QLabel( this );
  LSaldo->setText( "<b>Saldo actual:</b>" );
  CBCajas = new ECBTabla( this );
  // Reordena los items para que el combo box este arriba
  QGridLayout *l = qobject_cast<QGridLayout *>(this->layout());
  QLayoutItem *i = l->takeAt( 0 );
  l->addWidget( CBCajas, 0, 0 );
  l->addWidget( LSaldo, 1, 0 );
  l->addItem( i, 2, 0 );

  // Genero los items para el filtrado
  GBFiltrado = new QGroupBox( this );
  GBFiltrado->setTitle( "Filtrar movimientos de caja" );
  QGridLayout *lg = new QGridLayout();
  GBFiltrado->setLayout( lg );

  QLabel *linicio = new QLabel( GBFiltrado );
  QLabel *lfin = new QLabel( GBFiltrado );
  linicio->setText( "Desde:" );
  lfin->setText( " Hasta: " );

  DTEFin = new QDateEdit( GBFiltrado );
  DTEFin->setMaximumDate( QDate::currentDate() );
  DTEFin->setDate( QDate::currentDate() );
  DTEFin->setDisplayFormat( "dd/MM/yyyy" );

  DTEInicio = new QDateEdit( GBFiltrado );
  DTEInicio->setMaximumDate( QDate::currentDate().addDays( -1 ) );
  DTEInicio->setDisplayFormat( "dd/MM/yyyy" );
  connect( DTEFin, SIGNAL( dateChanged( QDate ) ), this, SLOT( actualizarFiltro() ) );
  connect( DTEInicio, SIGNAL( dateChanged( QDate ) ), this, SLOT( actualizarFiltro() ) );

  LTexto = new QLabel( GBFiltrado );
  LTexto->setText( "Buscar en movimiento: " );

  LETexto = new QLineEdit( GBFiltrado );
  connect( LETexto, SIGNAL( textEdited( QString ) ), this, SLOT( actualizarFiltro() ) );

  lg->addWidget( linicio, 0, 0 );
  lg->addWidget( DTEInicio, 0, 1 );
  lg->addWidget( lfin, 0, 2 );
  lg->addWidget( DTEFin, 0, 3 );
  lg->addWidget( LTexto, 0, 4 );
  lg->addWidget( LETexto, 0, 5 );
  GBFiltrado->setVisible( false );
  l->addWidget( GBFiltrado, 3, 0 );

  // Inicializo el modelo de las cajas, lo pongo en el combobox y conecto las señales para cambiar los datos
  CBCajas->setearTabla( "caja" );
  CBCajas->setearCampoId( "id_caja" );
  CBCajas->setearCampoTexto( "nombre" );
  CBCajas->setearCampoOrden( "nombre" );
  connect( CBCajas, SIGNAL( cambioId( int ) ), this, SLOT( cambioCaja( int ) ) );

  this->vista->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );

  ActFiltrar = new QAction( this );
  ActFiltrar->setText( "Filtrar" );
  ActFiltrar->setCheckable( true );
  ActFiltrar->setIcon( QIcon( ":/imagenes/buscar.png" ) );
  connect( ActFiltrar, SIGNAL( triggered() ), this, SLOT( filtrar() ) );

  ActCierre = new QAction( this );
  ActCierre->setText( "Cerrar caja" );
  ActCierre->setIcon( QIcon( ":/imagenes/cierrecaja.png" ) );
  ActCierre->setStatusTip( "Genera un cierre para la caja seleccionada" );
  connect( ActCierre, SIGNAL( triggered() ), this, SLOT( hacerCierre() ) );

  QAction *ActSep = new QAction( this );
  ActSep->setSeparator( true );

  QAction *ActSep2 = new QAction( this );
  ActSep2->setSeparator( true );

  this->addAction( new ActIngresoCaja( this ) );
  this->addAction( new ActRetiroCaja( this ) );
  if( MCajas::existen2omas() ) {
    this->addAction( new ActTransferir( this ) );
  } else {
      CBCajas->setCurrentIndex( 1 );
      CBCajas->setEnabled( false );
  }
  this->addAction( ActCierre );
  this->addAction( ActSep );
  this->addAction( ActImprimir );
  this->addAction( ActPdf );
  this->addAction( ActCerrar );
  this->addAction( ActSep2 );
  this->addAction( ActFiltrar );
}

void VResumenCaja::setearCaja(int id_caja)
{
    CBCajas->setearId( id_caja );
    cambioCaja( id_caja );
}


void VResumenCaja::cambioCaja( int id )
{
    if( id > 0 )
    {
        modelo->ultimosMovimientosCaja( id );
        LSaldo->setText( QString( "<b>Saldo Actual:</b> $ %L1" ).arg( MCajas::saldo( id ), 10, 'f', 2 ) );
    }
}

void VResumenCaja::ingreso() {
    FormRetiroIngreso *f = new FormRetiroIngreso( this, FormRetiroIngreso::Ingreso );
    f->setearCaja( CBCajas->idActual() );
    f->exec();
    modelo->ultimosMovimientosCaja( CBCajas->idActual() );
}

void VResumenCaja::retiro() {
    FormRetiroIngreso *f = new FormRetiroIngreso( this, FormRetiroIngreso::Retiro );
    f->setearCaja( CBCajas->idActual() );
    f->exec();
    modelo->ultimosMovimientosCaja( CBCajas->idActual() );
}

void VResumenCaja::transferir() {
    FormTransferir *ft = new FormTransferir();
    ft->setearCajaOrigen( CBCajas->idActual() );
    ft->exec();
}

void VResumenCaja::imprimir()
{
    EReporte *rep = new EReporte( 0 );

    ParameterList parametros;
    parametros.append( "id_caja", CBCajas->idActual() );
    if( ActFiltrar->isChecked() ) {
        parametros.append( "filtro", this->modelo->filter() );
    }
    rep->especial( "ResumenCaja", parametros );
    rep->hacer();

    delete rep;
}

void VResumenCaja::aPdf()
{
    EReporte *rep = new EReporte( 0 );

    ParameterList parametros;
    parametros.append( "id_caja", CBCajas->idActual() );
    if( ActFiltrar->isChecked() ) {
        parametros.append( "filtro", this->modelo->filter() );
    }
    rep->especial( "ResumenCaja", parametros );
    rep->hacerPDF( parametros, "Resumen de caja " + CBCajas->currentText() );

    delete rep;
}

void VResumenCaja::filtrar()
{
    if( ActFiltrar->isChecked() ) {
        GBFiltrado->setVisible( true );
    } else {
        GBFiltrado->setVisible( false );
        this->modelo->setFilter( "" );
        this->modelo->select();
    }
}

void VResumenCaja::actualizarFiltro()
{
    QString filtro = QString();
    filtro.append( QString( " id_caja = %1 " ).arg( CBCajas->idActual() ) );
    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {
        filtro.append( QString( " AND fecha_hora >= '%1T00:00:00' AND fecha_hora <= '%2T23:59:59' " )
                                 .arg( DTEInicio->date().toString( "yyyy-MM-dd" ) )
                                 .arg( DTEFin->date().toString( "yyyy-MM-dd" ) ) );
    } else if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {
        filtro.append( QString( " DATE( fecha_hora ) >= DATE( '%1' ) AND DATE( fecha_hora ) <= DATE( '%2' ) " )
                                 .arg( DTEInicio->date().toString( "yyyy-MM-dd" ) )
                                 .arg( DTEFin->date().toString( "yyyy-MM-dd" ) ) );
    }
    if( !LETexto->text().isEmpty() ) {
        filtro.append( QString( " AND razon LIKE \"%%%1%%\" " ).arg( LETexto->text() ) );
    }
    this->modelo->setFilter( filtro );
    this->modelo->select();
}

void VResumenCaja::hacerCierre()
{ emit agregarVentana( new FormCierreCaja() ); }
