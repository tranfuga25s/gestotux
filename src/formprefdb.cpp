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
#include "formprefdb.h"
#include "preferencias.h"

FormPrefDb::FormPrefDb(QWidget* parent, Qt::WFlags fl)
: EVentana( parent, fl ), Ui::FormPrefDbBase()
{
 this->setAttribute( Qt::WA_DeleteOnClose );
 setupUi(this);
}

/*!
    \fn FormPrefDb::cargar()
 */
void FormPrefDb::cargar()
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 RBInterna->setChecked( p->value( "dbInterna", true ).toBool() );
 RBExterna->setChecked( p->value( "dbExterna", false ).toBool() );
 p->beginGroup( "mysql" );
 LEHost->setText( p->value( "host" ).toString() );
 if( p->contains( "puerto" ) )
 {
  SBPuerto->setValue( p->value( "puerto", 3306 ).toInt() );
  CkBPuerto->setChecked( true );
 }
 LEUsuario->setText( p->value( "usuario" ).toString() );
 if( p->contains( "contra" ) )
 {
  CkBPass->setChecked( true );
  LEPass->setText( p->value( "contra", "desconocido" ).toString() );
 }
 LEBaseDatos->setText( p->value( "base", "gestotux" ).toString() );
 CkBAutomatico->setChecked( p->value( "automatico", false ).toBool() );
 p->endGroup();
 CBFrecuencia->setCurrentIndex( p->value( "frecuenciaBackup", 1 ).toInt() );
 p->endGroup();
 p->endGroup();
 p = 0;
}


/*!
    \fn FormPrefDb::guardar()
 */
void FormPrefDb::guardar()
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 p->setValue( "dbInterna", RBInterna->isChecked() );
 p->setValue( "dbExterna", RBExterna->isChecked() );
 p->beginGroup( "mysql" );
 p->setValue( "base", LEBaseDatos->text() );
 p->setValue( "usuario", LEUsuario->text() );
 p->setValue( "host", LEHost->text() );
 if( CkBPuerto->isChecked() )
 {
  p->setValue( "puerto", SBPuerto->value() );
 } else {
     if( p->contains( "puerto" ) ) { p->remove( "puerto" ); }
 }
 if( CkBPass->isChecked() )
 {
  p->setValue( "contra", LEPass->text() );
 }
 p->setValue( "automatico", CkBAutomatico->isChecked() );
 p->endGroup();
 if( !RBExterna->isChecked() )
 {
  p->setValue( "frecuenciaBackup", CBFrecuencia->currentIndex() );
 }
 else
 {
  // Si tiene una base de datos externa, el backup se hace en la base de datos
  p->setValue( "frecuenciaBakup", 7 );
 }
 p->endGroup();
 p->endGroup();
}


/*!
    \fn FPrefGeneral::aplicar()
 */
void FormPrefDb::aplicar()
{}
