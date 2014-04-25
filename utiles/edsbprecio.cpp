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

#include <QLatin1Char>
#include <QKeyEvent>
#include <QCoreApplication>

EDSBPrecio::EDSBPrecio(QWidget *parent)
 : QDoubleSpinBox( parent )
{
 setPrefix( "$ " );
}

/*!
    \fn EDSBPrecio::keyPressEvent ( QKeyEvent * event )
 */
void EDSBPrecio::keyPressEvent( QKeyEvent * event )
{
 //qDebug( QString( "Tecla: %1, texto: %2" ).arg( event->nativeScanCode() ).arg( event->text()).toLocal8Bit() );
 switch( event->nativeScanCode() )
 {
#ifdef Q_WS_X11
     case 91:
     {
       QKeyEvent *ev = new QKeyEvent( event->type(), Qt::Key_Comma, event->modifiers(), ",", event->isAutoRepeat(), event->count() );
       ev->setAccepted( false );
       QCoreApplication::sendEvent( this, ev );
       break;
     }
#endif
#ifdef Q_WS_WIN
     case 83:
     {
       QKeyEvent *ev = new QKeyEvent( event->type(), Qt::Key_Comma, event->modifiers(), ",", event->isAutoRepeat(), event->count() );
       ev->setAccepted( false );
       QCoreApplication::sendEvent( this, ev );
       break;
     }
#endif
     default:
     {
       QDoubleSpinBox::keyPressEvent( event );
       break;
     }
   }
}

