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
#ifndef MPREFCATEGORIAS_H
#define MPREFCATEGORIAS_H

#include <QSqlTableModel>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MPrefCategorias : public QSqlTableModel
{
Q_OBJECT
public:
    MPrefCategorias(QObject *parent = 0);
    ~MPrefCategorias();
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );

};

#endif
