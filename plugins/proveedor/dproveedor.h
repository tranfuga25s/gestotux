/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller   *
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
#ifndef DPROVEEDOR_H
#define DPROVEEDOR_H

#include <QItemDelegate>

/**
 * 	\brief Clase que ayuda la edicion de proveedores
 *
 * Delegate que permite personalizar la edicion de los campos del formulario de Proveedores.\n
 * Cuando se editan numeros de telefono, restringe las entradas a numeros unicamente.\n
 * Cuando se editan direccionde de email, se restringe la entrada para que contenga efectivamente una direccion valida.
 * Tambien se verifica que se utilice un cuit/cuil correcto.
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class DProveedor : public QItemDelegate
{
Q_OBJECT
public:
    DProveedor(QObject *parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif
