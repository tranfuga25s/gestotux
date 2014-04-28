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
#include "formprefventa.h"

#include "preferencias.h"
#include <QCheckBox>
#include <QDoubleSpinBox>

FormPrefVenta::FormPrefVenta( QWidget* parent ) :
 FormPrefHijo(), Ui::FormPrefVentaBase()
{
    this->setParent( parent );
    setupUi(this);
    this->setWindowTitle( "Ventas" );
    this->setWindowIcon( QIcon( ":/imagenes/factura.png" ) );
    this->setAttribute( Qt::WA_DeleteOnClose );
}


/*!
    \fn FormPrefVenta::cargar()
 */
void FormPrefVenta::cargar()
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Ventas" );
 this->CkBBusquedaPrecio->setChecked( p->value( "buscarPrecio", false ).toBool() );
 this->CkBFiltrarProveedor->setChecked( p->value( "filtrarProveedor", false ).toBool() );
 p->endGroup();
 p->endGroup();
 p=0;
}


/*!
    \fn FormPrefVenta::aplicar()
 */
void FormPrefVenta::aplicar()
{ return; }


/*!
    \fn FormPrefVenta::guardar()
 */
void FormPrefVenta::guardar()
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Ventas" );
 p->setValue( "buscarPrecio", this->CkBBusquedaPrecio->isChecked() );
 p->setValue( "filtrarProveedor", this->CkBFiltrarProveedor->isChecked() );
 p->endGroup();
 p->endGroup();
 p=0;
}
