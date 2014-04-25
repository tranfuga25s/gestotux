/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller   				   *
 *   juiraze@yahoo.com.ar   						   *
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
#include "dsino.h"
#include <QCheckBox>
#include <QPainter>
#include <math.h>
#include <QPolygonF>
#include <QPointF>

const int PaintingScaleFactor = 20;

DSiNo::DSiNo(QObject *parent)
 : QItemDelegate(parent)
{
    crossPolygon << QPointF( 0.15, 0.00 ) << QPointF( 0.50, 0.35 ) << QPointF( 0.85, 0.00 ) << QPointF( 1.00, 0.15 )
                << QPointF( 0.65, 0.50 ) << QPointF( 1.00, 0.85 ) << QPointF( 0.85, 1.00 ) << QPointF( 0.50, 0.65 )
                << QPointF( 0.15, 1.00 ) << QPointF( 0.00, 0.85 ) << QPointF( 0.35, 0.50 ) << QPointF( 0.00, 0.15 )
                << QPointF( 0.15, 0.00 );
    tickPolygon << QPointF( 0.00, 0.70 ) << QPointF( 0.30, 1.00 )
                   << QPointF( 1.00, 0.15 ) << QPointF( 0.85, 0.00 )
                   << QPointF( 0.30, 0.70 ) << QPointF( 0.15, 0.65 )
                   << QPointF( 0.00, 0.70 );
}


DSiNo::~DSiNo()
{
}


QWidget* DSiNo::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
 QCheckBox *editor = new QCheckBox( parent );
 editor->setText( index.model()->headerData( index.column(), Qt::Horizontal ).toString() );
 editor->setTristate( false );
 editor->setPalette( option.palette );
 editor->setFont( option.font );
 editor->setBackgroundRole( QPalette::Background );
 return editor;
}

void DSiNo::setEditorData(QWidget* editor, const QModelIndex& index) const
{
 qobject_cast<QCheckBox *>(editor)->setChecked( index.data( Qt::EditRole ).toBool() );
}

void DSiNo::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
 model->setData( index, qobject_cast<QCheckBox *>(editor)->isChecked(), Qt::EditRole );
}

/*!
    \fn DSiNo::paint(QPainter *painter, const QStyleOptionViewItem &option,  const QModelIndex &index) const
 */
void DSiNo::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBackgroundMode( Qt::TransparentMode );

    if( option.state & QStyle::State_Selected )
    {
        painter->setBrush( option.palette.alternateBase() );
        painter->fillRect( option.rect, option.palette.highlight());
    }
    else
    {
        painter->setBrush( option.palette.highlight() );
    }
    int yOffset = ( option.rect.height() - PaintingScaleFactor ) / 2;
    int xOffset = ( option.rect.width() - PaintingScaleFactor ) / 2;
    painter->translate( option.rect.x() + xOffset, option.rect.y() + yOffset);
    painter->scale( PaintingScaleFactor, PaintingScaleFactor);

    if ( index.data(Qt::EditRole).toBool() ) {
            painter->drawPolygon( tickPolygon, Qt::WindingFill );
        } else {
            painter->drawPolygon( crossPolygon, Qt::WindingFill );
        }
    painter->translate(1.0, 0.0);
    painter->restore();
}


/*!
    \fn DSiNo::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
 */
QSize DSiNo::sizeHint(const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    return PaintingScaleFactor * QSize( 1, 1 );
}