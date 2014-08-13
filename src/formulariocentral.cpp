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
#include "formulariocentral.h"

#include "gestotux.h"
#include <QToolBar>
#include <QPushButton>
#include <QTabBar>
#include <QDebug>

FormularioCentral::FormularioCentral( QWidget *parent )
: QTabWidget( parent )
{
 connect( this, SIGNAL( currentChanged( int ) ), this, SLOT( cambioWidget( int ) ) );
 PBCerrar = new QPushButton( this );
 PBCerrar->setFlat( true );
 PBCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
 this->setCornerWidget( PBCerrar );
 connect( PBCerrar, SIGNAL( clicked() ), this, SLOT( cerrarActivo() ) );
}


void FormularioCentral::cambioWidget( int id )
{
 gestotux::barraAcciones()->clear();
 if( id > -1 )
 {
  gestotux::barraAcciones()->addActions( this->widget( id )->actions() );
 }
 if( this->widget( id )->objectName() == "inicio" ) {
     this->cornerWidget()->setVisible(false);
 } else { this->cornerWidget()->setVisible(true); }
 if( mapaDocks.contains( id ) ) {
     mapaDocks.value( id ).second->setVisible( true );

 }
}


void FormularioCentral::cerrarActivo()
{
    if( mapaDocks.contains( currentIndex() ) ) {
        mapaDocks.value( currentIndex() ).second->hide();
        mapaDocks.remove( currentIndex() );
    }
    currentWidget()->close();
    removeTab( currentIndex() );
    cambioWidget( currentIndex() );
}


/*!
    \fn FormularioCentral::agregarForm( QWidget *ventana )
 */
void FormularioCentral::agregarForm( QWidget *ventana )
{
 if( ventana->objectName().isEmpty() )
 {
  qWarning( "intentando agregar ventana sin nombre, no aparecera" );
  return;
 }
 if( !existeVentana( ventana->objectName() ) )
 {
  connect( ventana, SIGNAL( agregarVentana( QWidget * ) ), this, SLOT( agregarForm( QWidget * ) ) );
  connect( ventana, SIGNAL( agregarDockWidget( Qt::DockWidgetArea, QDockWidget * ) ), this, SLOT( agregarDock( Qt::DockWidgetArea, QDockWidget * ) ) );
  this->setCurrentIndex( this->addTab( ventana, ventana->windowIcon(), ventana->windowTitle() ) );
  this->tabBar()->setTabData( currentIndex(), ventana->objectName() );
  this->currentWidget()->setFocus( Qt::ActiveWindowFocusReason );
 }
 else
 {
   // Busco el form que tiene la ventana y lo pongo como actual
   qWarning( "Ya existe la ventana que esta intentando abrir.\n Se mostrar la que existe abierta." );
   qDebug() << "Ventana ya abierta: " << ventana->objectName();
   this->setCurrentWidget( this->findChild<QWidget *>( ventana->objectName() ) );
   ventana->deleteLater();
 }
}


/*!
    \fn FormularioCentral::existeVentana( QString nombre )
 */
bool FormularioCentral::existeVentana( QString nombre )
{
 for( int i=0; i < this->tabBar()->count(); i++ )
 {
  if( this->tabBar()->tabData( i ) == nombre )
  { return true; }
 }
 return false;
}

/*
  \fn FormularioCentral::agregarDock( Qt::DockWidgetArea pos, QDockWidget *obj )
  Pasarela para que la ventana principal agrege una ventana tipo dockwidget.
  Si el objeto no tiene un padre que pertenezca a las ventanas actuales, no se ocultará al cambiar de pestaña
  \param pos Posición donde se desea colocar el widget
  \param obj DockWidget que se desea colocar
 */
void FormularioCentral::agregarDock( Qt::DockWidgetArea pos, QDockWidget *obj )
{
    // agregar mappeo para que al cambiar de pestaña se oculten o muestren
    if( this->indexOf( obj->parentWidget() ) == -1 ) {
        qDebug( "Intentando insertar un dockwidget que no tiene padre, no se ocultará cuando se cambie de pestaña" );
    }
    connect( obj->parentWidget(), SIGNAL( destroyed( QObject * ) ), this, SLOT( cerraronDockPadre( QObject * ) ) );
    connect( obj, SIGNAL( destroyed( QObject * ) ), this, SLOT( cerraronDock( QObject * ) ) );
    qobject_cast<QWidget *>(obj)->setAttribute( Qt::WA_DeleteOnClose );
    mapaDocks.insertMulti( this->indexOf( obj->parentWidget() ), QPair<Qt::DockWidgetArea, QDockWidget *>( pos, obj ) );
    qobject_cast<gestotux *>(this->parent())->agregarDock( pos, obj );
}

/**
 * @brief FormularioCentral::cerraronDockPadre
 * @param obj
 */
void FormularioCentral::cerraronDockPadre( QObject *obj )
{
    // obj es el padre de algun dockwidget
    // Busco el id del padre
    int indice = this->indexOf( qobject_cast<QWidget *>( obj ) );
    if( indice == -1 ) {
        qDebug( "Señal implementada para un padre que no esta en la lista" );
        return;
    }
    if( mapaDocks.contains( indice ) ) {
        // tengo que cerrar ese elemento
        QDockWidget *hijo = qobject_cast<QDockWidget *>(mapaDocks.value( indice ).second);
        if( hijo->isVisible() )
        { hijo->close(); }
        mapaDocks.remove( indice );
    }

}

/**
 * @brief FormularioCentral::cerraronDock
 * @param obj
 */
void FormularioCentral::cerraronDock( QObject *obj )
{
    foreach( int i, mapaDocks.keys() ) {
        QDockWidget *doc = mapaDocks.value( i ).second;
        if( doc == obj ) {
            mapaDocks.remove( i );
            return;
        }
    }
    //qWarning() << "No se encontro como eliminar el dock";
}
