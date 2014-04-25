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

#include "EFlecha.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPaintEngine>
#include "math.h"

EFlecha::EFlecha( QWidget *parent ) {
  this->setParent( parent );
}


EFlecha::EFlecha(QWidget *parent, QWidget *origen, QWidget *destino, int tamFlecha ) :
QWidget(parent), QLineF()
{
    this->setParent( parent );
    this->origen = origen;
    this->move( this->origen->pos() );
    this->destino = destino;
    this->tamFlecha = tamFlecha;
    this->resize( this->destino->x() - this->pos().x(), this->destino->y() - this->pos().y() + 20 );
    this->tamFlecha = tamFlecha;
    // Por ahora dibujo una flecha desde las esquinas
    this->p1().setX( origen->x() );
    this->p1().setY( origen->y() );
    this->p2().setX( destino->x() );
    this->p2().setY( destino->y() );
    calcularDistanciaMinima();
}

QSize EFlecha::sizeHint() const
{
    if( this->origen == 0 || this->destino == 0 ) {
        return QSize( 0, 0 );
    } else {
        return QRect( this->p1().toPoint(), this->p2().toPoint() ).size();
    }
}

void EFlecha::setearOrigen( QWidget *origen ) {
  this->origen = origen;
  this->p1().setX( origen->x() );
  this->p1().setY( origen->y() );
}

void EFlecha::setearDestino( QWidget *destino ) {
  this->destino = destino;
  this->p2().setX( destino->x() );
  this->p2().setY( destino->y() );
  calcularDistanciaMinima();
}

void EFlecha::calcularDistanciaMinima() {
  // Busco los lados del widget destino-origen mas cercanos
  QPoint t1 = QPoint( origen->pos().x() + origen->rect().width(),
                      origen->pos().y() + origen->rect().height() )
              - destino->pos();
  int d1 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x() + ( origen->rect().width() / 2 ),
               origen->pos().y() ) -
       QPoint( destino->pos().x() + ( destino->rect().width() / 2 ),
               destino->pos().y() + destino->rect().height() );
  int d2 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x() + origen->rect().width(),
               origen->pos().y() ) -
       QPoint( destino->pos().x(),
               destino->pos().y() + destino->rect().height() );
  int d3 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x() + origen->rect().width(),
               origen->pos().y() + ( origen->rect().height() / 2 ) ) -
       QPoint( destino->pos().x(),
               destino->pos().y() + ( destino->rect().height() / 2 ) );
  int d4 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x() + origen->rect().width(),
               origen->pos().y() + origen->rect().height() ) -
       QPoint( destino->pos().x(),
               destino->pos().y() );
  int d5 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x() + ( origen->rect().width() / 2 ),
               origen->pos().y() + origen->rect().height()  ) -
       QPoint( destino->pos().x() + ( destino->rect().width() / 2 ),
               destino->pos().y() );
  int d6 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x(),
               origen->pos().y() + origen->rect().height()  ) -
       QPoint( destino->pos().x() + destino->rect().width(),
               destino->pos().y() );
  int d7 = t1.manhattanLength();
  t1 = QPoint( origen->pos().x(),
               origen->pos().y() + ( origen->rect().height() / 2 ) ) -
       QPoint( destino->pos().x() + destino->rect().width(),
               destino->pos().y() + ( destino->rect().width() / 2 ) );
  int d8 = t1.manhattanLength();
  // Busco los menores
  int min = qMin( qMin( qMin( qMin( d1, d2 ), qMin( d3, d4 ) ), qMin( d5, d6 ) ), qMin( d7, d8 ) );
  if( min == d1 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x() + origen->rect().width(),
                                           origen->pos().y() + origen->rect().height() ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x(),
                                           destino->pos().y() ) );
  } else if( min == d2 ) {

      *inicio = this->mapToGlobal( QPoint( origen->pos().x() + ( origen->rect().width() / 2 ),
                                           origen->pos().y() + origen->rect().height() ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x() + ( destino->rect().width() / 2 ),
                                           destino->pos().y() ) );
  } else if( min == d3 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x(),
                                           origen->pos().y() + origen->rect().height() ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x(),
                                           destino->pos().y() + destino->rect().height() ) );
  } else if( min ==  d4 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x() + origen->rect().width(),
                                           origen->pos().y() + ( origen->rect().height() / 2 ) ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x(),
                                           destino->pos().y() + ( destino->rect().height() / 2 ) ) );
  } else if( min == d5 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x() + origen->rect().width(),
                                           origen->pos().y() + origen->rect().height() ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x(),
                                           destino->pos().y() ) );
  } else if( min == d6 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x() + ( origen->rect().width() / 2 ),
                                           origen->pos().y() + origen->rect().height() ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x() + ( destino->rect().width() / 2 ),
                                           destino->pos().y() ) );
  } else if( min == d7 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x(),
                                           origen->pos().y() + origen->rect().height() ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x() + destino->rect().width(),
                                           destino->pos().y() ) );
  } else if( min == d8 ) {
      *inicio = this->mapToGlobal( QPoint( origen->pos().x(),
                                           origen->pos().y() + ( origen->rect().height() / 2 ) ) );
      *fin    = this->mapToGlobal( QPoint( destino->pos().x() + destino->rect().width(),
                                           destino->pos().y() + ( destino->rect().height() / 2 ) ) );
  } else {
          inicio->setX(0);
          inicio->setY(0);
          fin->setX(0);
          fin->setY(0);
  }
}

void EFlecha::setearTamFlecha( int tam )
{ this->tamFlecha = tam; }


void EFlecha::paintEvent( QPaintEvent * /*evento */ )
{
    this->setP1( *inicio );
    this->setP2( *fin );
    if( tamFlecha == 0  ) {  tamFlecha = 20; }
    QPoint f1;
    f1.setX( tamFlecha * cos( M_PI * ( 3.0 / 4.0 ) ) );
    f1.setY( tamFlecha * sin( M_PI * ( 3.0 / 4.0 ) ) );

    QPainter p(this);
    p.setPen( Qt::black );
    p.drawLine( this->p1(), this->p2() );
     p.save();
     p.translate( p2().x(), p2().y() );
     p.drawLine( QPointF( 0, 0 ), f1 );
     p.save();
      p.rotate( 90 );
      p.drawLine( QPointF( 0, 0 ), f1 );
     p.restore();
    p.restore();
    // fin
}
