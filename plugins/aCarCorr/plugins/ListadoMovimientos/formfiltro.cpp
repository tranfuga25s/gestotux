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
#include "formfiltro.h"

#include <QSqlTableModel>
#include "../../emovimiento.h"
#include "../../mcategoria.h"
#include "../../mestablecimiento.h"

FormFiltro::FormFiltro(QWidget* parent, Qt::WFlags fl)
: QDialog( parent ), Ui::FormFiltrosBase()
{
	setupUi(this);
	this->setWindowTitle( "Elija los datos" );
	this->setObjectName( "FormFiltroInforme" );

	PBCancelar->setIcon( QIcon( ":/imagenes/stop.png" ) );
	PBAceptar->setIcon( QIcon( ":/imagenes/ver.png" ) );
	PBAceptar->setDefault( true );

	connect( PBAceptar, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( PBCancelar, SIGNAL( clicked() ), this, SLOT( reject() ) );

	CBEstablecimiento->setModel( new MEstablecimiento( CBEstablecimiento ) );
	CBEstablecimiento->setModelColumn( 1 );
	qobject_cast<QSqlTableModel *>(CBEstablecimiento->model())->select();

	CBCategoria->setModel( new MCategoria( CBCategoria ) );
	CBCategoria->setModelColumn( 1 );
	qobject_cast<QSqlTableModel *>(CBCategoria->model())->select();

	CBTipo->addItem( "Ingreso por Stock", stock );
	CBTipo->addItem( "Ingreso por Compra", compra );
	CBTipo->addItem( "Movimiento entre Establecimientos", mudanza );
	CBTipo->addItem( "Salida por ventas", venta );

	DEFechaEspecifica->setDate( QDate::currentDate() );
	DEFechaInicio->setDate( QDate::currentDate() );
	DEFechaFin->setDate( QDate::currentDate() );

	adjustSize();
}

FormFiltro::~FormFiltro()
{
}




/*!
    \fn FormFiltro::tieneCategoria() const
 */
bool FormFiltro::tieneCategoria() const
{
  return CkBCategoria->isChecked();
}


/*!
    \fn FormFiltro::tieneEstablecimiento() const
 */
bool FormFiltro::tieneEstablecimiento() const
{
 return CkBEstablecimiento->isChecked();
}


/*!
    \fn FormFiltro::tieneFechaEspecifica() const
 */
bool FormFiltro::tieneFechaEspecifica() const
{
 return RBFechaEspecifica->isChecked();
}


/*!
    \fn FormFiltro::tieneTipo() const
 */
bool FormFiltro::tieneTipo() const
{
 return CkBTipo->isChecked();
}


/*!
    \fn FormFiltro::tieneRangoFecha() const
 */
bool FormFiltro::tieneRangoFecha() const
{
 return RBRangoFechas->isChecked();
}


/*!
    \fn FormFiltro::FechaEspecifica() const
 */
QDate FormFiltro::FechaEspecifica() const
{
 return DEFechaEspecifica->date();
}


/*!
    \fn FormFiltro::categoria() const
 */
int FormFiltro::categoria() const
{
  return CBCategoria->model()->data( CBCategoria->model()->index( CBCategoria->currentIndex(), 0 ), Qt::EditRole ).toInt();
}


/*!
    \fn FormFiltro::establecimiento() const
 */
int FormFiltro::establecimiento() const
{
 return CBEstablecimiento->model()->data( CBEstablecimiento->model()->index( CBEstablecimiento->currentIndex(), 0 ), Qt::EditRole ).toInt();
}


/*!
    \fn FormFiltro::rangoFechas() const
 */
QPair<QDate, QDate> FormFiltro::rangoFechas() const
{
  QPair<QDate, QDate> fechas;
  fechas.first = DEFechaInicio->date();
  fechas.second = DEFechaFin->date();
  return fechas;
}


/*!
    \fn FormFiltro::tipo() const
 */
int FormFiltro::tipo() const
{
 return CBTipo->itemData( CBTipo->currentIndex() ).toInt();
}
