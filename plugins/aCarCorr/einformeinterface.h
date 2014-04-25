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
#ifndef EINFORMEINTERFACE_H
#define EINFORMEINTERFACE_H

#include <QtPlugin>
#include <QPrinter>
#include <QMenu>
/**
 * \brief Interfaz para informes de caravanas
 *
 *  Interfaz para hacer un informe
 *  @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EInformeInterface
{
public:
    virtual ~EInformeInterface() {}
    virtual bool inicializar() = 0;
    virtual QString nombre() const = 0;
    virtual double version() const = 0;
    virtual void crearMenu( QMenu *menu ) = 0;
    virtual void agregarVentana( QWidget *v ) = 0;
    virtual QList<QAction *> accionesBarra() = 0;
};

Q_DECLARE_INTERFACE(EInformeInterface,
                    "tranfuga.EInformeInterface/1.1" );

#endif
