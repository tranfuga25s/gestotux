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
#include "edcolor.h"
#include <QColor>
#include <QComboBox>

EDColor::EDColor(QObject *parent)
 : QItemDelegate(parent)
{
}


EDColor::~EDColor()
{
}


QWidget* EDColor::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
 QComboBox *combo = new QComboBox( parent );
 foreach( QString color, QColor::colorNames() )
 {
  combo->insertItem( -1, color );
  combo->setItemData( combo->count(), QColor( color ), Qt::BackgroundColorRole );
 }
 combo->setAutoCompletion( true );
 combo->setEditable( true );
 return combo;
}

