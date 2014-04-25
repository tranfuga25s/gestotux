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
#include "formnuevopresupuesto.h"
#include "presupuestador.h"
#include "visorresumen.h"
#include "eeditor.h"
#include "emcliente.h"
#include "emautos.h"

#include <QVBoxLayout>

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>
#include "ereporte.h"

FormNuevoPresupuesto::FormNuevoPresupuesto(QWidget* parent, Qt::WFlags fl)
: EVentana( parent, fl ), Ui::FormNuevoPresupuestoBase()
{
	setupUi(this);
	this->setAttribute( Qt::WA_DeleteOnClose );
	DTFecha->setDate( QDate::currentDate() );
	dSBTotal->setValue( 0 );

	QAction *ActGuardar= new QAction( "Guardar", this );
	ActGuardar->setIcon( QIcon( ":/imagenes/add.png" ) );
	connect( ActGuardar, SIGNAL( triggered() ), this, SLOT( agregar() ) );

	QAction *ActCancelar = new QAction( "Cancelar", this );
	ActCancelar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
	connect( ActCancelar, SIGNAL( triggered() ), this, SLOT( close() ) );

	// Creo el editor
	editor = new EEditor( this );
	QVBoxLayout *v = new QVBoxLayout( GBContenido );
	v->addWidget( editor );

	// Genero el modelo de los autos para no tener problema con el slot
	modeloautos = new EMAutos( CBAuto );
	// Cargo los Clientes
	CBCliente->setModel( new EMCliente( CBCliente ) );
	CBCliente->setModelColumn( 1 );
	CBCliente->setCurrentIndex( -1 );
	connect( CBCliente, SIGNAL( currentIndexChanged( int ) ), this, SLOT( filtrarPorCliente( int ) ) );

	// Cargo los Autos
	CBAuto->setModel( modeloautos );
	CBAuto->setModelColumn( 1 );
	CBAuto->setCurrentIndex( -1 );


	setWindowTitle( "Nuevo Presupuesto" );
        LETitulo->setEnabled( CkBTitulo->isChecked() );
	setObjectName( "NuevoPresupuesto" );
	setWindowIcon( QIcon( ":/imagenes/presupuesto.png" ) );
	addAction( ActGuardar );
	addAction( ActCancelar );
}

FormNuevoPresupuesto::~FormNuevoPresupuesto()
{
}

#include "mpresupuestos.h"
#include <QSqlRecord>
#include <QMessageBox>

/*!
    \fn FormNuevoPresupuesto::agregar()
 */
void FormNuevoPresupuesto::agregar()
{
 if( CkBTitulo->isChecked() && LETitulo->text().isEmpty() )
 {
 QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un titulo para el presupuesto o desseleccione la opcion de titulo personalizado" );
  return;
 }
 if( editor->contenido( Qt::AutoText ).isEmpty() )
 {
  QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un detalle de presupuesto" );
  return;
 }
 if( dSBTotal->value() <= 0 )
 {
  QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un monto a presupuestar" );
  return;
 }
 if( SBKilometraje->value() <= 0 )
 {
  QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un kilometraaje valido" );
  return;
 }
 // un cliente y un auto tienen que estar seleccionados
 if( CBCliente->currentText().isEmpty() )
 {
  QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un cliente valido" );
  return;
 }
 if( CBAuto->currentText().isEmpty() )
 {
  QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un auto valido" );
  return;
 }
 //Agrego el registro
 MPresupuestos *presupuesto = new MPresupuestos( this );
 presupuesto->setEditStrategy( QSqlTableModel::OnManualSubmit );
 QSqlRecord registro = presupuesto->record();
 registro.remove( 0 );
 registro.setValue( "fecha", DTFecha->date() );
 if( CkBTitulo->isChecked() )
 { registro.setValue( "titulo", LETitulo->text() ); }
 else { registro.setValue( "titulo", "Presupuesto" ); }
 registro.setValue( "id_auto", CBAuto->model()->data( CBAuto->model()->index( CBAuto->currentIndex(), 0 ) ).toInt() );
 registro.setValue( "kilometraje", SBKilometraje->value() );
 registro.setValue( "contenido", editor->contenido( Qt::RichText ) );
 if( GBMemo->isChecked() )
 { registro.setValue( "memo", TBMemo->toPlainText() ); }
 else  { registro.setNull( "memo" ); }
 registro.setValue( "total", dSBTotal->value() );
 registro.setValue( "creado", QDateTime::currentDateTime() );
 registro.setValue( "modificado", QDateTime::currentDateTime() );
 if( presupuesto->insertRecord( -1, registro ) )
 {
  // Registro agregado correctamente
  // Obtengo el num de presupuesto antes de hacer "SUBMIT"
  if( presupuesto->submitAll() )
  {
    // obtengo el numero de presupuesto
	  int num_presupuesto = presupuesto->query().lastInsertId().toInt();
	  QMessageBox mensaje;
	  mensaje.setText( QString( "El presupuesto se guardo correctamente con el numero %1.\n\n ¿Que desea hacer a continuacion?" ).arg( num_presupuesto ) );

	  QPushButton *Bimprimir = mensaje.addButton( tr( "Imprimir" ), QMessageBox::ResetRole );
	  Bimprimir->setIcon( QIcon( ":/imagenes/imprimir.png" ) );

	  /*QPushButton *Bemail = mensaje.addButton( tr( "Enviar por email" ), QMessageBox::ApplyRole );
	  Bemail->setIcon( QIcon( ":/imagenes/email.png" ) );*/

	  mensaje.addButton( tr( "Solo Guardar" ), QMessageBox::AcceptRole );

	  mensaje.exec();
	  if( mensaje.clickedButton() == Bimprimir )
	  {
		// Imprimir
		EReporte *reporte = new EReporte( this->parent() );
		reporte->setArchivo( "plugins/presupuestos/informe-presupuestador.xml" );
		reporte->agregarParametro( "num_presupuesto", num_presupuesto );
		reporte->previsualizar();
		this->close();
	  }
	  /*else if( mensaje.clickedButton() == Bemail )
	  {
		 // Enviar x email
	  }*/
          else
	  {
		close();
	  }
  }
  else
  {
    // Error de submit
    qWarning( "Error al hacer el submit de los datos" );
    qWarning( qPrintable( presupuesto->query().lastError().text() ) );
    qWarning( qPrintable( presupuesto->query().lastQuery() ) );
    return;
   }
 }
 else
 {
  qWarning( "No se pudo agregar el presupuesto -  Error de insert" );
  qWarning( qPrintable( presupuesto->query().lastError().text() ) );
  qWarning( qPrintable( presupuesto->query().lastQuery() ) );
 }
}


/*!
    \fn FormNuevoPresupuesto::filtrarPorCliente( int indice )
 */
void FormNuevoPresupuesto::filtrarPorCliente( int indice )
{
 modeloautos->filtrarPorCliente( CBCliente->model()->data( CBCliente->model()->index( indice, 0 ), Qt::EditRole ).toInt() );
}
