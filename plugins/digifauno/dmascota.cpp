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
#include "dmascota.h"

DMascota::DMascota( QWidget *parent )
 : QSqlRelationalDelegate( parent )
{
}


DMascota::~DMascota()
{
}




/*!
    \fn DMascota::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	Crea el editor predefinido, excepto en la columna 4, que crea un combobox con las opciones de macho y hembra
	@param parent Padre sobre el cual se debe crear el hijo
	@param option Propiedades de estilo
	@param index Indice del cual se desea crear un editor
 */
QWidget * DMascota::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  if( index.column() == 4 )
  {
 	QComboBox *sexo = new QComboBox( parent );
	sexo->insertItem( 0, "Macho", QVariant( 0 ) );
	sexo->insertItem( 1, "Hembra", QVariant( 1 ) );
	return sexo;
  }
  else
  {
	return QSqlRelationalDelegate::createEditor( parent, option, index );
  }
}


/*!
    \fn DMascota::setEditorData( QWidget * editor, const QModelIndex & index ) const
	Setea los datos en el editor luego de su inicializacion.
	@param editor Puntero al editor
	@param index Indice a setear los datos
 */
void DMascota::setEditorData( QWidget * editor, const QModelIndex & index ) const
{
 if( index.column() == 6 )
 {
	QComboBox *edit = static_cast<QComboBox *>(editor);
	edit->setCurrentIndex( index.data( Qt::EditRole ).toInt() );
 }
 else
 {
	QSqlRelationalDelegate::setEditorData( editor, index );
 }
}


/*!
    \fn DMascota::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
	Genera el traspaso del valor actual del editor a datos en el modelo. En el caso de la columna 6, el indice del combobox
	@param editor Puntero al editor
	@param model Puntero al modelo de datos
	@param index Indice que se desa actualizar
 */
void DMascota::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
 if( index.column() == 6 )
 {
	QComboBox *edit = static_cast<QComboBox *>(editor);
	model->setData( index, edit->currentIndex() );
 }
 else
 {
	QSqlRelationalDelegate::setModelData( editor, model, index );
 }
}


/*!
    \fn DMascota::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index )
	Redimensiona la geometria del editor para que calce en el espacio especificado
	@param editor Puntero convertible al editor
	@param option Opcion que contiene el tamaño a redimensionar
	@param index Indice que se esta actualizando
 */
void DMascota::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index )
{
  if( index.column() == 6 )
 {
  QComboBox *e = static_cast<QComboBox *>(editor);
  e->setGeometry( option.rect );
  e->updateGeometry();
 }
 else
 {
	QSqlRelationalDelegate::updateEditorGeometry ( editor, option, index );
 }
}
