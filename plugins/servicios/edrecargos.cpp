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
#include "edrecargos.h"

#include <QDoubleSpinBox>
#include <QComboBox>

EDRecargos::EDRecargos(QObject *parent)
 : QSqlRelationalDelegate(parent)
{
}

/*!
    \fn EDRecargos::createEditor( QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex &index ) const
 */
QWidget * EDRecargos::createEditor( QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex &index ) const
{
 // Creo el editor dependiendo de la columna asiganada
 switch( index.column() )
 {
  // Dias pasados
  case 2:
  {
   QComboBox *editor = new QComboBox( parent );
   for( int i = 1; i<=31; i++ )
   {
    editor->addItem( QString::number( i ), i );
   }
   return editor;
   break;
  }
  case 3:
  {
   QDoubleSpinBox *editor = new QDoubleSpinBox( parent );
   editor->setSuffix( " %" );
   editor->setRange( 0.0, 999.99 );
   return editor;
   break;
  }
  case 4:
  {
   QDoubleSpinBox *editor = new QDoubleSpinBox( parent );
   editor->setPrefix( "$ " );
   editor->setRange( 0.0, 9999999999999999999.999 );
   return editor;
   break;
  }
  default:
  { return QSqlRelationalDelegate::createEditor( parent, option, index ); break; }
 }
}


/*!
    \fn EDRecargos::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex & index ) const
 */
void EDRecargos::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex & index ) const
{
  switch( index.column() )
 {
  // Dias pasados
  case 2:
  {
   QComboBox *edit = qobject_cast<QComboBox *>(editor);
   model->setData( index, edit->itemData( edit->currentIndex() ).toInt() );
   break;
  }
  case 3:
  case 4:
  {
   QDoubleSpinBox *edit = qobject_cast<QDoubleSpinBox *>(editor);
   model->setData( index, edit->value() );
   break;
  }
  default:
  { return QSqlRelationalDelegate::setModelData( editor, model, index ); break; }
 }
}


/*!
    \fn EDRecargos::setEditorData( QWidget *editor, const QModelIndex &index ) const
 */
void EDRecargos::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
 switch( index.column() )
 {
  // Dias pasados
  case 2:
  {
   QComboBox *edit = qobject_cast<QComboBox *>(editor);
   edit->setCurrentIndex( index.model()->data( index, Qt::EditRole ).toInt() );
   break;
  }
  case 3:
  case 4:
  {
   QDoubleSpinBox *edit = qobject_cast<QDoubleSpinBox *>(editor);
   edit->setValue( index.model()->data( index, Qt::EditRole ).toDouble() );
   break;
  }
  default:
  { return QSqlRelationalDelegate::setEditorData( editor, index ); break; }
 }
}
