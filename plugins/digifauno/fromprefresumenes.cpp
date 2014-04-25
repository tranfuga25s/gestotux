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
#include <QSqlQueryModel>
#include <QComboBox>
#include "preferencias.h"
#include <QPrinter>
#include "fromprefresumenes.h"

FromPrefResumenes::FromPrefResumenes( QWidget* parent )
: FormPrefHijo( parent ), Ui::FormPrefResumenesBase()
{
	setupUi(this);
	QSqlQueryModel *modelo = new QSqlQueryModel( this );
	modelo->setQuery( "SELECT nombre FROM categoria" );
	CBCategoria->setModel( modelo );

        groupBox_2->setEnabled( false );
// 	CBOrientacion->insertItem( 0, tr( "Vertical" ), QPrinter::Portrait );
// 	CBOrientacion->insertItem( 1, tr( "Horizontal" ), QPrinter::Landscape );

// 	CBHoja->insertItem( -1, tr( "A4"     ), QPrinter::A4 );
// 	CBHoja->insertItem( -1, tr( "Carta"  ), QPrinter::Letter );
// 	CBHoja->insertItem( -1, tr( "Oficio" ), QPrinter::Folio );
}

FromPrefResumenes::~FromPrefResumenes()
{
}


/*!
    \fn FromPrefResumenes::cargar()
 */
void FromPrefResumenes::cargar()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Resumenes" );
 GBCategoria->setChecked( p->value( "detalla", true ).toBool() );
 CBCategoria->setCurrentIndex( p->value( "id_categoria", 2 ).toInt() );
//  CBHoja->setCurrentIndex( p->value( "tam_hoja_combo", 0 ).toInt() );
//  CBOrientacion->setCurrentIndex( p->value( "orientacion_combo", 0 ).toInt() );
 p->endGroup();
 p->endGroup();
}


/*!
    \fn FPrefGeneral::guardar()
 */
void FromPrefResumenes::guardar()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Resumenes" );
 p->setValue( "detalla", GBCategoria->isChecked() );
 p->setValue( "id_categoria", CBCategoria->currentIndex()  );
//  p->setValue( "tam_hoja", CBHoja->itemData( CBHoja->currentIndex() ) );
//  p->setValue( "tam_hoja_combo", CBHoja->currentIndex() );
//  p->setValue( "orientacion", CBOrientacion->itemData( CBOrientacion->currentIndex() ) );
//  p->setValue( "orientacion_combo", CBOrientacion->currentIndex() );
 p->endGroup();
 p->endGroup();
}


/*!
    \fn FromPrefResumenes::aplicar()
 */
void FromPrefResumenes::aplicar()
{
}



