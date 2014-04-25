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
#include "formagregarmasctoa.h"
#include "mdueno.h"
#include "mmascota.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QSqlRecord>
#include <QCompleter>
#include "mrazas.h"
#include "eactcerrar.h"
#include "eactguardar.h"

FormAgregarMascota::FormAgregarMascota( QWidget* parent )
: EVentana( parent ), Ui::FormAgregarMascotaBase()
{
	setupUi(this);
	setObjectName("agregarMascota");
	setWindowTitle( "Agregar Nueva Mascota" );
	modeloDueno = new MDueno( TVDueno );
	TVDueno->setModel( modeloDueno );
	modeloDueno->select();
	TVDueno->setSelectionBehavior( QAbstractItemView::SelectRows );
	TVDueno->resizeColumnsToContents();
	TVDueno->hideColumn( 0 );
	TVDueno->hideColumn( 3 );

	CBGenero->insertItem( 0, "Macho");
	CBGenero->insertItem( 1, "Hembra" );
	CBGenero->insertItem( 2, " " );
	CBGenero->setCurrentIndex( 2 );
	// Cosas ademas
	this->addAction( new EActCerrar( this ) );
	this->addAction( new EActGuardar( this ) );
	PBAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
	PBAgregar->setShortcut( QKeySequence( "Crtl+a" ) );
	// Conecto las seï¿½ales
	connect( PBAgregar,  SIGNAL( clicked() ), this, SLOT( agregarDueno() ) );
	// Pongo a editar el nombre
	LENombre->setCursorPosition( 0 );
	// Autocompletado
	completarazas = new QCompleter( this );
	modeloRazas = new MRazas( completarazas );
	completarazas->setModel( modeloRazas );
	completarazas->setCompletionMode( QCompleter::PopupCompletion );
	completarazas->setCaseSensitivity( Qt::CaseInsensitive );
	LERaza->setCompleter( completarazas );
}

FormAgregarMascota::~FormAgregarMascota()
{
}

/*!
    \fn FormAgregarMascota::guardar()
	Guarda todos los datos y controla que lo ingresado sea correcto
 */
void FormAgregarMascota::guardar()
{
 if( CBGenero->currentIndex() == 2 )
 {
  QMessageBox::warning( this, "Seleccione un genero", "Por favor, seleccione un genero para esta mascota" );
  return;
 }
 if( LENombre->text().isEmpty() )
 {
  QMessageBox::warning( this, "Ingrese Nombre","Por favor, ingrese un nombre para la mascota" );
  return;
 }
 if( TVDueno->selectionModel()->selectedRows() == QModelIndexList() )
 {
  QMessageBox::warning( this, "Seleccione un dueño", "Por favor, Seleccione uno de los dueños existentes o utilice el boton agregar dueño para agregar uno nuevo." );
  return;
 }
 MMascota *modelo = new MMascota( this );
 modelo->setEditStrategy( QSqlTableModel::OnManualSubmit );
 if( modelo->agregarMascota(  LENombre->text(),
				LERaza->text(),
				CBGenero->currentIndex(),
				TVDueno->selectionModel()->selectedRows().at(0) ) )
 {
  if( !modelo->submitAll() )
  {
   QMessageBox::critical( this, "Error", "Error: Hubo un error al guardar los datos." );
   this->close();
   return;
  }
  else
  {
    QMessageBox::information( this, "Guardado correctamente", "Los datos de esta nueva mascota se guardaron correctamente" );
    this->close();
    return;
  }
 }
 else
 {
  QMessageBox::critical( this, "Error", "Error: Hubo un error al guardar los datos" );
  this->close();
  return;
 }
}

/*!
    \fn FormAgregarMascota::cancelar()
	Cancela todos los cambios y cierra la ventana
 */
void FormAgregarMascota::cancelar()
{
 this->close();
}


/*!
    \fn FormAgregarMascota::agregarDueno()
	Agrega un nuevo dueño a la vista
 */
void FormAgregarMascota::agregarDueno()
{
 if( !modeloDueno->insertRecord( -1, modeloDueno->record() ) )
 {
//   qDebug( "Error al insertar registro en los dueños" );
  return;
 }
 else
 {
  TVDueno->edit( modeloDueno->index( modeloDueno->get_ultima_row(), 1 ) );
 }
}
