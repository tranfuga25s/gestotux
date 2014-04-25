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

#include <QSqlQuery>
#include "formmodificarpresupuesto.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>

#include "eactcerrar.h"
#include "eactguardar.h"
#include "mpresupuestos.h"
#include "emcliente.h"
#include "emautos.h"
#include "ereporte.h"

FormModificarPresupuesto::FormModificarPresupuesto( QWidget* parent, Qt::WFlags fl)
: EVentana( parent, fl ), Ui::FormNuevoPresupuestoBase()
{
	this->setAttribute( Qt::WA_DeleteOnClose );
	setupUi(this);
	setObjectName( "modificarPresupuesto" );
	setWindowTitle( "Modificar Presupuesto" );
	setWindowIcon( QIcon( ":/imagenes/presupuesto.png" ) );

	addAction( new EActGuardar( this ) );
	addAction( new EActCerrar( this ) );

	editor = new EEditor( this );
	QVBoxLayout *v = new QVBoxLayout( GBContenido );
	v->addWidget( editor );

	// Cargo los Clientes
	CBCliente->setModel( new EMCliente( CBCliente ) );
	CBCliente->setModelColumn( 1 );
	CBCliente->setCurrentIndex( -1 );

	// Cargo los Autos
	CBAuto->setModel( new EMAutos( CBAuto ) );
	CBAuto->setModelColumn( 1 );
	CBAuto->setCurrentIndex( -1 );

	modelo = new MPresupuestos( this, false );
}

FormModificarPresupuesto::~FormModificarPresupuesto()
{
}

#include <QModelIndex>
/*!
    \fn FormModificarPresupuesto::setId( int idDB )
 */
void FormModificarPresupuesto::setId( int row )
{
  if( row < 0 )
  { qWarning( "Indice Invalido" ); }
  //qDebug( qPrintable( "Cargando id: " + QString::number( row ) ) );
  modelo->setFilter( QString( "id_presupuesto = %1" ).arg( row ) );
  modelo->select();
  QSqlRecord registro = modelo->record(0);
  if( registro.isEmpty() )
  { qWarning( "Sin registros" ); return; }
  dSBTotal->setValue( registro.value( "total" ).toDouble() );
  DTFecha->setDate( registro.value( "fecha" ).toDate() );
  editor->setHtml( registro.value( "contenido" ).toString() );
  // Se busca el cliente primero para que el filtrado de cliente no moleste la busqueda del auto
  QSqlQuery cola2( QString( "SELECT id FROM clientes, autos WHERE id_auto = %1 AND autos.id_dueno = clientes.id" ).arg( registro.value( "id_auto" ).toInt() ) );
  if( cola2.next() )
  { CBCliente->setCurrentIndex( CBCliente->model()->match( CBCliente->model()->index( 0,0 ), Qt::DisplayRole, cola2.record().value(0).toInt() ).at(0).row() ); }
  else
  { CBCliente->setCurrentIndex( -1 ); qDebug( qPrintable( "Error al conseguir el cliente - " + cola2.lastError().text() + " - " + cola2.lastQuery() ) ); }
  // Coloco el auto
  CBAuto->setCurrentIndex( CBAuto->model()->match( CBAuto->model()->index( 0,0 ), Qt::DisplayRole, registro.value( "id_auto" ) ).at(0).row() );
  if( registro.value( "memo" ).isNull() )
  { GBMemo->setChecked( false ); }
  else
  { TBMemo->setText( registro.value( "memo" ).toString() );  GBMemo->setChecked( true ); }
  SBKilometraje->setValue( registro.value( "kilometraje" ).toDouble() );
  if( !registro.value( "titulo" ).isNull() )
  { LETitulo->setText( registro.value( "titulo" ).toString() ); CkBTitulo->setChecked( true ); }
  else
  { CkBTitulo->setChecked( true ); }
  indice = row;
  connect( CBCliente, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioDueno( int ) ) );
}

#include <QSqlField>
/*!
    \fn FormModificarPresupuesto::guardar()
 */
void FormModificarPresupuesto::guardar()
{
 if( CkBTitulo->isChecked() && LETitulo->text().isEmpty() )
 {
 QMessageBox::information( this, "Faltan Datos", "Por favor, ingrese un titulo para el presupuesto o desseleccione la opcion de titulo personalizado" );
  return;
 }
 if( editor->contenido( Qt::PlainText ).isEmpty() )
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
 //Modifico el registro
 modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
 qDebug( qPrintable( modelo->filter() ) );
 if( modelo->filter().isEmpty() )
 { modelo->setFilter( QString( "id_presupuesto = %1" ).arg( indice ) ); modelo->select(); }
 QSqlRecord registro = modelo->record( 0 );
 if( registro.isEmpty() )
 { qWarning( "Sin registros" ); return; }
 registro.remove( registro.indexOf( "id_presupuesto" ) );
 if( registro.contains( "fecha" ) )
 { registro.setValue( "fecha", DTFecha->date() ); registro.field( "fecha" ).setGenerated( true ); }
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
 /*registro.setValue( "creado", QDateTime::currentDateTime() );
 registro.setValue( "modificado", QDateTime::currentDateTime() );*/
 if( modelo->setRecord( 0, registro ) )
 {
  // Registro agregado correctamente
  if( modelo->submitAll() )
  {
	  QMessageBox mensaje;
	  mensaje.setText( QString( "El presupuesto se guardo correctamente con el numero %1.\n\n ¿Que desea hacer a continuacion?" ).arg( indice ) );

	  QPushButton *Bimprimir = mensaje.addButton( tr( "Imprimir" ), QMessageBox::ResetRole );
	  Bimprimir->setIcon( QIcon( ":/imagenes/imprimir.png" ) );

	  /*QPushButton *Bemail = mensaje.addButton( tr( "Enviar por email" ), QMessageBox::ApplyRole );
	  Bemail->setIcon( QIcon( ":/imagenes/email.png" ) );*/

	  mensaje.addButton( tr( "No hacer nada" ), QMessageBox::AcceptRole );

	  mensaje.exec();
	  if( mensaje.clickedButton() == Bimprimir )
	  {
		// Imprimir
		EReporte *reporte = new EReporte( this->parent() );
		reporte->setArchivo( "plugins/presupuestos/informe-presupuestador.xml" );
		reporte->agregarParametro( "num_presupuesto", indice );
		reporte->previsualizar();
		this->close();
	  }
	  /*else if( mensaje.clickedButton() == Bemail )
	  {
		 // Enviar x email
	  }*/
          else
	  {
		_m->select();
		close();
	  }
   }
   else
   {
  	// Error al hacer el submit con el modelo
	qWarning( "Error al hacer el submit de los datos" );
    	qWarning( qPrintable( "error type: " + QString::number( modelo->query().lastError().type() ) + " -  " + modelo->query().lastError().text() + " - Error num: " + QString::number( modelo->query().lastError().number() ) ) );
    	qWarning( qPrintable( modelo->query().lastQuery() ) );
   }
 }
 else
 {
   qWarning( "Error al actualizar los datos del registro" );
   qWarning( qPrintable( "error type: " + QString::number( modelo->query().lastError().type() ) + " -  " + modelo->query().lastError().text() + " - Error num: " + QString::number( modelo->query().lastError().number() ) ) );
   qWarning( qPrintable( modelo->query().lastQuery() ) );
   qWarning( qPrintable( QSqlDatabase::database().lastError().text() ) );
   return;
 }

}


/*!
    \fn FormModificarPresupuesto::setModelo( QSqlTableModel *m )
 */
void FormModificarPresupuesto::setModelo( QSqlTableModel *m )
{  _m = m; }


/*!
    \fn FormModificarPresupuesto::cambioDueno( int id_combo )
 */
void FormModificarPresupuesto::cambioDueno( int id_combo )
{
 // busco el id de db para hacer el filtrado
 int id_cliente = CBCliente->model()->data( CBCliente->model()->index( id_combo, 0 ) ).toInt();
 qobject_cast<EMAutos *>(CBAuto->model())->filtrarPorCliente( id_cliente );
}
