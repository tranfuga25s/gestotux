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

#include "informecierrecaja.h"

#include "mmovimientoscaja.h"
#include "eregistroplugins.h"
#include "einfoprogramainterface.h"
#include "mcajas.h"
#include "preferencias.h"

#include <QPrinter>
#include <QDomDocument>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextTableCell>
#include <QTextTableCellFormat>
#include <QTextCursor>
#include <QSqlRecord>
#include <QDateTime>
#include <QSqlQuery>
#include <QFile>

InformeCierreCaja::InformeCierreCaja(QObject *parent) :
    QObject(parent)
{
    documento = new QTextDocument();
    // Cargo el estilo
    QFile *archivoEstilo = new QFile( ":/estilo.css" );
    if( !archivoEstilo->open( QIODevice::ReadOnly ) )
    {
      qDebug(  "Error al abrir el archivo de estilo: :/estilo.css" );
      _estilo = "";
    } else {
      _estilo = archivoEstilo->readAll();
    }
    archivoEstilo->close();
    delete( archivoEstilo );
    archivoEstilo = 0;
}

void InformeCierreCaja::imprimir( QPrinter *impresora )
{
    QTextDocument _d;
    _d.setDefaultStyleSheet( _estilo );
    _d.setHtml( documento->toHtml() );
    _d.print( impresora );
}

void InformeCierreCaja::hacerResumen( int id_caja, bool ultimo, int id_cierre )
{
    if( id_caja == -1 ) {
        qWarning( "Numero de caja incorrecto" );
        //abort();
        return;
    } else if( ultimo == false && id_cierre == -1 ) {
        qWarning( "El cierre pedido es incorrecto" );
        //abort();
        return;
    }
    // Busco los datos
    MMovimientosCaja *m = new MMovimientosCaja( this );
    if( ultimo ) {
      id_cierre = m->buscarUltimoCierre( id_caja );
      if( id_cierre == -1 ) {
          return;
      }
    }
    QSqlQuery resultados = m->buscarMovimientos( id_caja, id_cierre );
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Inicio el renderizado
    QTextCursor cursor( documento );
    int cantidadCol = 6;
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Caja" );
    bool responsable = p->value( "responsable" ).toBool();
    bool firma = p->value( "firma", true ).toBool();
    bool logo = p->value( "logo" ).toBool();
    p->endGroup();p->endGroup();delete p; p = 0;
    if( responsable ) { cantidadCol++; }
    /////////////////////////////////////
    /// Hago la cabecera de la tabla
    QTextTable *tabla = cursor.insertTable( 1, cantidadCol );
    QTextTableFormat formatoTabla = tabla->format();
    formatoTabla.setHeaderRowCount( 1 );
    formatoTabla.setWidth( QTextLength( QTextLength::PercentageLength, 100 ) );
    tabla->setFormat( formatoTabla );
    tabla->cellAt( 0,0 ).firstCursorPosition().insertHtml( " # Op " );
    tabla->cellAt( 0,1 ).firstCursorPosition().insertHtml( " Fecha/Hora " );
    tabla->cellAt( 0,2 ).firstCursorPosition().insertHtml( " Razon " );
    tabla->cellAt( 0,3 ).firstCursorPosition().insertHtml( " Ingreso " );
    tabla->cellAt( 0,4 ).firstCursorPosition().insertHtml( " Egreso " );
    tabla->cellAt( 0,5 ).firstCursorPosition().insertHtml( " Saldo " );
    if( responsable ) {
        tabla->cellAt( 0, 6 ).firstCursorPosition().insertHtml( " Responsable " );
    }
    QTextBlockFormat bfizq = tabla->cellAt( 0, 5 ).firstCursorPosition().blockFormat();
    bfizq.setAlignment( Qt::AlignRight );
    // Averiguo el saldo hasta el momento del cierre anterior
    double saldo_anterior = m->saldoEnMovimientoAnteriorA( id_caja, id_cierre );
    while( resultados.next() ) {
        int pos = tabla->rows();
        tabla->insertRows( pos, 1 );
        tabla->cellAt( pos, 0 ).firstCursorPosition().insertHtml( QString( "# %1 " ).arg( resultados.record().value("id_movimiento" ).toInt(), 4 ) );
        tabla->cellAt( pos, 1 ).firstCursorPosition().insertHtml( resultados.record().value("fecha_hora" ).toDateTime().toString( Qt::SystemLocaleDate ) );
        tabla->cellAt( pos, 2 ).firstCursorPosition().insertHtml( resultados.record().value("razon" ).toString() );
        if( resultados.record().value( "cierre" ).toBool() == false ) {
            // Ingreso
            double haber = resultados.record().value( "ingreso" ).toDouble();
            saldo_anterior += haber;
            if( haber != 0 ) {
                tabla->cellAt( pos, 3 ).firstCursorPosition().insertHtml( QString( " $ %L1" ).arg( haber, 10, 'f', 2  ) );
            } else {
                tabla->cellAt( pos, 3 ).firstCursorPosition().insertHtml( "" );
            }
            tabla->cellAt( pos, 3 ).firstCursorPosition().setBlockFormat( bfizq );
            // Egreso
            double debe = resultados.record().value( "egreso" ).toDouble();
            saldo_anterior -= debe;
            if( debe != 0 ) {
                tabla->cellAt( pos, 4 ).firstCursorPosition().insertHtml( QString( " $ %L1" ).arg( debe, 10, 'f', 2  ) );
            } else {
                tabla->cellAt( pos, 4 ).firstCursorPosition().insertHtml( "" );
            }
            tabla->cellAt( pos, 4 ).firstCursorPosition().setBlockFormat( bfizq );
            // Subtotal hasta el momento
            tabla->cellAt( pos, 5 ).firstCursorPosition().insertHtml( QString( " $ %L1" ).arg( saldo_anterior, 10, 'f', 2  ) );
            tabla->cellAt( pos, 5 ).firstCursorPosition().setBlockFormat( bfizq );
        } else {
            //saldo_anterior += resultados.record().value( "ingreso" ).toDouble();
            tabla->cellAt( pos, 5 ).firstCursorPosition().insertHtml( QString( " $ %L1" ).arg( saldo_anterior, 10, 'f', 2 ) );
            tabla->cellAt( pos, 5 ).firstCursorPosition().setBlockFormat( bfizq );
        }
        if( responsable ) {
            tabla->cellAt( pos, 6 ).firstCursorPosition().insertHtml( resultados.record().value( "responsable" ).toString() );
        }
    }
    // Saldos finales
    cursor.movePosition( QTextCursor::End );
    cursor.insertBlock();
    cursor.insertHtml( QString( "<b>Saldo Final:</b>   $  %L1" ).arg( saldo_anterior ) );
    cursor.insertBlock();
    if( firma ) {
        cursor.insertBlock();
        cursor.insertText( "Controlado por: ________________________" );
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertText( "Firma: ____________" );
    }
    // Termino el resumen
    cursor.movePosition( QTextCursor::Start );
    cursor.insertBlock();
    if( logo ) {
        //cursor.insertImage( ERegistroPlugins::getInstancia()->pluginInfo()->imagenPrograma() );
        //cursor.insertImage( ":/imagenes/gestotux32.png" );
    }
    cursor.insertHtml( "<h1>Cierre de Caja</h1>" );
    cursor.insertBlock();
    cursor.insertHtml( QString( "<b>Fecha de Cierre:</b> %1 <br />" ).arg( QDateTime::currentDateTime().toString( Qt::SystemLocaleLongDate ) ) );
    cursor.insertHtml( QString( "<b>Caja:</b> %1<br />").arg( MCajas::nombreCaja( id_caja ) ) );
    return;
}
