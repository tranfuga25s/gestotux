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
#include "dproveedor.h"

#include <QLineEdit>
#include <QModelIndex>
#include <QMessageBox>
#include "ELECuitCuil.h"

DProveedor::DProveedor(QObject *parent)
 : QItemDelegate(parent)
{
}


QWidget* DProveedor::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
 switch( index.column() )
 {
        // CUIT/CUIL
        case 7:
        {
            ELECuitCuil *editor = new ELECuitCuil( parent );
            return editor;
        }
	// E-Mail
	case 3:
	{
		QLineEdit *editor = new QLineEdit( parent );
 		editor->setValidator( new QRegExpValidator( QRegExp( "[a-z0-9_]{1,99}\\S@[a-z]{1,99}\\S\\.[a-z]{1,99}\\S+" ), editor ) );
		editor->setAlignment( Qt::AlignRight );
		return editor;
	}
	//Numero de telefonos
	case 4:
	case 5:
	case 6:
	{
		QLineEdit *editor = new QLineEdit( parent );
 		editor->setValidator( new QRegExpValidator( QRegExp( "[0-9_- ]+" ), editor ) );
		editor->setAlignment( Qt::AlignRight );
		return editor;
                break;
	}
	default:
	{
		return QItemDelegate::createEditor(parent, option, index);
	}
 }
}

void DProveedor::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QItemDelegate::setEditorData(editor, index);
}

void DProveedor::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  switch( index.column() )
 {
	// E-Mail
	case 3:
	case 4:
	case 5:
	case 6:
	{
		QLineEdit *l = qobject_cast<QLineEdit *>(editor);
		if( l->hasAcceptableInput() )
		{
			QItemDelegate::setModelData( editor, model, index );
			return;
		}
		else
		{
			QMessageBox::warning( editor, "Error de ingreso", "El dato ingresado es incorrecto. Por favor verifique que sea un dato completo" );
			return;
		}
	}
        case 7:
        {
                ELECuitCuil *ed = qobject_cast<ELECuitCuil *>(editor);
                if( ed->verificar() )  {
                    QItemDelegate::setModelData( editor, model, index );
                } else {
                    QMessageBox::warning( editor, "Error de ingreso", "El dato ingresado es incorrecto. Por favor, ingrese un cuit/cuil correcto" );
                    return;
                }
                break;
        }
	default:
	{
		QItemDelegate::setModelData(editor, model, index);
		return;
	}
 }
}

