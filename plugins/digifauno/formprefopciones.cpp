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
#include "preferencias.h"

#include "formprefopciones.h"

FormPrefOpciones::FormPrefOpciones( QWidget* parent )
: EVentana( parent ), Ui::FormPrefOpcionesBase()
{
 setupUi(this);
 this->setAttribute( Qt::WA_DeleteOnClose );
 this->setWindowTitle( "Peluqueria" );
 this->setWindowIcon( QIcon( ":/imagenes/icono.png" ) );
}

FormPrefOpciones::~FormPrefOpciones()
{
}




/*!
    \fn FormPrefOpciones::cargar()
 */
void FormPrefOpciones::cargar()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 SBLimite->setValue( p->value( "limite", 5 ).toInt() );
 p->endGroup();
 p->endGroup();
}


/*!
    \fn FormPrefOpciones::guardar()
 */
void FormPrefOpciones::guardar()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 p->setValue( "limite", SBLimite->value() );
 p->endGroup();
 p->endGroup();
}


/*!
    \fn FormPrefOpciones::aplicar()
 */
void FormPrefOpciones::aplicar()
{
}

