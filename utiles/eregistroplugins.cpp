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
#include "eregistroplugins.h"
#include "einfoprogramainterface.h"
#include <QApplication>
#include <QStringList>
#include <QDebug>

ERegistroPlugins *ERegistroPlugins::instance = 0;
EInterfazEmail *ERegistroPlugins::_plugEmail = 0;

ERegistroPlugins::ERegistroPlugins( QObject */*parent*/ )
{
    _plugins = new QHash<QString, EPlugin *>();
    _plugins2 = new QHash<QString, QObject *>();
    _pluginInfo = 0;
    // Elimino cualquier PID anterior
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "carga" );
    foreach( QString grupo, p->childGroups() )
    {
        if( grupo != QString::number( QCoreApplication::applicationPid() ) )
            p->remove( grupo );
    }
    p->endGroup();
    p=0;
}


ERegistroPlugins::~ERegistroPlugins()
{
    delete _plugins;
}

/*!
 * \fn ERegistroPlugins::getInstancia()
 * Devuelve una instancia unica de este objeto
 */
ERegistroPlugins* ERegistroPlugins::getInstancia()
{
 if (instance==0)
 {
  instance = new ERegistroPlugins();
 }
 return instance;
}

/*!
 * \fn ERegistroPlugins::pluginInfo()
 * Devuelve un puntero al plugin de EInfoProgramaInterface que esta activo.
 */
EInfoProgramaInterface *ERegistroPlugins::pluginInfo()
{
 if( _pluginInfo != 0 )
 {  return _pluginInfo;  }
 else
 {
  qWarning( "Llamando al pluginInfo antes de cargarlo" );
  abort();
 }
}


/*!
 * \fn ERegistroPlugins::plugins()
 * Devuelve el listado de punteros a los plugins que se encuentran cargados como un QList
 */
QList<EPlugin *> ERegistroPlugins::pluginsPunteros()
{
  return _plugins->values();
}

/*!
 * \fn ERegistroPlugins::plugins()
 * Devuelve el listado de punteros a los plugins que se encuentran cargados como un QList
 */
QStringList ERegistroPlugins::plugins()
{
    return _plugins->keys();
}

/*!
 *   \fn ERegistroPlugins::pluginsHash()
 * Devuelve un listado de los plugins que estan cargados con un Hash <nombre, puntero>
 */
QHash<QString, EPlugin *> *ERegistroPlugins::pluginsHash()
{ return _plugins; }

/*!
 * \fn ERegistroPlugins::agregarPlugin( EPlugin *obj )
 * Agrega el plugin pasado como parametro a la lista interna
 */
void ERegistroPlugins::agregarPlugin( EPlugin *obj )
{
 _plugins->insert( obj->nombre(),obj );
 preferencias *p = preferencias::getInstancia();
 p->inicio();
 p->beginGroup( "carga" );
 p->beginGroup( QString::number( QCoreApplication::applicationPid() ) );
 QStringList plugins = p->value( "plugins" ).toStringList();
 plugins.append( obj->nombre() );
 p->setValue( "plugins", plugins );
 p->sync();
 p->endGroup();
 p=0;
}

void ERegistroPlugins::agregarPlugin( QString nombre, QObject *obj )
{
    _plugins2->insert( nombre, obj );
}

/*!
 * \fn ERegistroPlugins::plugin( const QString &nombre )
 * Devuelve el objeto referenciante al plugin con ese nombre
 */
EPlugin* ERegistroPlugins::plugin( const QString &nombre )
{
 return _plugins->value( nombre );
}

QObject* ERegistroPlugins::pluginQObject( const QString &nombre )
{
    return _plugins2->value( nombre );
}


/*!
 * \fn ERegistroPlugins::setPluginInfo( EInfoProgramaInterface *obj )
 * Setea el objeto de plugin de informacion
 */
void ERegistroPlugins::setPluginInfo( EInfoProgramaInterface *obj )
{
 //qDebug() << "Seteando plugin de Info cliente: " << obj->nombrePrograma();
 _pluginInfo = obj;
}

/*!
 * \fn ERegistroPlugins::existePlugin( const QString &nombre )
 * Verifica la existencia de un plugin especifico en los plugins cargados
 * \return Verdadero si existe, Falso en caso contrario.
 */
bool ERegistroPlugins::existePlugin( const QString &nombre )
{
 return _plugins->contains( nombre );
}

/*!
 * \fn ERegistroPlugins::existePluginExterno( const QString &nombre )
 * Verifica la existencia de un plugin especifico en los plugins cargados
 * \return Verdadero si existe, Falso en caso contrario.
 */
bool ERegistroPlugins::existePluginExterno( const QString &nombre )
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "carga" );
    p->beginGroup( QString::number( QCoreApplication::applicationPid() ) );
    QStringList plug = p->value( "plugins" ).toStringList();
    p->endGroup();
    p->endGroup();
    p=0;
    return plug.contains( nombre );
}

/*!
 * \fn ERegistroPlugins::pluginInfoSeteado()
 * Devuelve si esta seteado el plugin de informacion del programa
 */
bool ERegistroPlugins::pluginInfoSeteado()
{
    if( _pluginInfo == 0 )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void ERegistroPlugins::setearPluginEmail(EInterfazEmail *obj)
{
    if( obj != 0 ) {
        _plugEmail = obj;
    }
}

EInterfazEmail *ERegistroPlugins::getPluginEmail()
{
    if( _plugEmail != 0 ) {
        return _plugEmail;
    }
    return 0;
}
