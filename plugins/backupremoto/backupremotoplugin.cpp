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
#include "backupremotoplugin.h"

#include <QString>
#include <QIcon>
#include <QAction>
#include <QMenuBar>
#include <QList>
#include <QStackedWidget>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

/*!
    \fn BackupRemotoPlugin::accionesBarra()
        Retorna las acciones que tiene la barra lateral para este plugin
        @return Lista de acciones
 */
QList<QActionGroup *> BackupRemotoPlugin::accionesBarra()
{ return QList<QActionGroup*>(); }


bool BackupRemotoPlugin::inicializar()
{
 Q_INIT_RESOURCE(backupremoto);

 ActBackup = new QAction( this );
 ActBackup->setText( "Backup Remoto" );
 ActBackup->setStatusTip( "Genera una copia de seguridad y la envía al servidor remoto" );
 ActBackup->setIcon( QIcon( ":/imagenes/backup.png" ) );
 connect( ActBackup, SIGNAL( triggered() ), this, SLOT( hacerBackup() ) );

 return true;
}

#include "formprefbackupremoto.h"
/*!
    \fn BackupRemotoPlugin::formsPreferencias()
 */
QWidgetList BackupRemotoPlugin::formsPreferencias()
{
 QWidgetList lista;
 lista.append( new FormPrefBackupRemoto() );
 return lista;
}

/*!
    \fn BackupRemotoPlugin::nombre() const
 */
QString BackupRemotoPlugin::nombre() const
{ return "backupremoto"; }


/*!
    \fn BackupRemotoPlugin::tipo() const
 */
int BackupRemotoPlugin::tipo() const
{ return EPlugin::comun; }


/*!
    \fn BackupRemotoPlugin::crearMenu( QMenuBar *m )
 */
void BackupRemotoPlugin::crearMenu( QMenuBar *m )
{
 QMenu *menuHer = m->findChild<QMenu *>( "menuArchivo" );
 menuHer->addSeparator();
 menuHer->addAction( ActBackup );
}

/*!
 * \brief BackupRemotoPlugin::verificarTablas
 * \return
 */
bool BackupRemotoPlugin::verificarTablas( QStringList )
{ return true; }

/*!
 * \fn BackupRemotoPlugin::version() const
 */
double BackupRemotoPlugin::version() const
{  return 0.1; }

/*!
    \fn BackupRemotoPlugin::crearToolBar( QToolBar *t )
 */
void BackupRemotoPlugin::crearToolBar( QToolBar */*t*/ )
{}


/*!
    \fn BackupRemotoPlugin::seCierraGestotux()
 */
void BackupRemotoPlugin::seCierraGestotux()
{
 Q_CLEANUP_RESOURCE(backupremoto);
}

QAction *BackupRemotoPlugin::botonPantallaInicial()
{
    return ( new QAction( this ) );
}

#include "ebackupremoto.h"
void BackupRemotoPlugin::hacerBackup()
{ emit agregarVentana( new EBackupRemoto() ); }

Q_EXPORT_PLUGIN2( backupremoto, BackupRemotoPlugin )
