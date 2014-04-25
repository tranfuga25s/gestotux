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

#include <QStyleFactory>
#include <QComboBox>
#include <QApplication>
#include <QTimer>
#include <QDir>
#include "preferencias.h"
#include "fprefgeneral.h"

FPrefGeneral::FPrefGeneral( QWidget* parent )
: EVentana( parent ), Ui::FPrefGeneralBase()
{
	setupUi(this);
 	this->setAttribute( Qt::WA_DeleteOnClose );
	CBEstilo->insertItems( -1, QStyleFactory::keys() );
	CBSobreEstilo->setEnabled( CkBSobreEstilo->isChecked() );
	// Busco los estilos disponibles
	QDir dir( QCoreApplication::applicationDirPath() );
	dir.cd( "sobreestilos" );
	if( dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot ).isEmpty() )
	{
		CkBSobreEstilo->setEnabled(false);
		CBSobreEstilo->setEnabled(false);
	}
	else
	{
		// inserto los directorios de estilo
		CBSobreEstilo->insertItems( -1, dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) );
	}
        /// @TODO: Habilitar cuando se realize el sistema de actualizaciones
        //this->GBActualizaciones->setVisible( false );
}

/*!
    \fn FPrefGeneral::cargar()
 */
void FPrefGeneral::cargar()
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 ChBMaximizado->setChecked( p->value( "maximizado" , false ).toBool() );
 CBEstilo->setCurrentIndex( p->value( "estilo-int", 0 ).toInt() );
 ChBSplash->setChecked( p->value( "splash", true ).toBool() );
 ChBIconoBandeja->setChecked( p->value( "icono_bandeja", false ).toBool() );
 CkBSobreEstilo->setChecked( p->value( "sobreestilo", false ).toBool() );
 CBSobreEstilo->setCurrentIndex( p->value( "sobreestiloint", -1 ).toInt() );
 CkBReloj->setChecked( p->value( "reloj", true ).toBool() );
 p->endGroup();
 p->beginGroup("Actualizaciones");
 LEServidor->setText( p->value("servidor", "tranfuga.no-ip.org" ).toString() );
 SBPuerto->setValue( p->value( "puerto", 21 ).toInt() );
 LERuta->setText( p->value( "ruta", "" ).toString() );
 LEUsuario->setText( p->value( "usuario", "" ).toString() );
 LEContra->setText( p->value( "contra","" ).toString() );
 CkBAnonimo->setChecked( p->value( "anonimo", true ).toBool() );
 p->endGroup();
 p->endGroup();
 // conecto el slot que realiza el cambio
 connect( CBSobreEstilo, SIGNAL( currentIndexChanged( const QString & ) ), this, SLOT( cambioSobreEstilo( const QString & ) ) );
 // conecto el slot para deshacer los cambios si se dechequea la casilla
 connect( CkBSobreEstilo, SIGNAL( toggled( bool ) ), this, SLOT( cambioSobreEstilo( bool ) ) );
 // conecto los slots de anonimo
 connect( CkBAnonimo, SIGNAL( toggled( bool ) ), LEUsuario, SLOT( setEnabled( bool ) ) );
 connect( CkBAnonimo, SIGNAL( toggled( bool ) ), LEContra , SLOT( setEnabled( bool ) ) );
 connect( CkBAnonimo, SIGNAL( toggled( bool ) ), LUsuario , SLOT( setEnabled( bool ) ) );
 connect( CkBAnonimo, SIGNAL( toggled( bool ) ), LContra  , SLOT( setEnabled( bool ) ) );

}


/*!
    \fn FPrefGeneral::guardar()
 */
void FPrefGeneral::guardar()
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Preferencias" );
 p->beginGroup( "General" );
 p->setValue( "maximizado", ChBMaximizado->isChecked() );
 p->setValue( "estilo", CBEstilo->currentText() );
 p->setValue( "estilo-int", CBEstilo->currentIndex() );
 p->setValue( "splash", ChBSplash->isChecked() );
 p->setValue( "icono_bandeja", ChBIconoBandeja->isChecked() );
 p->setValue( "sobreestilo", CkBSobreEstilo->isChecked() );
 p->setValue( "sobreestilonombre", CBSobreEstilo->currentText() );
 p->setValue( "sobreestiloint", CBSobreEstilo->currentIndex() );
 p->setValue( "reloj", CkBReloj->isChecked() );
 p->endGroup();
 p->beginGroup( "Actualizaciones" );
 p->setValue( "servidor", LEServidor->text() );
 p->setValue( "puerto", SBPuerto->value() );
 p->setValue( "ruta", LERuta->text() );
 p->setValue( "usuario", LEUsuario->text() );
 p->setValue( "contra", LEContra->text() );
 p->setValue( "anonimo", CkBAnonimo->isChecked() );
 p->endGroup();
 p->endGroup();
 p->sync();
 p = 0;
}


/*!
    \fn FPrefGeneral::aplicar()
 */
void FPrefGeneral::aplicar()
{
  QApplication::setStyle( QStyleFactory::create( CBEstilo->currentText() ) );
}


/*!
    \fn FPrefGeneral::cambioSobreEstilo( bool estado )
 */
void FPrefGeneral::cambioSobreEstilo( bool estado )
{
 // el combobox se deshabilita solo asique limpio el estilo que haya
 if( estado )
 {
  cambioSobreEstilo( CBSobreEstilo->currentText() );
 }
 else
 {
   qApp->setStyleSheet( QString() );
 }
}


/*!
    \fn FPrefGeneral::cambioSobreEstilo( const QString &estilo )
 */
void FPrefGeneral::cambioSobreEstilo( const QString &estilo )
{
	QDir dir( QCoreApplication::applicationDirPath() );
	dir.cd( "sobreestilos" );
	dir.cd( estilo );
	QFile file( dir.absoluteFilePath( QString( estilo ).append( ".qss" ) ) );
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);
}
