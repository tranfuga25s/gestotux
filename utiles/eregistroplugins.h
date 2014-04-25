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
#ifndef EREGISTROPLUGINS_H
#define EREGISTROPLUGINS_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QHash>
#include <qglobal.h>
#include <QExplicitlySharedDataPointer>
#include <QSharedData>
#include "eplugin.h"
#include "einterfazemail.h"
#include "preferencias.h"

class EInfoProgramaInterface;
/**
 * \brief Registrador de plugins cargados
 * Clase que mantiene la lista de plugins y los muestra en un formulario de preferencias
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class ERegistroPlugins : public QObject
{
Q_OBJECT
public:

    EInfoProgramaInterface *pluginInfo();
    QList<EPlugin *> pluginsPunteros();
    QStringList plugins();
    QHash<QString, EPlugin *> *pluginsHash();
    static ERegistroPlugins *getInstancia();

    EPlugin *plugin( const QString &nombre );
    QObject *pluginQObject( const QString &nombre );

    void setPluginInfo( EInfoProgramaInterface *obj );
    void agregarPlugin( EPlugin *obj );
    void agregarPlugin( QString nombre, QObject *obj );

    bool existePlugin( const QString &nombre );
    bool existePluginExterno( const QString & nombre );
    bool pluginInfoSeteado();

    void setearPluginEmail( EInterfazEmail *obj );
    EInterfazEmail *getPluginEmail();

private:
   ERegistroPlugins(QObject *parent = 0);
   ERegistroPlugins( const ERegistroPlugins & ); // oculto constructor de copia
   ERegistroPlugins &operator=(const ERegistroPlugins & ); // oculto operador de asignacion

   ~ERegistroPlugins();
   static ERegistroPlugins *instance;
   static EInterfazEmail *_plugEmail;
   QHash<QString, EPlugin *> *_plugins;
   QHash<QString, QObject *> *_plugins2;
   EInfoProgramaInterface *_pluginInfo;

};

#endif
