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
#include "resumengeneral.h"

#include <QSqlQuery>
#include <QTextBlockFormat>
#include <QDate>
#include <QSqlError>
#include <QSqlRecord>
#include <QTextCursor>
#include <QTextTable>
#include <QDate>
#include <QProgressDialog>
#include <QTextDocument>

resumenGeneral::resumenGeneral(QObject *parent)
 : QObject(parent), noSeguir( false )
{
 doc = new QTextDocument( this );
}

resumenGeneral::~resumenGeneral()
{}

/*!
    \fn resumenGeneral::renderizar( QDate primero, QDate ultimo )
	Genera la salida visible del resumen
 */
void resumenGeneral::renderizar( QDate primero, QDate ultimo )
{
 doc->clear();
 dialogo = new QProgressDialog();
 dialogo->setLabelText( "Iniciando resumen... " );
 dialogo->setAutoClose( true );
 dialogo->setCancelButtonText( "Cancelar" );
 dialogo->setRange( 0, 4 );
 dialogo->setValue( 0 );
 dialogo->show();
 connect( dialogo, SIGNAL( canceled() ), this, SLOT( parar() ) );
 // Parametros necesarios
 this->primero = primero;
 this->ultimo = ultimo;
 this->total = 0;
 ///////////////////////////////////////////////
 // Contenido del resumen
 QTextCursor *cursor = new QTextCursor( doc );
 dialogo->setLabelText( "Formateando resumen... " );
 dialogo->setValue( dialogo->value() + 1 );
 QTextCharFormat fuente;
 fuente.setFontWeight( QFont::Bold );
 fuente.setFontPointSize( 20 );

 QTextBlockFormat cabecera;
 cabecera.setAlignment( Qt::AlignHCenter );

 cursor->insertBlock( cabecera, fuente );
 cursor->insertText( titulo );

 fuente.setFontWeight( QFont::StyleNormal );
 fuente.setFontItalic( true );
 fuente.setFontPointSize( 10 );

 cursor->insertBlock( cabecera, fuente );
 cursor->insertText( descripcion );

 dialogo->setLabelText( "Generando tabla... " );
 dialogo->setValue( dialogo->value() + 1 );
 // Inserto la tabla
// Formato de la tabla
 QTextTableFormat tableFormat;
 QVector<QTextLength> constraints;
 constraints << QTextLength(QTextLength::PercentageLength, 10 );
 constraints << QTextLength(QTextLength::PercentageLength, 75 );
 constraints << QTextLength(QTextLength::PercentageLength, 15 );
 tableFormat.setColumnWidthConstraints( constraints );
 tableFormat.setAlignment( Qt::AlignHCenter );
 tableFormat.setHeaderRowCount( 1 );
 tableFormat.setBorder( 0 );
 tableFormat.setBorderStyle( QTextFrameFormat::BorderStyle_Solid );

 tabla = cursor->insertTable( 1, 3, tableFormat );

 dialogo->setLabelText( "Insertando cabecera... " );
 dialogo->setValue( dialogo->value() + 1 );
 QTextTableCell celda = tabla->cellAt( 0, 0 );

 centrado = celda.format().toBlockFormat();
 centrado.setAlignment( Qt::AlignHCenter );

 derecha = celda.format().toBlockFormat();
 derecha.setAlignment( Qt::AlignRight );

 QTextCharFormat cabeceraTabla;
 cabeceraTabla.setFontItalic( false );
 cabeceraTabla.setFontWeight( QFont::Bold );
 cabeceraTabla.setBackground( Qt::gray );

 celda = tabla->cellAt( 0,0 );

 celda.setFormat( cabeceraTabla );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertText( "Cant" );

 celda = tabla->cellAt( 0, 1 );
 celda.setFormat( cabeceraTabla );
 celda.firstCursorPosition().insertText( "Tipo" );

 celda = tabla->cellAt( 0, 2 );
 celda.setFormat( cabeceraTabla );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertText( "Sub-Total" );

 dialogo->setLabelText( "Iniciando obtencion de datos... " );
 dialogo->setValue( dialogo->value() + 1 );
 QTextCharFormat cabeceraSeccion;
 cabeceraSeccion.setBackground( Qt::lightGray );
 cabeceraSeccion.setFontItalic( true );
 cabeceraSeccion.setFontWeight( QFont::Bold );

 //////////////////////////
 // Peluqueria
 ////////////////////////////////////////////////////////////////////////////
 if( noSeguir )
 {
  return;
 }
 this->peluqueria();
 ////////////////////////////////////////////////////////////////////////////
 // Resumen de Ventas
 if( noSeguir )
 {
  return;
 }
 this->ventas();
 ////////////////////////////////////////////////////////////////////////////
 // Resumen de Compras
  if( noSeguir )
 {
  return;
 }
 this->compras();
  if( noSeguir )
 {
  return;
 }
 this->gastos();
 QTextCharFormat formatoTotal;
 formatoTotal.setFontWeight( QFont::Bold );
 // Inserto el total general
 tabla->insertRows( -1, 1 );
 tabla->mergeCells( tabla->rows()-1, 0, 1, 2 );
 tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( derecha );
 tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Total Final:", formatoTotal );
 tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
 tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1 ").arg( total ), formatoTotal );
 if( total < 0 )
 {
  QTextCharFormat formato = tabla->cellAt( tabla->rows()-1, 2 ).format();
  formato.setBackground( Qt::red );
  tabla->cellAt( tabla->rows()-1, 2 ).setFormat( formato );
 }
 dialogo->close();
 return;
}




/*!
    \fn resumenGeneral::peluqueria()
 */
void resumenGeneral::peluqueria()
{
 QSqlQuery cola;
 cola.prepare( "SELECT COUNT(id) FROM peluqueria WHERE " + hacerWhere() );
 if( cola.exec() )
 {
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   if( cantidad.toInt() > 0 )
   {
         dialogo->setLabelText( "Peluqueria... " );
         dialogo->setRange( 0, 1 );
         dialogo->setValue( 0 );
          if( noSeguir )
          {
            return;
          }
	// Busco la cantidad en precio
	cola.prepare( "SELECT SUM(precio) FROM peluqueria WHERE " + hacerWhere() );
	if( cola.exec() )
	{
		if( cola.next() )
		{
			tabla->insertRows( -1, 1 );

			tabla->mergeCells( tabla->rows()-1, 0, 1, 3 );
			QTextTableCell celda = tabla->cellAt( tabla->rows()-1,0 );
			celda.firstCursorPosition().setBlockFormat( centrado );
			celda.setFormat( cabeceraSeccion );
			celda.firstCursorPosition().insertText( "PELUQUERIA CANINA" );

                        tabla->insertRows( -1, 1 );
			celda = tabla->cellAt( tabla->rows()-1, 0 );
			celda.firstCursorPosition().setBlockFormat( centrado );
			celda.firstCursorPosition().insertText( QString( "%1" ).arg( cantidad.toInt() ) );

			celda = tabla->cellAt( tabla->rows()-1, 1 );
			celda.firstCursorPosition().insertText( "Servicios Prestados" );

			celda = tabla->cellAt( tabla->rows()-1, 2 );
			celda.firstCursorPosition().setBlockFormat( centrado );
			celda.firstCursorPosition().insertText( QString( "$ %L1" ).arg( cola.record().value(0).toDouble() ) );

			total += cola.record().value(0).toDouble();
			dialogo->setValue( 1 );
		}
 	}
   }
  }
 }
 else
 {
  qDebug( "Error al ejecutar la cola de cantidad de servicios" );
  qDebug( cola.lastError().text().toLocal8Bit() );
  qDebug( cola.lastQuery().toLocal8Bit() );
 }
}


/*!
    \fn resumenGeneral::gastos()
 */
void resumenGeneral::gastos()
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Seccion de Gastos
 QSqlQuery cola;
 cola.prepare( "SELECT COUNT(id) FROM gastos WHERE " + hacerWhere() );
 if( cola.exec() )
 {
  qDebug( qPrintable( cola.lastQuery() ) );
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   qDebug( cola.lastError().text().toLocal8Bit() );
   qDebug( cola.lastQuery().toLocal8Bit() );
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   if( cantidad.toInt() > 0 )
   {
    if( noSeguir )
    {
      return;
    }
        dialogo->setLabelText( "Gastos - Sumando..." );
        dialogo->setRange( 0, cantidad.toInt()+1 );
        dialogo->setValue( 0 );
	// Busco la cantidad en precio
	cola.prepare( "SELECT SUM( costo ) FROM gastos WHERE " + hacerWhere() );
	if( cola.exec() )
	{
		if( cola.next() )
		{
		      if( noSeguir )
		      {
		        return;
		      }
			total -= cola.record().value(0).toDouble();
			QVariant subtotalgastos = cola.record().value(0);

			tabla->insertRows( -1, 1 );
			tabla->mergeCells( tabla->rows()-1, 0, 1, 3 );

			QTextTableCell celda = tabla->cellAt( tabla->rows()-1,0 );
			celda.firstCursorPosition().setBlockFormat( centrado );
			celda.firstCursorPosition().insertText( "GASTOS", cabeceraSeccion );

                        dialogo->setLabelText( "Gastos - Generando contenidos... " );
                        // Listado de gastos
                        QSqlQuery cola1;
                        if( cola1.exec( "SELECT descripcion, COUNT( id ), SUM( costo ) FROM gastos WHERE " + hacerWhere() + " GROUP BY descripcion"  ) )
                        {
                          while( cola1.next() )
                          {
                           if( noSeguir ) {return;}
                              dialogo->setValue( dialogo->value() + 1 );
                             tabla->insertRows( -1, 1 );
                             tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( centrado );
                             tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( QString( "%L1" ).arg( cola1.record().value(1).toInt() ) );
                             tabla->cellAt( tabla->rows()-1, 1 ).firstCursorPosition().insertText( cola1.record().value(0).toString() );
                             tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
                             tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1" ).arg( cola1.record().value(2).toDouble() ) );
                          }

                        }
                        else
                        {
                          qDebug( "Error al ejecutar cola1 - gastos" );
                          qDebug( cola1.lastError().text().toLocal8Bit() );
                          qDebug( cola1.lastQuery().toLocal8Bit() );
                        }
			// Pongo el totales
			dialogo->setLabelText( "Gastos - Seteando subtotal... " );
			tabla->insertRows( -1, 1 );
			tabla->mergeCells( tabla->rows()-1, 0, 1, 2 );
			tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( derecha );
			tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Total de gastos:" );
			tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
			tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1" ).arg( subtotalgastos.toDouble() ) );
			dialogo->setValue( dialogo->value() + 1 );
		}
 	}
	else
	{
	 qDebug( "Error de exec cola sum costo gastos" );
         qDebug( cola.lastError().text().toLocal8Bit() );
         qDebug( cola.lastQuery().toLocal8Bit() );
	}
   }
  }
 }
 else
 {
  qDebug( "Error al intentar ejecutar cola de cantidad de gastos " );
 }
 return;
}


/*!
    \fn resumenGeneral::compras()
 */
void resumenGeneral::compras()
{
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Seccion de Gastos
 QSqlQuery cola;
 cola.prepare( "SELECT COUNT(id) FROM compras WHERE " + hacerWhere() );
 if( cola.exec() )
 {
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   qDebug( cola.lastError().text().toLocal8Bit() );
   qDebug( cola.lastQuery().toLocal8Bit() );
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   if( cantidad.toInt() > 0 )
   {
        if( noSeguir ) {return;}
	// Busco la cantidad en precio
	dialogo->setLabelText( "Compras - Sumando... " );
	dialogo->setRange( 0, cantidad.toInt()+1 );
	dialogo->setValue( 0 );
	cola.prepare( "SELECT SUM( costo ) FROM compras WHERE " + hacerWhere() );
	if( cola.exec() )
	{
		if( cola.next() )
		{
		        if( noSeguir ) {return;}
			total -= cola.record().value(0).toDouble();
			QVariant subtotalcompras = cola.record().value(0);

			tabla->insertRows( -1, 1 );
			tabla->mergeCells( tabla->rows()-1, 0, 1, 3 );

			QTextTableCell celda = tabla->cellAt( tabla->rows()-1,0 );
			celda.firstCursorPosition().setBlockFormat( centrado );
			celda.firstCursorPosition().insertText( "COMPRAS", cabeceraSeccion );

                        // Listado de gastos
                        dialogo->setLabelText( "Compras - Generando listado... " );
                        QSqlQuery cola1; // si los quiere agrupados: GROUP BY c.id_proveedor;
                        if( cola1.exec( "SELECT 1, p.nombre, c.costo FROM compras c, proveedor p WHERE c.id_proveedor = p.id AND  " + hacerWhere() ) )
                        {
                          while( cola1.next() )
                          {
                             if( noSeguir ) {return;}
                             dialogo->setValue( dialogo->value() + 1 );
                             tabla->insertRows( -1, 1 );
                             tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( centrado );
                             tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( QString( "%L1" ).arg( cola1.record().value(0).toInt() ) );
                             tabla->cellAt( tabla->rows()-1, 1 ).firstCursorPosition().insertText( cola1.record().value(1).toString() );
                             tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
                             tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1" ).arg( cola1.record().value(2).toDouble() ) );
                          }

                        }
                        else
                        {
                          qDebug( "Error al ejecutar cola1 - compas" );
                          qDebug( cola1.lastError().text().toLocal8Bit() );
                          qDebug( cola1.lastQuery().toLocal8Bit() );
                        }
			// Pongo el totales
			dialogo->setLabelText( "Compras - Seteando sub-total... " );
			tabla->insertRows( -1, 1 );
			tabla->mergeCells( tabla->rows()-1, 0, 1, 2 );
			tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( derecha );
			tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Total de compras:" );
			tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
			tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1" ).arg( subtotalcompras.toDouble() ) );
			dialogo->setValue( dialogo->value() + 1 );
		}
 	}
	else
	{
	 qDebug( "Error de exec cola sum costo compras" );
         qDebug( cola.lastError().text().toLocal8Bit() );
         qDebug( cola.lastQuery().toLocal8Bit() );
	}
   }
  }
 }
 else
 {
  qDebug( "Error al intentar ejecutar cola de cantidad de compras " );
 }
 return;
}


/*!
    \fn resumenGeneral::ventas()
 */
void resumenGeneral::ventas()
{
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Seccion de Gastos
 QSqlQuery cola;
 cola.prepare( "SELECT COUNT(id) FROM ventas WHERE " + hacerWhere() );
 if( cola.exec() )
 {
  if( !cola.next() )
  {
   qDebug( "error en  next1()" );
   qDebug( cola.lastError().text().toLocal8Bit() );
   qDebug( cola.lastQuery().toLocal8Bit() );
   return;
  }
  else
  {
   QVariant cantidad = cola.record().value( 0 );
   if( cantidad.toInt() > 0 )
   {
        dialogo->setLabelText( "Ventas - Sumando... " );
        dialogo->setRange( 0, cantidad.toInt() +1 );
        dialogo->setValue( 0 );
        if( noSeguir ) {return;}
	// Busco la cantidad en precio
	cola.prepare( QString( "SELECT SUM( precio*cantidad ) FROM ventas_productos WHERE id_venta IN ( SELECT id FROM ventas WHERE " + hacerWhere() + " )" ) );
	if( cola.exec() )
	{
		if( cola.next() )
		{
		        if( noSeguir ) {return;}
			total += cola.record().value(0).toDouble();
			QVariant subtotalventas = cola.record().value(0);

			tabla->insertRows( -1, 1 );
			tabla->mergeCells( tabla->rows()-1, 0, 1, 3 );

			QTextTableCell celda = tabla->cellAt( tabla->rows()-1,0 );
			celda.firstCursorPosition().setBlockFormat( centrado );
			celda.firstCursorPosition().insertText( "VENTAS", cabeceraSeccion );

                        // Listado de ventas
                        dialogo->setLabelText( "Ventas - Generando listado... " );
                        QSqlQuery cola1;
                        if( cola1.exec( "SELECT SUM( vc.cantidad ), p.nombre, SUM( vc.cantidad*vc.precio ) FROM ventas_productos vc, producto p WHERE vc.id_producto = p.id AND vc.id_venta IN ( SELECT id FROM ventas WHERE " + hacerWhere() + " ) GROUP BY vc.id_producto; " ) )
                        {
                          while( cola1.next() )
                          {
                             if( noSeguir ) {return;}
                             dialogo->setValue( dialogo->value() +1 );
                             tabla->insertRows( -1, 1 );
                             tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( centrado );
                             tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( QString( "%L1" ).arg( cola1.record().value(0).toInt() ) );
                             tabla->cellAt( tabla->rows()-1, 1 ).firstCursorPosition().insertText( cola1.record().value(1).toString() );
                             tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
                             tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1" ).arg( cola1.record().value(2).toDouble() ) );
                          }

                        }
                        else
                        {
                          qDebug( "Error al ejecutar cola1 - compas" );
                          qDebug( cola1.lastError().text().toLocal8Bit() );
                          qDebug( cola1.lastQuery().toLocal8Bit() );
                        }
			// Pongo el totales
			dialogo->setLabelText( "Ventas - Setenado subtotal.. " );
			tabla->insertRows( -1, 1 );
			tabla->mergeCells( tabla->rows()-1, 0, 1, 2 );
			tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().setBlockFormat( derecha );
			tabla->cellAt( tabla->rows()-1, 0 ).firstCursorPosition().insertText( "Total de ventas:" );
			tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().setBlockFormat( centrado );
			tabla->cellAt( tabla->rows()-1, 2 ).firstCursorPosition().insertText( QString( "$ %L1" ).arg( subtotalventas.toDouble() ) );
			dialogo->setValue( dialogo->value() + 1 );
		}
 	}
	else
	{
	 qDebug( "Error de exec cola sum costo compras" );
         qDebug( cola.lastError().text().toLocal8Bit() );
         qDebug( cola.lastQuery().toLocal8Bit() );
	}
   }
   else
   {
    qDebug( "No existen ventas" );
   }
  }
 }
 else
 {
  qDebug( "Error al intentar ejecutar cola de cantidad de compras " );
 }
 return;
}


/*!
    \fn resumenGeneral::parar()
 */
void resumenGeneral::parar()
{
 noSeguir = true;
}


/*!
    \fn resumenGeneral::imprimir( QPrinter* impresora )
 */
void resumenGeneral::imprimir( QPrinter* impresora )
{
 doc->print( impresora );
}


/*!
    \fn resumenGeneral::hacerWhere()
 */
QString resumenGeneral::hacerWhere()
{
 if( QSqlDatabase::database().driverName() == "QSQLITE" )
 {
  return QString( "fecha BETWEEN '%1' AND '%2' " ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( ultimo.toString( "yyyy-MM-dd" ) );
 }
 else if( QSqlDatabase::database().driverName() == "QMYSQL" )
 {
  return QString( "fecha BETWEEN CAST( '%1' AS DATE ) AND CAST( '%2' AS DATE ) " ).arg( primero.toString( "yyyy-MM-dd" ) ).arg( ultimo.toString( "yyyy-MM-dd" ) );
 }
}
