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
#include "formagregarservicio.h"
#include "mmascota.h"
#include "mpeluqueria.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QCompleter>
#include "eactcerrar.h"
#include "preferencias.h"

FormAgregarServicio::FormAgregarServicio( QWidget* parent )
: EVentana( parent ), Ui::FormAgregarServPeluqueriaBase()
{
	setupUi(this);
	setObjectName( "FormAgregarServicio" );
	setWindowTitle( "Agregar Nuevo Servicio de Peluqueria" );
	setWindowIcon( QIcon( ":/imagenes/pelu.gif" ) );

	this->addAction( new EActCerrar( this ) );

	QSqlQueryModel *modeloMascota = new QSqlQueryModel( CBMascota );
	if( QSqlDatabase::database().driverName() == "QSQLITE" )
	{
		modeloMascota->setQuery( "SELECT m.id, m.nombre||' - '||d.apellido||', '||d.nombre FROM mascota m, dueno d WHERE m.id_dueno = d.id ORDER BY 2" );
	}
	else if( QSqlDatabase::database().driverName() == "QMYSQL" )
	{
		modeloMascota->setQuery( "SELECT m.id, CONCAT( CONCAT( CONCAT( CONCAT( m.nombre, \" - \" ), d.apellido ), \", \" ), d.nombre ) FROM mascota m, dueno d WHERE m.id_dueno = d.id ORDER BY 2" );
	}
	CBMascota->setModel( modeloMascota );
	CBMascota->setModelColumn( 1 );
	CBMascota->setCurrentIndex( -1 );
	connect( CBMascota, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioIndice( int ) ) );

	CWFecha->setMaximumDate( QDate::currentDate() );
	CWFecha->setSelectionMode( QCalendarWidget::SingleSelection );

	PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
	PBAgregar->setShortcut( QKeySequence( "Ctrl+a" ) );
	PBAgregar->setText( "&Agregar" );
	PBBorrar->setIcon( QIcon( ":/imagenes/borrar.png" ) );
	PBBorrar->setShortcut( QKeySequence( "Ctrl+L" ) );
	PBBorrar->setText( "&Limpiar" );

	modeloPeluqueria = new MPeluqueria( this, false );
	modeloPeluqueria->setSolo_lectura( true );
	preferencias *p = preferencias::getInstancia();
	modeloPeluqueria->setLimite( p->value( "General\\limite", 5 ).toInt() );
	modeloPeluqueria->setSort( 4, Qt::DescendingOrder );
 	modeloPeluqueria->setEditStrategy( QSqlTableModel::OnRowChange );
	TVHistorial->setModel( modeloPeluqueria );
	TVHistorial->hideColumn( 0 );
	TVHistorial->hideColumn( 1 );
	TVHistorial->resizeColumnsToContents();

	connect( PBAgregar, SIGNAL( clicked() ), this, SLOT( agregar() ) );
	connect( PBBorrar , SIGNAL( clicked() ), this, SLOT( borrar()  ) );

	QCompleter *completador = new QCompleter( LEDescripcion );
	QSqlQueryModel *modeloDescripciones = new QSqlQueryModel( completador );
	modeloDescripciones->setQuery( "SELECT DISTINCT descripcion FROM peluqueria" );
	completador->setModel( modeloDescripciones );
	LEDescripcion->setCompleter( completador );
}

FormAgregarServicio::~FormAgregarServicio()
{
}


/*!
    \fn FormAgregarServicio::cambioIndice( int indice )
	Funcion llamada cada vez que se cambia la mascota para actualizar el historial
	@param indice Id de mascota
 */
void FormAgregarServicio::cambioIndice( int indice )
{
 if( !CBMascota->model()->index( indice, 0 ).isValid() )
 {
  qDebug( "Error al filtrar" );
  return;
 }
 id_mascota =  CBMascota->model()->data( CBMascota->model()->index( indice, 0 ), Qt::EditRole );
 modeloPeluqueria->setFilter( " id_mascota = " + id_mascota.toString() );
 modeloPeluqueria->select();
 //qDebug( modeloPeluqueria->filter().toLocal8Bit() );
 TVHistorial->resizeColumnsToContents();
}


/*!
    \fn FormAgregarServicio::agregar()
	Agrega los datos si se lleno correctamente
 */
void FormAgregarServicio::agregar()
{
 if( CBMascota->currentIndex() < 0 )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor, seleccione una mascota de la lista desplegable" );
  qDebug( "Error de masctoa" );
  return;
 }
 if( LEDescripcion->text().isEmpty() )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor, ingrese una descripcion para el servicio" );
  qDebug( "Error, descripcion esta vacio" );
  return;
 }
 if( !dSpBPrecio->value() > 0 )
 {
  QMessageBox::warning( this, "Faltan Datos", "Por favor, ingrese un precio para el servicio prestado" );
  qDebug( QString( "Error, precio es invalido: %1" ).arg( dSpBPrecio->value() ).toLocal8Bit() );
  return;
 }
 modeloPeluqueria->setLimite( -1 );
 if( modeloPeluqueria->agregarServicio( LEDescripcion->text(),
					dSpBPrecio->value(),
					CWFecha->selectedDate(),
					id_mascota ) )
 {
  QMessageBox::information( this, "Correcto", "Los datos se han guardado correctamente" );
  borrar();
  preferencias *p = preferencias::getInstancia();
  modeloPeluqueria->setLimite( p->value( "General\\limite", 5 ).toInt() );
  modeloPeluqueria->select();
  TVHistorial->resizeColumnsToContents();
  return;
 }
 else
 {
  QMessageBox::critical( this, "Error", "Hubo un error al intentar agregar el servicio. No se guardaron datos." );
  return;
 }
}


/*!
    \fn FormAgregarServicio::borrar()
	Borra los campos de servicio
 */
void FormAgregarServicio::borrar()
{
 CWFecha->setSelectedDate( QDate::currentDate() );
 LEDescripcion->clear();
 dSpBPrecio->setValue( 0.0 );
}


/*!
    \fn FormAgregarServicio::cerrar()
	Cierra la ventana verificando que no existan datos sin guardar
 */
void FormAgregarServicio::cerrar()
{
 if( modeloPeluqueria->isDirty( QModelIndex() ) )
 {
  int ret;
  ret = QMessageBox::warning( this, "Esta seguro?",
                   "Aun hay cambios pendientes para guardar. Desea guardar los cambios?",
                   "Si", "No" );
  if ( ret == 0 )
  {
   modeloPeluqueria->submitAll();
   close();
  }
  else
  {
   modeloPeluqueria->revertAll();
   close();
  }
 }
 else
 {
  close();
 }
}
