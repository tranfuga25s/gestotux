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
#ifndef MDUENO_H
#define MDUENO_H

#include <QSqlTableModel>

/**
Modelo que maneja los datos de un dueño

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MDueno : public QSqlTableModel
{
 Q_OBJECT
public:
    MDueno( QObject * parent = 0 );
    ~MDueno();

    QVariant data(const QModelIndex& idx, int role) const;
    int get_ultima_row() const;

protected:
    int ultima_row;
    void primeInsert ( int row, QSqlRecord & record );
};

#endif
