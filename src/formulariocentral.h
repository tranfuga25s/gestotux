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
#ifndef FORMULARIOCENTRAL_H
#define FORMULARIOCENTRAL_H

#include <QTabWidget>
#include <QDockWidget>
#include <QMap>
#include <QPair>
class QPushButton;
/**
 * \brief Formulario Central
 *
 * Formulario que muestra las ventanas internas del programa
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class FormularioCentral : public QTabWidget
{
Q_OBJECT
public:
    FormularioCentral( QWidget *parent = 0 );

protected slots:
    void cambioWidget( int id );
    void cerrarActivo();
    void cerraronDockPadre( QObject *obj );
    void cerraronDock( QObject *obj );

public slots:
    void agregarForm( QWidget *ventana );
    void agregarDock( Qt::DockWidgetArea, QDockWidget * );

private:
    QPushButton *PBCerrar;
    bool existeVentana( QString nombre );
    QMap<int, QPair< Qt::DockWidgetArea, QDockWidget *> > mapaDocks;
};

#endif
