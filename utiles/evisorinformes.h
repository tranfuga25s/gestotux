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
#ifndef EVISORINFORMES_H
#define EVISORINFORMES_H

#include <QPrintPreviewWidget>
#include <QPrinterInfo>
#include <QDockWidget>
/**
 * \brief Visor de informes en vista de impresion
 *
 * Clase que encapsula un visor de informes en vista de impresion y muestra el contenido con su barra de herramientas y complementos
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EVisorInformes : public QPrintPreviewWidget
{
Q_OBJECT
public:
    EVisorInformes(QPrinter *impre = new QPrinter( QPrinterInfo::defaultPrinter() ), QWidget *parent = 0);
    ~EVisorInformes();
signals:
	void agregarVentana( QWidget * );
        void agregarDockWidget( Qt::DockWidgetArea, QDockWidget * );

protected slots:
    void primerPagina();
    void anterior();
    void ultimaPagina();
    void siguiente();
    void imprimir();
    void aPdf();

private:
	QPrinter *impresora;
};

#endif
