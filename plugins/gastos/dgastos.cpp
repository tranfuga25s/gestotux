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
#include "dgastos.h"

DGastos::DGastos(QObject *parent)
 : QSqlRelationalDelegate(parent)
{
}


DGastos::~DGastos()
{
}


QWidget* DGastos::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
 switch( index.column() )
 {
        case 1:
        {
                QComboBox *combo = qobject_cast<QComboBox *>( QSqlRelationalDelegate::createEditor( parent, option, index));
                QSqlTableModel *modelo = qobject_cast<QSqlTableModel *>(combo->model());
                modelo->setFilter( "tipo = '2'" );
                combo->setModel( modelo );
                return combo;
                break;
        }
        default:
        {
                return QSqlRelationalDelegate::createEditor(parent, option, index);
                break;
        }
 }
}
