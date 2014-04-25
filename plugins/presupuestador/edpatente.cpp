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
#include "edpatente.h"
#include <QLineEdit>

EDPatente::EDPatente(QObject *parent)
 : QItemDelegate(parent)
{
}


EDPatente::~EDPatente()
{
}


QSize EDPatente::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QItemDelegate::sizeHint(option, index);
}

QWidget* EDPatente::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
 QLineEdit *editor = new QLineEdit( parent );
 editor->setInputMask( ">AAAa-999" );
 editor->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
 return editor;
}

void EDPatente::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QItemDelegate::setEditorData(editor, index);
}

void EDPatente::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QItemDelegate::setModelData(editor, model, index);
}

void EDPatente::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QItemDelegate::updateEditorGeometry(editor, option, index);
}

