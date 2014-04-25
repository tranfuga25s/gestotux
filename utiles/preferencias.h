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
#ifndef PREFERENCIAS_H
#define PREFERENCIAS_H

#include <QSettings>

/**
    \brief Almacen de preferencias

    Clase que almacena las prefencias y brinda acceso a ellas con metodos con validacion automatica

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class preferencias : public QSettings
{
public:
    ~preferencias();
    static preferencias *getInstancia();
    void inicio();

private:
    preferencias();
    preferencias( preferencias & );
    preferencias( const preferencias & );
    /*!
     * Puntero que guarda una referencia estatica a si mismo
     * De esa manera se mantiene las mismas preferencias a pesar de ser llamado muchas veces
     */
    static preferencias *instance;
};

#endif
