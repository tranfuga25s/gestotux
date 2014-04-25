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
#ifndef EVENTANA_H
#define EVENTANA_H

#include <QWidget>
#include <QKeyEvent>
#include <QDockWidget>

/*!
 * 	\brief Ventana base de todas las ventanas
 *
 *  Ventana generica, metodos a implementar y cosas repetitivas implementadas ya
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EVentana : public QWidget
{
Q_OBJECT
public:
    EVentana( QWidget *padre = 0, Qt::WFlags fl = 0 );
    ~EVentana();
    void closeEvent( QCloseEvent *e );

protected:
   /**
    * Puntero al widget padre
    */
    QWidget *padre;

private:
    void keyPressEvent( QKeyEvent *event );

signals:
	void agregarVentana( QWidget * );
        void agregarDockWidget( Qt::DockWidgetArea, QDockWidget * );
};

#endif
