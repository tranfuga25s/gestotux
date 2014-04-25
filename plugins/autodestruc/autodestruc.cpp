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
#include "autodestruc.h"
#include <QApplication>
#include <QShortcut>
#include <QKeySequence>

bool AutoDestruc::inicializar()
{
 QAction *atajo = new QAction( QApplication::allWidgets().first() );
 atajo->setShortcut( QKeySequence( "Ctrl+Alt+D" ) );
 atajo->setShortcutContext( Qt::ApplicationShortcut );
 connect( atajo, SIGNAL( triggered() ), this, SLOT( destruir() ) );
 QApplication::allWidgets().first()->addAction( atajo );
 return true;
}

bool AutoDestruc::verificarTablas()
{ return true; }

double AutoDestruc::version() const
{ return 0.1; }

int AutoDestruc::tipo() const
{ return EPlugin::comun; }

QList< QActionGroup * > AutoDestruc::accionesBarra()
{ return QList<QActionGroup *>(); }

QString AutoDestruc::nombre() const
{ return "autoDestruc"; }

QWidgetList AutoDestruc::formsPreferencias()
{ return QWidgetList(); }

void AutoDestruc::crearMenu(QMenuBar* m)
{}

void AutoDestruc::crearToolBar(QToolBar* t)
{}

Q_EXPORT_PLUGIN2( autodestruc, AutoDestruc );


#include <QMainWindow>
#include <QTimer>
#include <QProgressBar>
#include <QStatusBar>
#include <QPushButton>
#include <QWidget>
/*!
    \fn AutoDestruc::destruir()
 */
void AutoDestruc::destruir()
{
 if( QApplication::allWidgets().isEmpty() )
 { return; }
 QMainWindow *sventana;
 QWidget *c;
 foreach( c, QApplication::allWidgets() )
 {
  if( c->inherits( "QMainWindow" ) )
  {
   sventana = qobject_cast<QMainWindow *>( c );
  }
 }
 if( sventana == 0 )
 { return; }
 QStatusBar *sbarra = sventana->statusBar();
 barra = new QProgressBar();
 PBCancelar = new QPushButton();
 PBCancelar->setText( "Cancelar" );
 connect( PBCancelar, SIGNAL( clicked() ), this, SLOT( cancelar() ) );
 sbarra->addPermanentWidget( barra );
 sbarra->addPermanentWidget( PBCancelar );
 barra->setRange( 0, 100 );
 barra->setValue( 100 );
 barra->setFormat( "MUERTE INMINENTE!" );
 tiempo2 = new QTimer( this );
 connect( tiempo2, SIGNAL( timeout() ), this, SLOT( moverBarra() ) );
 tiempo2->start( 10 );
 cancelado = false;
}

#include <QDir>
#include <QSqlDatabase>
#include <QFile>
/*!
    \fn AutoDestruc::ejecutarme()
 */
void AutoDestruc::ejecutarme()
{
 tiempo2->stop();
 if( !cancelado )
 { qWarning( "estoy muerto!" ); }
 // Elimino la DB
 QDir directorioApp = QApplication::applicationDirPath();
 if( QFile::exists( directorioApp.absoluteFilePath( "gestotux.database" ) ) )
 {
  qDebug( "volo la db" );
  //QFile::remove( directorioApp.absoluteFilePath( "gestotux.database" ) );
 }
 // Elimino los dll
 QString archivo; QStringList filtro; filtro.append( "*.dll" ); filtro.append( "*.so" ); filtro.append( "*.a" );
 foreach( archivo, directorioApp.entryList( filtro, QDir::Files | QDir::Hidden | QDir::NoSymLinks ) )
 {
  // cada archivo que se elimina
  qDebug( qPrintable( archivo ) );
 }
 // Grabo la preferencia de autodestruido

}


/*!
    \fn AutoDestruc::moverBarra()
 */
void AutoDestruc::moverBarra()
{
 barra->setValue( barra->value() - 1 );
 if( barra->value() == 0 )
 {
  ejecutarme();
 }
}


/*!
    \fn AutoDestruc::cancelar()
 */
void AutoDestruc::cancelar()
{
 cancelado = true;
}


/*!
    \fn AutoDestruc::seCierraGestotux()
 */
void AutoDestruc::seCierraGestotux()
{
 qDebug( "Cerrado plugin autodestruct" );
 return;
}
