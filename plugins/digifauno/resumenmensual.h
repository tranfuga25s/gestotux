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
#ifndef RESUMENMENSUAL_H
#define RESUMENMENSUAL_H

#include <QObject>
class VisorResumenes;
#include <QMap>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class resumenMensual : public QObject
{
Q_OBJECT
public:
    resumenMensual( QObject *parent = 0, int mes = 0 );
    ~resumenMensual();

    void generar( bool detallado );
    void setvisorResumen( VisorResumenes* theValue );
    VisorResumenes* visorResumen() const;
    void renderizar();

private:
	VisorResumenes *form;
	int mes;
	QMap<QString, QVariant> valores;
};

#endif
