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
#include "esplash.h"
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QPainter>
#include <QBitmap>
#include <QDesktopWidget>

ESplash::ESplash(QWidget *parent)
 : QSplashScreen( parent )
{
    //setAttribute( Qt::WA_DeleteOnClose );
    //this->setAttribute( Qt::WA_OpaquePaintEvent );
    //this->setAttribute( Qt::WA_NoSystemBackground );
    //this->setAutoFillBackground( false );
    this->setWindowOpacity(0);
    if( QFile::exists( QApplication::applicationDirPath() + QDir::separator() + "splash.png" ) )
    {
      this->setPixmap( QApplication::applicationDirPath() + QDir::separator() + "splash.png" );
      this->setMask( QBitmap( QApplication::applicationDirPath() + QDir::separator() + "splashmask.png" ) );
    }
    else
    {
      QPixmap pix( ":/images/splash.png" );
      this->setPixmap( pix );
      this->setMask( pix.mask() );
    }
}

void ESplash::drawContents( QPainter *painter )
{
  painter->setPen( this->color );
  QRect pos = rect();
  pos.setRect( pos.x() + 200, pos.y() + 140, pos.width(), pos.height() );
  painter->drawText( pos, this->alineacion, this->texto );
}


void ESplash::showMessage(const QString &message, int alignment, const QColor &color) {
  this->texto = message;
  this->alineacion = alignment;
  this->color = color;
  emit messageChanged( this->texto );
  repaint();
}

/*void ESplash::paintEvent(QPaintEvent *)
{
    QColor backgroundColor = palette().light().color();
    backgroundColor.setAlpha(0);
    QPainter customPainter(this);
    customPainter.fillRect(rect(),backgroundColor);
}*/
