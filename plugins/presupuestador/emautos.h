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
#ifndef EMAUTOS_H
#define EMAUTOS_H

#include <QSqlQueryModel>

/**
\brief Modelo para seleccion de autos x combobox

Modelo que muestra el auto con datos concatenados y se puede filtrar por duenos

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class EMAutos : public QSqlQueryModel
{
Q_OBJECT
public:
    EMAutos(QObject *parent = 0);
    ~EMAutos();

public slots:
    void filtrarPorCliente( int id_cliente );
};

#endif
