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
#ifndef LISTADOCARAVANAS_H
#define LISTADOCARAVANAS_H

#include <eplugin.h>
#include <einformeinterface.h>

/**
 * \brief Plugin de informe de caravanas x establecimiento
 *
 *	Clase base del plugin que muestra la lista de caravanas que se encuentran en un establecimiento en cierta fecha
 * 	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class ListadoCaravanas : public QObject, public EInformeInterface
{
Q_OBJECT
Q_INTERFACES( EInformeInterface )
public:
    QString nombre() const;
    double version() const;
    void crearMenu( QMenu *menu );
    bool inicializar();
    QList<QAction *>  accionesBarra();

signals:
    void agregarVentana( QWidget *ventana );

private:
       /*!
        * Accion para el menu
        */
	QAction *ActInformeCaravanas;
public slots:
    void informeCaravanas();
};

#endif
