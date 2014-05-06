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
#include "edsbprecio.h"

#include <QLineEdit>
#include <QKeyEvent>
#include <QCoreApplication>

#include <QDebug>

EDSBPrecio::EDSBPrecio(QWidget *parent)
 : QDoubleSpinBox( parent )
{
 setPrefix( "$ " );
 setDecimals( 2 );
}

/*!
 * \fn EDSBPrecio::keyPressEvent ( QKeyEvent * event )
 * \param evento
 */
void EDSBPrecio::keyPressEvent( QKeyEvent * event )
{
 if( event->key() == Qt::Key_Period
  && event->modifiers().testFlag( Qt::KeypadModifier ) )
 {
     // Convierto el ingreso en una coma
     QLocale locale;
     abort();
     int pos_cursor = this->lineEdit()->cursorPosition();
     int pos_coma = this->lineEdit()->text().indexOf( "," );
     if( pos_coma != -1 && pos_cursor < pos_coma ) {
         this->lineEdit()->setText( this->lineEdit()->text().remove( pos_cursor, abs( pos_coma-pos_cursor ) ) );
     }
     this->lineEdit()->setText( this->lineEdit()->text().split( "," ).first().replace( ",", "" ).append( locale.decimalPoint() ) );
     event->accept();
 } else {
       QDoubleSpinBox::keyPressEvent( event );
 }
}

