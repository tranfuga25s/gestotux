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
#ifndef DMIEMBROS_H
#define DMIEMBROS_H

#include <QItemDelegate>

/**
 * 	\brief Clase que ayuda la edicion de miembros
 *
 * Delegate que permite personalizar la edicion de los campos del formulario de miembros.\n
 * Cuando se editan numeros de telefono, restringe las entradas a numeros unicamente.\n
 * Cuando se editan direccionde de email, se restringe la entrada para que contenga efectivamente una direccion valida.
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class DMiembros : public QItemDelegate
{
Q_OBJECT
public:
    DMiembros(QObject *parent = 0);

    ~DMiembros();

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

};

#endif
