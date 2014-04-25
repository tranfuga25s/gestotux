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
#include "dproductos.h"

#include <QModelIndex>
#include "ecbcategoriasproductos.h"

#include <QSqlRecord>

DProductos::DProductos(QObject *parent)
: QSqlRelationalDelegate(parent)
{}

void DProductos::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    if( index.column() == 1 ) {
        ECBCategoriasProductos *combo = qobject_cast<ECBCategoriasProductos *>(editor);
        // El modelo te devuelve el nombre de la categoría
        combo->setearTexto( index.model()->data( index, Qt::DisplayRole ).toString() );
    } else {
        QSqlRelationalDelegate::setEditorData( editor, index );
    }
}

void DProductos::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( index.column() == 1 ) {
        ECBCategoriasProductos *combo = qobject_cast<ECBCategoriasProductos *>(editor);
        model->setData( index, combo->idActual(), Qt::UserRole );
    } else {
        QSqlRelationalDelegate::setModelData( editor, model, index );
    }
}
