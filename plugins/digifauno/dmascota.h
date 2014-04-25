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
#ifndef DMASCOTA_H
#define DMASCOTA_H

#include <QSqlRelationalDelegate>

/**
	\brief Clase de edicion de mascotas

	Clase que permite la correcta edicion de los valores de los campos de las mascotas

	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class DMascota : public QSqlRelationalDelegate
{
Q_OBJECT
public:
    DMascota(QWidget *parent = 0);
    ~DMascota();
    QWidget * createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void setEditorData( QWidget * editor, const QModelIndex & index ) const;
    void setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
    void updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index );
};

#endif
