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
#include "informesplugin.h"

#include <QPluginLoader>
#include <QDir>
#include <QApplication>
#include <QHash>

bool InformesPlugin::inicializar()
{
    ActActualizar = new QAction( this );
    ActActualizar->setText( "Actualizar informes" );
    connect( ActActualizar, SIGNAL( triggered()), this, SLOT( actualizar() ) );

    // Cargar los plugins
    return cargarPluginsInformes();
}

bool InformesPlugin::verificarTablas(QStringList)
{
 return true;
}

double InformesPlugin::version() const
{
 return 0.2;
}

int InformesPlugin::tipo() const
{ return EPlugin::comun; }

QList< QActionGroup * > InformesPlugin::accionesBarra()
{
 return QList<QActionGroup *>();
}

QString InformesPlugin::nombre() const
{ return "informes"; }

QWidgetList InformesPlugin::formsPreferencias()
{ return QWidgetList(); }

void InformesPlugin::crearMenu( QMenuBar* m )
{
 // Busco el menu de informes
 QMenu *mh = m->findChild<QMenu *>( "menuHerramientas" );
 QMenu *mInformes = mh->addMenu( "Informes" );
 mInformes->setObjectName( "menuInformes" );
 // Ingreso las acciones estandares
 // Ingreso la accion y/o menu de cada plugin
 if( !_plugins->isEmpty() )
 {
     foreach( EInformeInterface *plug, *_plugins ) {
         plug->hacerMenu( mInformes );
     }
 }
 else
 { qDebug( "No existen plugins para colocar en el menu de informes" ); }
 mInformes->addAction( ActActualizar );
}

void InformesPlugin::crearToolBar(QToolBar*)
{}

void InformesPlugin::seCierraGestotux()
{
    emit seCierraGestotuxSignal();
}

void InformesPlugin::actualizar()
{
    /// @TODO: Implementar actualización de informes
    qWarning( "No implementado" );
}

/*!
    \fn InformesPlugin::cargarPluginsInformes()
 */
bool InformesPlugin::cargarPluginsInformes()
{
  loader = new QPluginLoader( this );
 // Busco los plugins de presupuestos
 QDir pluginsDir = QDir(qApp->applicationDirPath());

 #if defined(Q_OS_WIN)
     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
         pluginsDir.cdUp();
 #elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
         pluginsDir.cdUp();
         pluginsDir.cdUp();
     }
 #endif
     pluginsDir.cd("plugins");
     if( !pluginsDir.exists( "informes" ) ) {  return false; }
     pluginsDir.cd("informes");
     if( !pluginsDir.exists() ) { return false; }

     _plugins = new QHash<QString, EInformeInterface *>();
      QStringList filtro;
#ifdef Q_WS_WIN32
      filtro.append( "*.dll" );
#endif
#ifdef Q_WS_X11
	filtro.append( "*so" );
#endif
     if( pluginsDir.entryList( filtro, QDir::Files ).isEmpty() ) { return false; }
     foreach( QString fileName, pluginsDir.entryList( filtro, QDir::Files ) )
     {

	loader->setFileName( pluginsDir.absoluteFilePath( fileName ) );
        if( loader->load() )
        {
		QObject *obj = loader->instance();
		EInformeInterface *plug = qobject_cast<EInformeInterface *>( obj );
		// Genero ahora el visor de informe
		if( plug->inicializar() )
		{
			connect( obj, SIGNAL( agregarVentana( QWidget * ) ), this, SIGNAL( agregarVentana( QWidget * ) ) );
                        connect( this, SIGNAL( seCierraGestotuxSignal() ), obj, SLOT( seCierraGestotux() ) );
			_plugins->insert( plug->nombre(), plug );
                        qDebug( QString( "Cargando Informe Plugin: %1" ).arg( pluginsDir.absoluteFilePath( fileName )).toLocal8Bit() );
		}
		else
		{
                        qWarning( QString( "Error de inicializacion en el plug in Informe %1" ).arg( plug->nombre() ).toLocal8Bit() );
		}
	}
	else
	{
                qWarning( QString( "Error al cargar el plugin Informe . Error: %1" ).arg( loader->errorString() ).toLocal8Bit() );
	}
	// Fin de la carga del plugin
  }
  return true;
}

Q_EXPORT_PLUGIN2( informes, InformesPlugin )
