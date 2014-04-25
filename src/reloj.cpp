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
#include "reloj.h"

#include <QTimer>
#include <QTime>
#include <QCloseEvent>
#include "preferencias.h"

Reloj::Reloj(QWidget *parent)
 : QLCDNumber( parent )
{
 setSegmentStyle(Filled);
 this->setAttribute( Qt::WA_DeleteOnClose );
 timer = new QTimer(this);
 connect( timer, SIGNAL( timeout() ), this, SLOT( showTime() ) );
 timer->start(1000);
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "Ventanas" );
 QSize tam = p->value( "Reloj", this->size() ).toSize();
 p->endGroup(); p = 0;
 this->resize( tam );
}


Reloj::~Reloj()
{
    timer->stop();
    delete timer;
}

void Reloj::showTime()
{
     QTime time = QTime::currentTime();
     QString text = time.toString("hh:mm");
     if ((time.second() % 2) == 0)
         text[2] = ' ';
     display(text);
}

void Reloj::closeEvent( QCloseEvent *e ) {
    // Guardo mi tamaño
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Ventanas" );
    p->setValue( "Ventanas/Reloj", this->size() );
    p->endGroup();
    p = 0;
    e->accept();
}
