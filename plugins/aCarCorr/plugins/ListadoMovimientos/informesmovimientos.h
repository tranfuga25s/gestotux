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
#ifndef INFORMESMOVIMIENTOS_H
#define INFORMESMOVIMIENTOS_H

#include <QObject>
#include "../../einformeinterface.h"
#include <QtPlugin>
#include <QFile>
class QAction;

/**
 * \brief Plugin de informe completo
 *
 * Este plugin realiza el informe completo de movimientos mediante filtros definidos
 *
 *	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class InformesMovimientos : public QObject, public EInformeInterface
{
 Q_OBJECT
 Q_INTERFACES(EInformeInterface)
public:
    QString nombre() const;
    bool inicializar();
    double version() const;
    void crearMenu(QMenu* m);
    QString cargarArchivoEstilo();
    QString cargarCabecera();
    QList<QAction *>  accionesBarra();
private:
       /*!
        * Accion para el menu de informes
        */
	QAction *ActInformeFiltroTotal;
       /*!
	* Puntero al archivo de estilo usado
	*/
	QFile *archivoEstilo;

public slots:
	void informeCompleto();

signals:
	void agregarVentana( QWidget *v );
};

#endif
