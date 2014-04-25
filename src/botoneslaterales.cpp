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
#include "botoneslaterales.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QIcon>
#include "eplugin.h"
#include "eregistroplugins.h"


BotonesLaterales::BotonesLaterales(QWidget* parent, Qt::WindowFlags f )
: QToolBox( parent, f )
{
    setObjectName( QString::fromUtf8( "TBacciones" ) );

    // El padre debe ser gestotux
    QList<EPlugin*> plugins = ERegistroPlugins::getInstancia()->pluginsPunteros();
    EPlugin *plug;
    foreach( plug, plugins )
    {
	if( !plug->accionesBarra().isEmpty() )
	{
		//qDebug( qPrintable( "accionesbarra: " + plug->nombre() ) );
		QActionGroup *pest;
		foreach( pest, plug->accionesBarra() )
		{
			if( !pest->actions().isEmpty() )
			{
				QWidget *w;
				//qDebug( qPrintable( "grupo: " + pest->objectName() ) );
				// Existe ya el grupo?
				if( this->findChild<QWidget*>( pest->objectName() ) != 0 )
				{
					//Existe
					w = this->findChild<QWidget *>( pest->objectName() );
					if( w->windowIcon().isNull() && pest->property( "icono" ).isValid() )
					{
						this->setItemIcon( this->indexOf( w ), QIcon( pest->property( "icono" ).toString() ) );
					}
					if( w->windowTitle().isEmpty() && pest->property( "titulo" ).isValid() )
					{
						this->setItemText( this->indexOf( w ), pest->property( "titulo" ).toString() );
					}
				}
				else
				{
					// No existe, lo creo
					w = new QWidget( this );
					QVBoxLayout *layout = new QVBoxLayout( w );
					w->setObjectName( pest->objectName() );
					this->addItem( w, QIcon( pest->property( "icono" ).toString()), pest->property("titulo").toString() );
					QSpacerItem *espaciador = new QSpacerItem( 10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
					layout->addItem( espaciador );
				}
				// Agrego las acciones
				QAction *act;
				foreach( act, pest->actions() )
				{
					QPushButton *b = new QPushButton( this );
					b->setText( act->text() );
					b->setIcon( act->icon() );
					b->setFlat( true );
					connect( b, SIGNAL( clicked() ), act, SIGNAL( triggered() ) );
					qobject_cast<QVBoxLayout *>(w->layout())->insertWidget( w->layout()->count() -1, b );
				}
			}// fin actiongroup vacio
		} // fin repeticion x actiongroup
	} // fin de si esta vacia
    }
    this->setMinimumWidth( 180 );
}


BotonesLaterales::~BotonesLaterales()
{
}
