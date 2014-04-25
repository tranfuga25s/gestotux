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
#ifndef INFORMESPLUGIN_H
#define INFORMESPLUGIN_H

#include "eplugin.h"
class QPluginLoader;
#include <QAction>
#include "einformeinterface.h"

/**
 * \brief Plugin que carga los informes
 *
 * Plugin que carga la lista de informes y muestra y administra los menus
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class InformesPlugin : public QObject, public EPlugin
{
Q_OBJECT
Q_INTERFACES( EPlugin )
public:
    bool inicializar();
    bool verificarTablas( QStringList tablas );
    double version() const;
    int tipo() const;
    QList< QActionGroup * > accionesBarra();
    QString nombre() const;
    QWidgetList formsPreferencias();
    void crearMenu( QMenuBar* m );
    void crearToolBar( QToolBar* t );
    QAction *botonPantallaInicial() { return new QAction( this ); }
    void crearAccionesGlobales(QMainWindow *){}

signals:
    void agregarVentana(QWidget* v);
    void agregarDockWidget(Qt::DockWidgetArea area, QDockWidget *ventana);
    void seCierraGestotuxSignal();

public slots:
    void seCierraGestotux();

private:
	QPluginLoader *loader;
        QHash<QString, EInformeInterface *> *_plugins;

        QAction *ActActualizar;

private slots:
        void actualizar();

private:
    bool cargarPluginsInformes();
};

#endif
