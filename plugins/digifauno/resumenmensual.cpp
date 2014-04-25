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
#include "resumenmensual.h"
#include "visorresumenes.h"
#include <QTextCursor>
#include <QLocale>
#include <QDate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QTextTable>


resumenMensual::resumenMensual( QObject *parent, int mes )
 : QObject( parent )
{
 this->mes = mes;
}


resumenMensual::~resumenMensual()
{
}



void resumenMensual::setvisorResumen( VisorResumenes* theValue )
{
	form = theValue;
}


VisorResumenes* resumenMensual::visorResumen() const
{
	return form;
}


/*!
    \fn resumenMensual::renderizar()
	Genera la salida visible del resumen
 */
void resumenMensual::renderizar()
{
 QLocale locale;
 form->setTitulo( tr( QString( "Resumen para el mes de %1" ).arg( locale.monthName( mes ) ).toLocal8Bit() ) );
 form->obtenerContenido()->clear();
 QTextCursor *cursor = new QTextCursor( form->obtenerContenido() );
 // Formato de la tabla
 QTextTableFormat tableFormat;
 QVector<QTextLength> constraints;
 constraints << QTextLength(QTextLength::PercentageLength, 60);
 constraints << QTextLength(QTextLength::PercentageLength, 18);
 constraints << QTextLength(QTextLength::PercentageLength, 15);
 tableFormat.setColumnWidthConstraints( constraints );
 tableFormat.setAlignment( Qt::AlignHCenter );
 tableFormat.setHeaderRowCount( 1 );
 tableFormat.setBorder( 0.5 );
 tableFormat.setBorderStyle( QTextFrameFormat::BorderStyle_Solid );

 QTextTable *tabla  = cursor->insertTable( 1, 3, tableFormat );

 // Formato de la celda
 QTextCharFormat formato;
 formato.setFontWeight( QFont::Bold );
 formato.setBackground( Qt::lightGray );

 QTextTableCell celda = tabla->cellAt( tabla->rows()-1, 0 );
 celda.setFormat(formato);
 celda.firstCursorPosition().blockFormat().setAlignment( Qt::AlignHCenter );
 celda.firstCursorPosition().insertText( "Tipo" );

 celda = tabla->cellAt( tabla->rows()-1, 1);
 celda.setFormat( formato );
 celda.firstCursorPosition().insertText( "Cantidad" );


 celda = tabla->cellAt( tabla->rows()-1, 2 );
 celda.setFormat( formato );
 celda.firstCursorPosition().insertText( tr( "Sub-Total" ) );
 celda.firstCursorPosition().blockFormat().setAlignment( Qt::AlignHCenter );
 // Lista la cabecera
 //////////////////////////
 // Servicios
 tabla->insertRows( -1, 1 );
 tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Servicios de Peluqueria" );
 tabla->cellAt( tabla->rows()-1, 1 ).firstCursorPosition().insertText( valores["servicios_cantidad"].toString() );
 tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( valores["servicios_subtotal"].toString().prepend( "$" ) );
 //////////////////////////
 // Ventas
 tabla->insertRows( -1, 1 );
 tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Ventas" );
tabla->cellAt( tabla->rows()-1, 1 ).firstCursorPosition().insertText( valores["ventas_cantidad"].toString() );
 tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( valores["ventas_subtotal"].toString().prepend( "$" ) );
 //////////////////////////
 // Compras
 tabla->insertRows( -1, 1 );
 tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Compras" );
tabla->cellAt( tabla->rows()-1, 1 ).firstCursorPosition().insertText( valores["compras_cantidad"].toString() );
 tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( valores["compras_subtotal"].toString().prepend( "$" ) );
 //////////////////////////
 // Total
 tabla->insertRows( -1, 1 );
 tabla->mergeCells( tabla->rows()-1, 0, 1, 2 );
 celda = tabla->cellAt( tabla->rows()-1, 0 );
 QTextCursor cur = celda.firstCursorPosition();
 cur.blockFormat().setAlignment( Qt::AlignRight );
 cur.insertHtml( "Total:" );

 celda = tabla->cellAt( tabla->rows()-1, 2 );
 celda.setFormat( formato );
 if( valores["total"].toDouble() < 0 )
 {
  ///@todo Poner en color rojo si queda en negativo
 }
 celda.firstCursorPosition().insertText( valores["total"].toString().prepend( "$" ) , formato );

}

/*!
    \fn resumenMensual::generar( bool detallado )
	Compila todos los datos necesarios para generar el resumen
 */
void resumenMensual::generar( bool detallado )
{
 qDebug( "--------> Iniciando generacion" );
 QDate primero,treinta;
 primero.setDate( QDate::currentDate().year(), mes, 1 );
 treinta.setDate( QDate::currentDate().year(), mes, primero.daysInMonth() );
double total = 0;
 QSqlQuery cola;
 ////////////////////////////////////////////////////////////////////////////
 // Resumen de Servicios
 cola.prepare( QString( "SELECT COUNT(id) FROM peluqueria WHERE fecha BETWEEN  '%1' AND '%2' " ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( treinta.toString( "yyyy-MM-dd" ) ) );
 if( cola.exec() )
 {
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   ///@todo Ver como mejorar el manejo de errores
   ///@todo agregar dialogo de error
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   valores["servicios_cantidad"] = cantidad;
   qDebug( QString( "Cantidad de servicios: %1" ).arg( cantidad.toString() ).toLocal8Bit() );
   if( cantidad.toInt() > 0 )
   {
	// Busco la cantidad en precio
	cola.prepare( QString( "SELECT SUM(precio) FROM peluqueria WHERE fecha BETWEEN '%1' AND '%2'" ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( treinta.toString( "yyyy-MM-dd" ) ) );
	if( cola.exec() )
	{
		if( cola.next() )
		{
			valores["servicios_subtotal"] = cola.record().value(0);
			qDebug( QString( "Sub-total Servicios: %1" ).arg( cola.record().value(0).toDouble() ).toLocal8Bit() );
			total += cola.record().value(0).toDouble();
			qDebug( QString( "Total parcial: %1" ).arg( total ).toLocal8Bit() );
		}
 	}
	else
	{
		qDebug( "Error al ejecutar la cola" );
	}
   }
   else
   {
    valores["servicios_subtotal"] = QVariant::fromValue( 0 );
   }
  }
 }
 else
 {
  qDebug( "Error al ejecutar la cola de cantidad de servicios" );
  qDebug( cola.lastError().text().toLocal8Bit() );
  qDebug( cola.lastQuery().toLocal8Bit() );
 }
 ////////////////////////////////////////////////////////////////////////////
 // Resumen de Ventas
 cola.prepare( QString( "SELECT COUNT(id) FROM ventas WHERE fecha BETWEEN '%1' AND '%2' " ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( treinta.toString( "yyyy-MM-dd" ) ) );
 if( cola.exec() )
 {
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   qDebug( cola.lastError().text().toLocal8Bit() );
   qDebug( cola.lastQuery().toLocal8Bit() );
   ///@todo agregar dialogo de error
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   valores["ventas_cantidad"] = cantidad;
   qDebug( QString( "Cantidad de ventas: %1" ).arg( cantidad.toInt() ).toLocal8Bit() );
   if( cantidad.toInt() > 0 )
   {
	// Busco la cantidad en precio
	cola.prepare( QString( "SELECT SUM( precio * cantidad ) FROM ventas_productos WHERE id_venta IN ( SELECT id FROM ventas WHERE fecha BETWEEN '%1' AND '%2'  )" ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( treinta.toString( "yyyy-MM-dd" ) ) );
	if( cola.exec() )
	{
		if( cola.next() )
		{
			valores["ventas_subtotal"] = cola.record().value(0);
			qDebug( QString( "Sub-total Ventas: %1" ).arg( cola.record().value(0).toInt() ).toLocal8Bit() );
			total += cola.record().value(0).toDouble();
			qDebug( QString( "Total parcial: %1" ).arg( total ).toLocal8Bit() );
		}
 	}
	else
	{
	 qDebug( "Error de exec cola sum precio venta-productods" );
         qDebug( cola.lastError().text().toLocal8Bit() );
         qDebug( cola.lastQuery().toLocal8Bit() );
	}
   }
   else
   {
    valores["ventas_subtotal"] = QVariant::fromValue( 0 );
   }
  }
 }
 else
 {
  qDebug( "Error al ejecutar la cola de cantidad de ventas" );
  qDebug( cola.lastError().text().toLocal8Bit() );
  qDebug( cola.lastQuery().toLocal8Bit() );
 }
 ////////////////////////////////////////////////////////////////////////////
 // Resumen de Compras
 cola.prepare( QString( "SELECT COUNT(id) FROM compras WHERE fecha BETWEEN '%1' AND '%2'" ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( treinta.toString( "yyyy-MM-dd" ) ) );
 if( cola.exec() )
 {
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   qDebug( cola.lastError().text().toLocal8Bit() );
   ///@todo agregar dialogo de error
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   valores["compras_cantidad"] = cantidad;
   qDebug( QString( "Cantidad de compras: %1" ).arg( cantidad.toString() ).toLocal8Bit() );
   if( cantidad.toInt() > 0 )
   {
	// Busco la cantidad en precio
	cola.prepare( QString( "SELECT SUM( precio_compra * cantidad ) FROM compras_productos WHERE id_compra IN ( SELECT id FROM compras WHERE fecha BETWEEN '%1' AND '%2' )" ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( treinta.toString( "yyyy-MM-dd" ) ) );
	if( cola.exec() )
	{
		if( cola.next() )
		{
  			valores["compras_subtotal"] = cola.record().value(0);
			qDebug( QString( "Sub-total Compras: %1" ).arg( cola.record().value(0).toString() ).toLocal8Bit() );
			total -= cola.record().value(0).toDouble();
			qDebug( QString( "Total parcial: %1" ).arg( total ).toLocal8Bit() );
		}
 	}
	else
	{
	 qDebug( "Error de exec cola sum precio_compra compra-productods" );
	}
   }
   else
   {
	valores["compras_subtotal"] = QVariant::fromValue( 0 );
   }
  }
 }
 else
 {
  qDebug( "Error al ejecutar la cola de cantidad de compras" );
  qDebug( cola.lastError().text().toLocal8Bit() );
  qDebug( cola.lastQuery().toLocal8Bit() );
 }
 valores["total"] = QVariant::fromValue( total );
 qDebug( QString( "Total Final: %1" ).arg( total ).toLocal8Bit() );
 renderizar();
}
