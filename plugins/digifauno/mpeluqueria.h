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
#ifndef MPELUQUERIA_H
#define MPELUQUERIA_H

#include <QSqlRelationalTableModel>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MPeluqueria : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MPeluqueria(QObject *parent = 0, bool relacion = false );
    ~MPeluqueria();
    QVariant data(const QModelIndex& item, int role) const;
    bool setData(const QModelIndex& item, const QVariant& value, int role);
    bool agregarServicio( QVariant desc, QVariant precio, QVariant fecha, QVariant mascota );
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    void setSolo_lectura( bool theValue ) { _solo_lectura = theValue;	}
    bool solo_lectura() const { return _solo_lectura; }
    void setLimite( int lim );
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

protected:
    int limite;
    bool _solo_lectura;
};

#endif
