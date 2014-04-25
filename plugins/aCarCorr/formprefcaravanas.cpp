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
#include "formprefcaravanas.h"

#include "mprefcategorias.h"
#include "edcategoriaespecial.h"

FormPrefCaravanas::FormPrefCaravanas(QWidget *parent)
 : EVentana(parent), Ui::FormPrefCaravanas()
{
 setupUi(this);
 this->setAttribute( Qt::WA_DeleteOnClose );
 this->setWindowTitle( "Caravanas" );
 this->setWindowIcon( QIcon( ":/imagenes/caravana.png" ) );
}


FormPrefCaravanas::~FormPrefCaravanas()
{
}

void FormPrefCaravanas::cargar()
{
  // Inicializo el modelo
 modelo = new MPrefCategorias( this );
 TVCategorias->setModel( modelo );
 TVCategorias->hideColumn( 0 );
 TVCategorias->setItemDelegate( new EDCategoriaEspecial( TVCategorias ) );
 modelo->select();
}


void FormPrefCaravanas::guardar()
{
 modelo->submitAll();
}

void FormPrefCaravanas::aplicar()
{
 modelo->submitAll();
}
