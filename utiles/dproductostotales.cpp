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
#include "dproductostotales.h"
#include <QLineEdit>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QCompleter>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include "edsbprecio.h"
#include "../plugins/productos/mproductos.h"
#include "preferencias.h"
#include "ecbproductos.h"

DProductosTotales::DProductosTotales( QWidget *parent )
 : QItemDelegate(parent)
{
 setClipping( false );
}

QWidget* DProductosTotales::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
 switch( index.column() )
 {
        // Cantidad
        case 0:
        {
           EDSBPrecio *e = new EDSBPrecio( parent );
           e->setRange( 0.0, 99999999.999 );
           e->setPrefix("");
           preferencias *p = preferencias::getInstancia();
           p->beginGroup( "Preferencias" );
           p->beginGroup( "Productos" );
           p->beginGroup( "Stock" );
           if( p->value( "limitar", false ).toBool() )
           {
             e->setMaximum( MProductos::stock( index.model()->data( index.model()->index( index.row(), 0 ), Qt::EditRole ).toInt() ) );
           }
           p->endGroup(); p->endGroup(); p->endGroup(); p = 0;
           return e;
           break;
        }
        // Producto
        case 1:
        {
                return 0;
                break;
        }
        // Precio Unitario
        case 2:
        {
                EDSBPrecio *e = new EDSBPrecio( parent );
                e->setPrefix( "$" );
                e->setSingleStep( 1 );
                e->setRange( 0.00, 99999999.99 );
                return e;
                break;

        }
        default:
        {
                // Los demas no deben ser editables
                return QItemDelegate::createEditor(parent, option, index);
                break;
        }
 }
 return QItemDelegate::createEditor( parent, option, index );
}

void DProductosTotales::setEditorData(QWidget* editor, const QModelIndex& index) const
{
 switch( index.column() )
 {
        // Producto
        case 1:
        {
             QComboBox *combo = qobject_cast<QComboBox *>(editor);
             combo->setCurrentIndex( combo->findData(  index.model()->data(index, Qt::EditRole ).toInt() ) );
             break;
        }
        // Precio unitario y cantidad
        case 0:
        case 2:
        {
                EDSBPrecio *e = qobject_cast<EDSBPrecio *>( editor );
                e->setValue( index.data( Qt::EditRole ).toDouble() );
                break;
        }
        default:
        {
                // Los demas no deben ser editables
                return QItemDelegate::setEditorData( editor, index);
                break;
        }
 }
}

void DProductosTotales::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  switch( index.column() )
 {
        // Producto
        case 1:
        {
                ECBProductos *combo = qobject_cast<ECBProductos *>(editor);
                // No necesito preocuparme si ingresa cualquier banana porque el cb administra la lista
                model->setData( index, combo->idActual() );
                break;
        }
        // Precio Unitario
        case 2:
        // Cantidad
        case 0:
        {
                EDSBPrecio *e = qobject_cast<EDSBPrecio *>( editor );
                model->setData( index, e->value() );
                break;
        }
        default:
        {
                // Los demas no deben ser editables
                return QItemDelegate::setEditorData( editor, index);
                break;
        }
 }
}

void DProductosTotales::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QItemDelegate::updateEditorGeometry(editor, option, index);
}

