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
#include "formprefproductos.h"

#include "preferencias.h"

FormPrefProductos::FormPrefProductos ( QWidget* parent, Qt::WFlags fl )
: FormPrefHijo(), Ui::FormPrefProductosBase()
{
 this->setParent(parent);
 setupUi ( this );
 this->setWindowTitle( "Productos" );
 this->setWindowIcon( QIcon( ":/imagenes/productos.png" ) );
 this->setWindowFlags( fl );
 this->setAttribute( Qt::WA_DeleteOnClose );
}


/*!
    \fn FormPrefProductos::cargar()
 */
void FormPrefProductos::cargar()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Productos");
 CkBCategorias->setChecked( p->value( "categorias", false ).toBool() );
 CkBMarcas->setChecked( p->value( "marcas", false ).toBool() );
 CkBMarcaProveedor->setChecked( p->value( "marca_proveedor", false ).toBool() );
 CkBDescripcion->setChecked( p->value( "descripcion", false ).toBool() );
 GBStock->setChecked( p->value( "stock", false ).toBool() );
 CkBCosto->setChecked( p->value( "mostrar-costo", true ).toBool() );
 CkBOcultarCodigo->setChecked( p->value( "ocultar_codigo", false ).toBool() );
 p->beginGroup( "Stock" );
 CkBPermitir->setChecked( p->value( "permitir", false ).toBool() );
 CkBLimitarVenta->setChecked( p->value( "limitar", false ).toBool() );
 CkBAvisosStock->setChecked( p->value( "avisos", false ).toBool() );
 DSBLimiteMinimo->setValue( p->value( "limiteMinimo", 0.0 ).toDouble() );
 p->endGroup();
 DsBGanancia->setValue( p->value( "ganancia", 10.0 ).toDouble() );
 p->endGroup();
 p->endGroup();
 p = 0;
}


/*!
    \fn FormPrefProductos::aplicar()
 */
void FormPrefProductos::aplicar()
{ return; }


/*!
    \fn FormPrefProductos::guardar()
 */
void FormPrefProductos::guardar()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "Productos" );
 p->setValue( "categorias",CkBCategorias->isChecked() );
 p->setValue( "marcas", CkBMarcas->isChecked() );
 p->setValue( "marca_proveedor", CkBMarcaProveedor->isChecked() );
 p->setValue( "descripcion",CkBDescripcion->isChecked() );
 p->setValue( "stock", GBStock->isChecked() );
 p->setValue( "mostrar-costo", CkBCosto->isChecked() );
 p->setValue( "ocultar_codigo", CkBOcultarCodigo->isChecked() );
 p->beginGroup( "Stock" );
 p->setValue( "permitir", CkBPermitir->isChecked() );
 p->setValue( "limitar", CkBLimitarVenta->isChecked() );
 p->setValue( "avisos", CkBAvisosStock->isChecked() );
 p->setValue( "limiteMinimo", DSBLimiteMinimo->value() );
 p->endGroup();
 p->setValue( "ganancia", DsBGanancia->value() );
 p->endGroup();
 p->endGroup();
 p = 0;
}
