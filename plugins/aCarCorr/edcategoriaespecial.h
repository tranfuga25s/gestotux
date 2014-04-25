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
#ifndef EDCATEGORIAESPECIAL_H
#define EDCATEGORIAESPECIAL_H

#include <QSqlRelationalDelegate>

/**
 * \brief Delegate de categorias especiales
 *
 * Delegate que permite activar una categoria como especial pero no volverla a normal... :P
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EDCategoriaEspecial : public QSqlRelationalDelegate
{
Q_OBJECT
public:
    EDCategoriaEspecial(QWidget *parent = 0);

    ~EDCategoriaEspecial();

};

#endif
