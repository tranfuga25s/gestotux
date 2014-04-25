/***************************************************************************
 *   Copyright (C) 2007 by Esteban Zeller                                  *
 *   juiraze@yahoo.com.ar                                                  *
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
#include "eactpdf.h"

EActPdf::EActPdf(QWidget *parent)
 : QAction(parent)
{
 this->setText( "Pdf" );
 this->setShortcut( QKeySequence( Qt::CTRL + Qt::SHIFT + Qt::Key_P ) );
 this->setIcon( QIcon( ":/imagenes/acroread.png" ) );
 this->setStatusTip( tr( "Guarda el informe actual en formato pdf( Ctrl + Shift + p )" ) );
 this->connect( this, SIGNAL( triggered() ), parent, SLOT( aPdf() ) );
}


EActPdf::~EActPdf()
{
}


