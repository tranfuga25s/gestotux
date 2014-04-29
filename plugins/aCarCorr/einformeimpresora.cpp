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
#include "einformeimpresora.h"
#include "preferencias.h"
#include <QPrinterInfo>

EInformeImpresora::EInformeImpresora()
 : QPrinter( QPrinterInfo::defaultPrinter())
{
 preferencias *p = preferencias::getInstancia();
 p->beginGroup( "aCarCorr" );
 p->beginGroup( "informes" );
 // Orientacion
 if( p->value( "orientacion", QPrinter::Portrait ).toInt() == QPrinter::Landscape )
 { this->setOrientation( QPrinter::Landscape ); }
 else
 { this->setOrientation( QPrinter::Portrait ); }
 // Tamaño
 switch( p->value( "tam", -1 ).toInt() )
 {
	case QPrinter::A1:
	{this->setPaperSize( QPrinter::A1 );break;}
	case QPrinter::A2:
	{this->setPaperSize( QPrinter::A2 );break;}
	case QPrinter::A3:
	{this->setPaperSize( QPrinter::A3 );break;}
	case QPrinter::A4:
	{this->setPaperSize( QPrinter::A4 );break;}
	case QPrinter::A5:
	{this->setPaperSize( QPrinter::A5 );break;}
	case QPrinter::A6:
	{this->setPaperSize( QPrinter::A6 );break;}
	case QPrinter::A7:
	{this->setPaperSize( QPrinter::A7 );break;}
	case QPrinter::A8:
	{this->setPaperSize( QPrinter::A8 );break;}
	case QPrinter::A9:
	{this->setPaperSize( QPrinter::A9 );break;}
	case QPrinter::B0:
	{this->setPaperSize( QPrinter::B0 );break;}
	case QPrinter::B1:
	{this->setPaperSize( QPrinter::B1 );break;}
	case QPrinter::B2:
	{this->setPaperSize( QPrinter::B2 );break;}
	case QPrinter::B3:
	{this->setPaperSize( QPrinter::B3 );break;}
	case QPrinter::B4:
	{this->setPaperSize( QPrinter::B4 );break;}
	case QPrinter::B5:
	{this->setPaperSize( QPrinter::B5 );break;}
	case QPrinter::B6:
	{this->setPaperSize( QPrinter::B6 );break;}
	case QPrinter::B7:
	{this->setPaperSize( QPrinter::B7 );break;}
	case QPrinter::B8:
	{this->setPaperSize( QPrinter::B8 );break;}
	case QPrinter::B9:
	{this->setPaperSize( QPrinter::B9 );break;}
	case QPrinter::B10:
	{this->setPaperSize( QPrinter::B10 );break;}
	case QPrinter::C5E:
	{this->setPaperSize( QPrinter::C5E );break;}
	case QPrinter::Comm10E:
	{this->setPaperSize( QPrinter::Comm10E );break;}
	///@todo Terminar de poner los tamaños de hoja que faltan
	default:
	{
		this->setPaperSize( QPrinter::A4 );
		//qWarning( "Usando hoja x default" );
		break;
	}
 }
 // Margenes
 qreal arriba,abajo,derecha,izquierda;
 arriba    = p->value( "margenarriba"   , 15.0 ).toDouble();
 abajo     = p->value( "margenabajo"    , 15.0 ).toDouble();
 derecha   = p->value( "margenderecha"  , 15.0 ).toDouble();
 izquierda = p->value( "margenizquierda", 15.0 ).toDouble();
 this->setPageMargins( izquierda, arriba, derecha, abajo, QPrinter::Millimeter );
 p->endGroup();
 p->endGroup();
}


EInformeImpresora::~EInformeImpresora()
{
}




/*!
    \fn EInformeImpresora::guardar()
 */
void EInformeImpresora::guardar()
{
 preferencias *p = preferencias::getInstancia();
 // Cargar las preferencias a la impresora que las guarda el dialogo mismo
 p->beginGroup( "aCarCorr" );
 p->beginGroup( "informes" );
 // Orientacion
 if( this->orientation() == QPrinter::Portrait )
 {
  p->setValue( "orientacion", QPrinter::Portrait );
 }
 else
 {
  p->setValue( "orientacion", QPrinter::Landscape );
 }
 // Tamaño
 p->setValue( "tam", paperSize() );
 qWarning( QString( "tam: %1" ).arg( paperSize() ).toLocal8Bit() );
 switch( this->pageSize() )
 {
	case QPrinter::A1:
	{p->setValue( "tam", QPrinter::A1 );break;}
	case QPrinter::A2:
	{p->setValue( "tam", QPrinter::A2 );break;}
	case QPrinter::A3:
	{p->setValue( "tam", QPrinter::A3 );break;}
	case QPrinter::A4:
	{p->setValue( "tam", QPrinter::A4 );break;}
	case QPrinter::A5:
	{p->setValue( "tam", QPrinter::A5 );break;}
	case QPrinter::A6:
	{p->setValue( "tam", QPrinter::A6 );break;}
	case QPrinter::A7:
	{p->setValue( "tam", QPrinter::A7 );break;}
	case QPrinter::A8:
	{p->setValue( "tam", QPrinter::A8 );break;}
	case QPrinter::A9:
	{p->setValue( "tam", QPrinter::A9 );break;}
	case QPrinter::B0:
	{p->setValue( "tam", QPrinter::B0 );break;}
	case QPrinter::B1:
	{p->setValue( "tam", QPrinter::B1 );break;}
	case QPrinter::B2:
	{p->setValue( "tam", QPrinter::B2 );break;}
	case QPrinter::B3:
	{p->setValue( "tam", QPrinter::B3 );break;}
	case QPrinter::B4:
	{p->setValue( "tam", QPrinter::B4 );break;}
	case QPrinter::B5:
	{p->setValue( "tam", QPrinter::B5 );break;}
	case QPrinter::B6:
	{p->setValue( "tam", QPrinter::B6 );break;}
	case QPrinter::B7:
	{p->setValue( "tam", QPrinter::B7 );break;}
	case QPrinter::B8:
	{p->setValue( "tam", QPrinter::B8 );break;}
	case QPrinter::B9:
	{p->setValue( "tam", QPrinter::B9 );break;}
	case QPrinter::B10:
	{p->setValue( "tam", QPrinter::B10 );break;}
	case QPrinter::C5E:
	{p->setValue( "tam", QPrinter::C5E );break;}
	case QPrinter::Comm10E:
	{p->setValue( "tam", QPrinter::Comm10E );break;}
	///@todo Terminar de poner los tamaños de hoja que faltan
	default:
	{
		p->setValue( "tam", QPrinter::A4 );
		qWarning( "guardando con el tamaño x default" );
		break;
	}
 }
 qreal arriba, abajo, izquierda, derecha;
 this->getPageMargins( &izquierda, &arriba, &derecha, &abajo, QPrinter::Millimeter );
 p->beginGroup( "margen" );
 p->setValue( "arriba"   , arriba    );
 p->setValue( "abajo"    , abajo     );
 p->setValue( "derecha"  , derecha   );
 p->setValue( "izquierda", izquierda );
 p->endGroup();
 qWarning( "Guardardado" );
 p->endGroup();
 p->endGroup();
}
