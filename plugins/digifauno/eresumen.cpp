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
#include "eresumen.h"
#include "resumengeneral.h"
#include "preferencias.h"
#include "evisorinformes.h"

#include <QPushButton>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <algorithm>
#include <QLabel>
#include <QDateEdit>
#include <QAction>
#include "eactcerrar.h"

EResumen::EResumen( QWidget *parent, tipo que )
: EVentana( parent )
{
 setObjectName( "selectorResumen" );
 setWindowTitle( "Seleccione el Resumen" );
 setAttribute( Qt::WA_DeleteOnClose );
 generar = que;
 QVBoxLayout *layoutPrincipal = new QVBoxLayout( this );
 layoutPrincipal->setMargin( 3 );
 layoutPrincipal->setSpacing( 3 );
 switch( que )
 {
  case diario:
  {
   QGroupBox *g = new QGroupBox( this );
   g->setTitle( "Elija la fecha del resumen" );
   g->setAlignment( Qt::AlignHCenter );
   g->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
   QVBoxLayout *v = new QVBoxLayout( g );
   CWfecha = new QCalendarWidget( g );
   CWfecha->setGridVisible( false );
   CWfecha->setMaximumDate( QDate::currentDate() );
   CWfecha->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
   v->addWidget( CWfecha );
   g->adjustSize();
   layoutPrincipal->addWidget( g );
   break;
  }
  case mensual:
  {
    QGroupBox *groupBox = new QGroupBox( this );
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setTitle( "Elija el mes para el resumen" );
    groupBox->setAlignment( Qt::AlignCenter );
    groupBox->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    QVBoxLayout *v = new QVBoxLayout(groupBox);
    v->setObjectName( "vLayout" );
    QHBoxLayout *h1 = new QHBoxLayout();
    h1->setObjectName("hLayout1");

    QLabel *Lmes = new QLabel(groupBox);
    Lmes->setObjectName(QString::fromUtf8("labelMes"));
    Lmes->setText( tr( "Mes:" ) );
    h1->addWidget(Lmes);

    CBMes = new QComboBox(groupBox);
    CBMes->setObjectName(QString::fromUtf8("CBMes"));
    // Inserto los meses
    QLocale locale;
    for( int i = 1; i<=12; i++ )
    {
     CBMes->insertItem( i, locale.monthName( i ) );
    }
    CBMes->setCurrentIndex( QDate::currentDate().month() );
    h1->addWidget(CBMes);


    v->addLayout(h1);

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

    QLabel *Lano = new QLabel(groupBox);
    Lano->setObjectName(QString::fromUtf8("lano"));
    Lano->setText( tr( "Año:" ) );
    horizontalLayout_2->addWidget(Lano);

    SBAno = new QSpinBox(groupBox);
    SBAno->setObjectName(QString::fromUtf8("SBAno"));
    SBAno->setMaximum( QDate::currentDate().year() );
    SBAno->setSingleStep( 1 );
    SBAno->setValue( QDate::currentDate().year() );

    horizontalLayout_2->addWidget(SBAno);

    v->addLayout(horizontalLayout_2);

   layoutPrincipal->addWidget( groupBox );
   break;
  }
  case anual:
  {
   QHBoxLayout *layout = new QHBoxLayout( this );
   QLabel *etiqueta = new QLabel( this );
   etiqueta->setText( QString( "Año:" ) );
   seleccion = new QSpinBox( this );
   seleccion->setRange( 0, QDate::currentDate().year() );
   seleccion->setSingleStep( 1 );
   seleccion->setValue( QDate::currentDate().year() );
   layout->addWidget( etiqueta );
   layout->addWidget( seleccion );
   layoutPrincipal->addLayout( layout );
   break;
  }
  case semanal:
  {
   QGroupBox *g = new QGroupBox( this );
   g->setTitle( "Elija un dia de la semana para el resumen" );
   g->setAlignment( Qt::AlignHCenter );
   g->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
   QVBoxLayout *v = new QVBoxLayout( g );
   CWfecha = new QCalendarWidget( g );
   CWfecha->setGridVisible( true );
   CWfecha->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
   v->addWidget( CWfecha );
   g->adjustSize();
   layoutPrincipal->addWidget( g );
   break;
  }
  case entrefechas:
  {
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName( "horizontalLayout" );
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *LInicio = new QLabel(this);
    LInicio->setObjectName( "LInicio" );
    LInicio->setText( "Fecha de inicio:" );

    horizontalLayout->addWidget(LInicio);

    DEInicio = new QDateEdit(this);
    DEInicio->setObjectName( "DEInicio" );
    DEInicio->setCalendarPopup(true);

    horizontalLayout->addWidget(DEInicio);

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName( "horizontalLayout_2" );
    horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    QLabel *LFin = new QLabel(this);
    LFin->setObjectName( "LFin" );
    LFin->setText( "Fecha de Fin:" );

    horizontalLayout_2->addWidget(LFin);

    DEFin = new QDateEdit(this);
    DEFin->setObjectName( "DEFin" );
    DEFin->setCalendarPopup(true);
    DEFin->setMaximumDate( QDate::currentDate() );

    horizontalLayout_2->addWidget(DEFin);

    layoutPrincipal->addItem( horizontalLayout );
    layoutPrincipal->addItem( horizontalLayout_2 );
  }
 }
 QAction *ActGenerar = new QAction( "Generar Reporte", this );
 ActGenerar->setIcon( QIcon( ":/imagenes/reporte.png" ) );
 connect( ActGenerar, SIGNAL( triggered() ), this, SLOT( generarResumen() ) );
 addAction( ActGenerar );
 EActCerrar *ActCerrar = new EActCerrar( this );
 connect( ActCerrar, SIGNAL( triggered() ), this, SLOT( close() ) );
 addAction( ActCerrar );
 this->adjustSize();
}


EResumen::~EResumen()
{
}




/*!
    \fn EResumen::generarResumen()
	Dependiendo del valor de "tipo" llama a los metodos necesarios para generar el resumen
 */
void EResumen::generarResumen()
{
 QDate primero, ultimo;
 resumenGeneral *resumen = new resumenGeneral( this->parent() );
 switch( generar )
 {
  case diario:
  {
   primero = CWfecha->selectedDate();
   ultimo = CWfecha->selectedDate();
   resumen->setTitulo( QString( "Resumen para el dia %1" ).arg( primero.toString() ) );
   resumen->setDescripcion( QString( "Resumen de entradas, salidas y gastos para el dia %1" ).arg( primero.toString() ) );
   resumen->renderizar( primero, ultimo );
   break;
  }
  case mensual:
  {
   resumen->setTitulo( QString( "Resumen para el mes de %1 del año %2" ).arg( CBMes->currentText() ).arg( QDate::currentDate().year() ) );
   resumen->setDescripcion( QString( "Resumen detallado de entradas,salidas y gastos para el mes de %1 de %2" ).arg( CBMes->currentText() ).arg( QDate::currentDate().year() ) );
   primero.setDate( SBAno->value(), CBMes->currentIndex(), 1 );
   ultimo.setDate( SBAno->value(),  CBMes->currentIndex(), primero.daysInMonth() );
   resumen->renderizar( primero, ultimo );
   break;
  }
  case anual:
  {
   resumen->setTitulo( QString( "Resumen del año %1" ).arg( seleccion->value() ) );
   resumen->setDescripcion( QString( "Resumen detallado de entradas,salidas y gastos para el año %1" ).arg( seleccion->value() ) );
   primero.setDate( seleccion->value(), 1, 1 );
   ultimo.setDate( seleccion->value(), 12, 31 );
   resumen->renderizar( primero, ultimo );
   break;
  }
  case semanal:
  {
   ultimo = CWfecha->selectedDate().addDays( 7 - CWfecha->selectedDate().dayOfWeek() );
   primero.setDate( CWfecha->selectedDate().year(), CWfecha->selectedDate().month(), CWfecha->selectedDate().day() - CWfecha->selectedDate().dayOfWeek() );
   resumen->setTitulo( "Resumen Semanal" );
   resumen->setDescripcion( QString( "Resumen detallado de entradas, salidas y gastos desde %1 al %2" ).arg( primero.toString() ).arg( ultimo.toString() ) );
   resumen->renderizar( primero, ultimo );
   break;
  }
  case entrefechas:
  {
   resumen->setTitulo( "Resumen entre fechas" );
   resumen->setDescripcion( QString( "Resumen entre %1  y %2" ).arg( DEInicio->date().toString() ).arg( DEFin->date().toString() ) );
   resumen->renderizar( DEInicio->date(), DEFin->date() );
   break;
  }
 }
 EVisorInformes *visor = new EVisorInformes( new QPrinter(), this );
 connect( visor, SIGNAL( paintRequested( QPrinter* ) ), resumen, SLOT( imprimir( QPrinter * ) ) );
 agregarVentana( visor );
 close();
}
