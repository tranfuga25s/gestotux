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
#include "eenviobackup.h"

#include <QFtp>
#include "preferencias.h"
#include "eregistroplugins.h"
#include "einfoprogramainterface.h"
#include <QFile>
#include <QDate>

EEnvioBackup::EEnvioBackup(QObject *parent)
 : QThread(parent)
{
 this->setTerminationEnabled();
}


EEnvioBackup::~EEnvioBackup()
{
}

/*!
	\fn void EEnvioBackup::run()
	Este metodo es el que se ejecuta al realiza el inicio del hilo correspondiente.
	Verifica que el backup no haya sido enviado y si no lo hizo, lo envia.
	Utiliza el ultimo archivo de backup que se creo en la pc actual.
*/
void EEnvioBackup::run()
{
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "backup" );
 if( p->value( "enviado", false ).toBool() == true  )
 {
  qDebug( "El backup ya ha sido enviado... saliendo del hilo" );
  exit(0);
  return;
 }
 setTerminationEnabled();
 archivo = new QFile( p->value( "archivo", QDate::currentDate().toString() ).toString() );
 if( !archivo->open( QIODevice::ReadOnly ) )
 {
  qDebug( QString( "Error al abrir el archivo de backup: %1" ).arg( p->value( "backup/archivo" ).toString() ).toLocal8Bit() );
  delete archivo;
  exit(0);
  return;
 }
 ftp = new QFtp( this );
 connect( ftp, SIGNAL( commandFinished( int, bool ) ), this, SLOT( finComando( int, bool ) ) );
 p->beginGroup( "ftp" );
 ftp->connectToHost( p->value( "host", "tranfuga.no-ip.org" ).toString(), p->value( "puerto", 21 ).toInt() );
 p->endGroup();p->endGroup();p=0;
 ftp->close();
 exec();
}

/*!
    \fn EEnvioBackup::finComando( int id, bool error )
	Slot llamado cada vez que un comando ftp es terminado
	@param id Numero de comando
	@param error Indica si hubo error en la ejecución del comando
 */
void EEnvioBackup::finComando( int id, bool error )
{
 if( error )
 {
  qDebug( QString( "Error: %1").arg( ftp->errorString() ).toLocal8Bit() );
  //delete ftp;
  delete archivo;
  exit(0);
  return;
 }
 switch( id ) {
    case 1:
    {
     ftp->login();
     break;
    }
    case 2:
    {
     ftp->cd( ERegistroPlugins::getInstancia()->pluginInfo()->directorioBackup() );
     break;
    }
    case 3:
    {
      ftp->put( archivo, QDateTime::currentDateTime().toString( "yyyyMMddhhmmsszzz") );
      break;
    }
    case 4:
    {
         qDebug( "Fin del Hilo" );
         preferencias *p = preferencias::getInstancia();
         p->beginGroup( "backup" );
         p->setValue( "enviado", true );
         p->endGroup();
         p->sync();
         p=0;
         ftp->close();
         delete archivo;
         break;
    }
    case 5:
    {
         //delete ftp;
         exit(0);
         break;
     }
  }
}
