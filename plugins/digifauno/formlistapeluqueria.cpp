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
#include "mpeluqueria.h"
#include "formlistapeluqueria.h"
#include <QPushButton>
#include <QKeySequence>
#include "digifauno.h"
#include "eactcerrar.h"
#include <QMessageBox>

FormListaPeluqueria::FormListaPeluqueria( QWidget* parent )
: EVentana( parent ), Ui::FormListadoPeluqueriaBase()
{
	setupUi(this);
	setObjectName( "lista_servicios" );
	setWindowTitle( "Listado de servicios brindados" );
	setWindowIcon( QIcon( ":/imagenes/pelu.gif" ) );

	this->addAction( new EActCerrar( this ) );
	QAction *ActAgregar = new QAction( "Agregar", this );
	ActAgregar->setIcon( QIcon( ":/imagenes/add.png" ) );
	ActAgregar->setShortcut( QKeySequence( "Ctrl+a" ) );
	DigiFauno *digifauno = qobject_cast<DigiFauno *>(parent);
        connect( ActAgregar, SIGNAL( triggered() ), this, SLOT( agregarServicioPeluqueria() ) );
	this->addAction( ActAgregar );

	QAction *ActBorrar = new QAction( "Borrar", this );
	ActBorrar->setIcon( QIcon( ":/imagenes/eliminar.png" ) );
	ActBorrar->setShortcut( QKeySequence( "Ctrl+b" ) );
	connect( ActBorrar, SIGNAL( triggered() ), this, SLOT( borrar() ) );
	this->addAction( ActBorrar );

	modelo = new MPeluqueria( TVPeluqueria, true );
	modelo->setEditStrategy( QSqlTableModel::OnRowChange );
	TVPeluqueria->setModel( modelo );
	modelo->select();
	TVPeluqueria->setSelectionBehavior( QAbstractItemView::SelectRows );
	TVPeluqueria->resizeColumnsToContents();
	TVPeluqueria->hideColumn( 0 );
	TVPeluqueria->setSortingEnabled( true );

	GBFiltrado->setChecked( false );

	QSqlQueryModel *modeloMascota = new QSqlQueryModel( CBMascota );
	if( QSqlDatabase::database().driverName() == "QSQLITE" )
	{
		modeloMascota->setQuery( "SELECT m.id, m.nombre||' - '||d.apellido||', '||d.nombre FROM mascota m, dueno d WHERE m.id_dueno = d.id" );
	}
	else if( QSqlDatabase::database().driverName() == "QMYSQL" )
	{
		modeloMascota->setQuery( "SELECT m.id, CONCAT( CONCAT( CONCAT( CONCAT( m.nombre, \" - \" ), d.apellido ), \", \" ), d.nombre ) FROM mascota m, dueno d WHERE m.id_dueno = d.id" );
	}
	CBMascota->setModel( modeloMascota );
	CBMascota->setModelColumn( 1 );
	CBMascota->setCurrentIndex( -1 );

	connect( GBFiltrado, SIGNAL( toggled ( bool )              ), this, SLOT( cambioEstadoGrupo( bool )    ) );
	connect( ChBMascota, SIGNAL( stateChanged ( int )          ), this, SLOT( cambioEstadoMascota( int )   ) );
	connect( ChBFecha  , SIGNAL( stateChanged ( int )          ), this, SLOT( cambioEstadoFecha( int )     ) );
	connect( CBMascota , SIGNAL( currentIndexChanged( int )    ), this, SLOT( cambioIndice( int )          ) );
	connect( DEFecha   , SIGNAL( dateChanged ( const QDate & ) ), this, SLOT( cambioFecha( const QDate & ) ) );

	///@todo Agregar calculo de fecha maxima y minima desde la tabla para limitar el filtrado
}

FormListaPeluqueria::~FormListaPeluqueria()
{
}


/*!
    \fn FormListaPeluqueria::cambioEstadoFecha(  int estado  )
 */
void FormListaPeluqueria::cambioEstadoFecha( int estado )
{
 switch( estado )
 {
  case Qt::Unchecked:
  {
   DEFecha->setEnabled( false );
   crear_filtro();
   break;
  }
  case Qt::Checked:
  {
   DEFecha->setEnabled( true );
   crear_filtro();
   break;
  }
 }
}


/*!
    \fn FormListaPeluqueria::cambioEstadoMascota(  int estado  )
 */
void FormListaPeluqueria::cambioEstadoMascota( int estado  )
{
 switch( estado )
 {
  case Qt::Unchecked:
  {
   CBMascota->setEnabled( false );
   crear_filtro();
   break;
  }
  case Qt::Checked:
  {
   CBMascota->setEnabled( true );
   crear_filtro();
   break;
  }
 }
}


/*!
    \fn FormListaPeluqueria::cambioIndice( int indice )
 */
void FormListaPeluqueria::cambioIndice( int indice  )
{
//  qDebug( "Filtrando por mascota" );
 crear_filtro();
}


/*!
    \fn FormListaPeluqueria::cambioFecha(  const QDate  &fecha )
 */
void FormListaPeluqueria::cambioFecha(  const QDate &fecha )
{
//  qDebug( "Filtrando por fecha" );
 crear_filtro();
}


/*!
    \fn FormListaPeluqueria::borrar()
 */
void FormListaPeluqueria::borrar()
{
  //Preguntar al usuario si esta seguro
 QItemSelectionModel *selectionModel = TVPeluqueria->selectionModel();
 QModelIndexList indices = selectionModel->selectedRows();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para eliminar",
                   QMessageBox::Ok );
   return;
 }
 //Hacer dialogo de confirmacion..
 int ret;
 ret = QMessageBox::warning( this, "Esta seguro?",
                   QString( "Esta seguro de eliminar %1 item?").arg( indices.size() ),
                   "Si", "No" );
 if ( ret == 0 )
 {
	QModelIndex indice;
	foreach( indice, indices )
	{
		if( indice.isValid() )
		{
			modelo->removeRow( indice.row() );
		}
	}
 }
 return;
}


/*!
    \fn FormListaPeluqueria::cambioEstadoGrupo( bool estado )
 */
void FormListaPeluqueria::cambioEstadoGrupo( bool estado )
{
//  qDebug( "Cambiado estado grupo" );
 if( estado )
 {
  CBMascota->setEnabled( ChBMascota->isChecked() );
  DEFecha->setEnabled( ChBFecha->isChecked() );
 }
 else
 {
  modelo->setFilter( "" );
  modelo->select();
 }
}


/*!
    \fn FormListaPeluqueria::crear_filtro()
 */
void FormListaPeluqueria::crear_filtro()
{
 bool bandera = false;
 QString filtro;
 if( ChBMascota->isChecked() )
 {
  int id_mascota = CBMascota->model()->data( CBMascota->model()->index( CBMascota->currentIndex(), 0 ) , Qt::EditRole ).toInt();
  filtro.append( "id_mascota = '" ).append( QString::number( id_mascota ) ).append( "'" );
  bandera = true;
 }
 if( ChBFecha->isChecked() )
 {
  if( bandera )
  {
    filtro.append( " AND " );
  }
  filtro.append( " fecha = '" ).append( DEFecha->date().toString( "yyyy-MM-dd" ) ).append( "'" );
 }
 modelo->setFilter( filtro );
 modelo->select();
//  qDebug( QString( "Cambiado filtro a: %1" ).arg( modelo->filter() ).toLocal8Bit() );
}

#include "formagregarservicio.h"
void FormListaPeluqueria::agregarServicioPeluqueria()
{
  emit agregarVentana( new FormAgregarServicio( ) );
}
