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
#include "visorresumen.h"

#include <QTextTable>
#include <QDate>
#include <QAction>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QDir>

visorResumen::visorResumen(QWidget *parent)
 : QTextEdit(parent)
{
 this->setAttribute( Qt::WA_DeleteOnClose );
 setReadOnly( true );
 destinatario = "";
 contenido = "";
 total = 0;
 fecha = QDate::currentDate();

 // Agrego las acciones
 QAction *ActCerrar = new QAction( "Cerrar", this );
 ActCerrar->setIcon( QIcon( ":/imagenes/fileclose.png" ) );
 ActCerrar->setShortcut( QKeySequence( "Ctrl+c" ) );
 ActCerrar->setStatusTip( "Cierra el presupuesto actual" );
 connect( ActCerrar, SIGNAL( triggered() ), this, SLOT( close() ) );

 QAction *ActImprimir = new QAction( "Imprimir", this );
 ActImprimir->setIcon( QIcon( ":/imagenes/impresora.png" ) );
 ActImprimir->setStatusTip( "Imprime el presupuesto actual" );
 connect( ActImprimir, SIGNAL( triggered() ), this, SLOT( imprimir() ) );

 QAction *ActPdf = new QAction( "Guardar a Pdf", this );
 ActPdf->setIcon( QIcon( ":/imagenes/acroread.png" ) );
 ActPdf->setStatusTip( "Guarda el resumen actual en un archivo pdf" );
 connect( ActPdf, SIGNAL( triggered() ), this, SLOT( aPdf() ) );
 
 addAction( ActImprimir );
 addAction( ActPdf );
 addAction( ActCerrar );
}


visorResumen::~visorResumen()
{
}




/*!
    \fn visorResumen::renderizarCabecera()
 */
void visorResumen::renderizarCabecera()
{
 //Cabecera de presupuesto
 cursor = new QTextCursor( document()->begin() );

 QTextTableFormat formato;
 QVector<QTextLength> constraints;
 constraints << QTextLength(QTextLength::PercentageLength, 20 );
 constraints << QTextLength(QTextLength::PercentageLength, 80 );
 formato.setColumnWidthConstraints( constraints );
 formato.setBorder( 0 );
 QTextTable *tabla = cursor->insertTable( 2, 2, formato );

 tabla->mergeCells( 0, 0, 2, 1 );

 QTextTableCell celda = tabla->cellAt( 0,0 );

  QTextBlockFormat centrado = celda.format().toBlockFormat();
  centrado.setAlignment( Qt::AlignHCenter );

 QTextImageFormat imageFormat;
 imageFormat.setName(":/imagenes/logo.png");
 imageFormat.setHeight( 50 );
 imageFormat.setWidth( 80 );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertImage( imageFormat );

 f = cursor->charFormat();
 f.setFontFamily( "BankGothic Lt BT" );
 f.setFontUnderline( true );
 f.setFontPointSize( 22 );
 f.setFontWeight( QFont::Bold );

 celda = tabla->cellAt( 0, 1 );
 celda.firstCursorPosition().setBlockCharFormat( f );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertText( "mmmm  Fontanessi" );

 celda = tabla->cellAt( 1, 1 );

 f.setFontUnderline( false );
 f.setFontPointSize( 8 );
 celda.firstCursorPosition().setCharFormat( f );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertText( "- SERVICIO TÉCNICO ELECTROMECANICO - MECANICA GENERAL -" );

 cursor->movePosition( QTextCursor::End );
 fb = cursor->blockFormat();
 fb.setAlignment( Qt::AlignHCenter );
 cursor->setCharFormat( f );
 cursor->setBlockFormat( fb );
 cursor->insertText( "INYECIÓN ELECTRÓNICA ( NAFTA-DIESEL )  - AIRE ACONDICIONADO - CALEFACIÓN  - INSTRUMENTAL - LIMPIAPARABRISAS - ARRANQUES - ALTERNADORES - REPARACIÓN DE TABLEROS Y FUSILERAS - CIERRES CENTRALIZADOS - ALARMAS" );
 cursor->insertBlock();

 f.setFontWeight( QFont::Normal );
 f.setFontPointSize( 9 );
 cursor->setCharFormat( f );
 cursor->insertText( "DIR.: OBISPO BONEO 717 - SANTA FE -- TELEFONO: (0342) 4-609132 -- CELULAR : (0342) 1 54-340 902 -- E-MAIL: gustavobedetti@yahoo.com.ar \n" );
 cursor->insertBlock();
}


void visorResumen::renderizarDestinatario()
{
 f.setFontPointSize( 11 );
 fb.setAlignment( Qt::AlignLeft );
 cursor->setBlockFormat( fb );
 cursor->setCharFormat( f );
 cursor->insertText( QString("Presupuestado para: %1" ).arg( destinatario ) );
 cursor->insertBlock();
}



void visorResumen::renderizarDetalle()
{
 QTextTableFormat formato;
 QVector<QTextLength> constraints;
 constraints << QTextLength(QTextLength::PercentageLength, 90 );
 constraints << QTextLength(QTextLength::PercentageLength, 10 );
 formato.setColumnWidthConstraints( constraints );
 formato.setBorder( 0 );
 QTextTable *tabla = cursor->insertTable( 3, 2, formato );
 tabla->mergeCells( 0, 0, 1, 2 );
 tabla->mergeCells( 1, 0, 1, 2 );

 QTextTableCell celda = tabla->cellAt( 0,0 );
 QTextCharFormat formatocelda = celda.format();
 formatocelda.setBackground( Qt::lightGray );
 formatocelda.setFontWeight( QFont::Bold );
 celda.setFormat( formatocelda );
 QTextBlockFormat centrado = celda.format().toBlockFormat();
 centrado.setAlignment( Qt::AlignHCenter );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertText( "PRESUPUESTO" );

 celda = tabla->cellAt( 1, 0 );
 formatocelda.clearBackground();
 formatocelda.clearForeground();
 formatocelda.setFontWeight( QFont::Normal );
 formatocelda.setFontPointSize( 12 );
 celda.setFormat( formatocelda );
 celda.firstCursorPosition().insertHtml( contenido );

 tabla->splitCell( 2, 0, 1, 2 );
 celda = tabla->cellAt( 2, 0 );
 fb.setAlignment( Qt::AlignRight );
 celda.firstCursorPosition().setBlockFormat( fb );
 celda.firstCursorPosition().insertText( "Total presupuestado:" );

 celda = tabla->cellAt( 2, 1 );
 f.setFontWeight( QFont::Bold );
 celda.firstCursorPosition().setBlockCharFormat( f );
 celda.firstCursorPosition().setBlockFormat( centrado );
 celda.firstCursorPosition().insertText( QString( "$ %L1" ).arg( total ) );

 cursor->movePosition( QTextCursor::End );
 cursor->insertBlock();
 f.setFontItalic( true );
 f.setFontPointSize( 8 );
 cursor->setBlockCharFormat( f );
 fb.setAlignment( Qt::AlignRight );
 cursor->setBlockFormat( fb );
 cursor->insertText( "Fecha:" + QDate::currentDate().toString() );
}


/*!
    \fn visorResumen::nombre()
 */
QString visorResumen::nombre()
{
 return destinatario;
}


/*!
    \fn visorResumen::set_registro( int id )
 */
void visorResumen::set_registro( int id )
{
 QSqlQuery cola( QString( "SELECT fecha, destinatario, contenido, total FROM presupuestos WHERE id = %1" ).arg( id ) );
 if( cola.next() )
 {
  QSqlRecord registro = cola.record();
  contenido = registro.value( "contenido" ).toString();
  destinatario = registro.value( "destinatario" ).toString();
  total = registro.value( "total" ).toDouble();
  fecha = registro.value( "fecha" ).toDate();
  renderizarCabecera();
  renderizarDestinatario();
  renderizarDetalle();
 }
 else
 {
  qDebug( "Error al ejecutar la cola para el contenido del resumen" );
 }
}


/*!
    \fn visorResumen::cerrate()
 */
void visorResumen::cerrate()
{
 destinatario = "";
 total = 0;
}


/*!
    \fn visorResumen::imprimir()
 */
void visorResumen::imprimir()
{
     QPrinter printer;
     QPrintDialog *dialog = new QPrintDialog( &printer, this );
     dialog->setWindowTitle( "Imprimir Presupuesto" );
     if ( textCursor().hasSelection() )
         dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
     if ( dialog->exec() != QDialog::Accepted )
         return;

     document()->print(&printer);
}


/*!
    \fn visorResumen::aPdf()
 */
void visorResumen::aPdf()
{
 
 #ifndef QT_NO_PRINTER
     QString fileName = QFileDialog::getSaveFileName( this, "Exportar a PDF", QDir::homePath() + QDir::separator() + nombre(), "*.pdf");
     if (!fileName.isEmpty()) {
         if (QFileInfo(fileName).suffix().isEmpty())
             fileName.append(".pdf");
         QPrinter printer(QPrinter::HighResolution);
         printer.setOutputFormat(QPrinter::PdfFormat);
         printer.setOutputFileName(fileName);
         document()->print(&printer);
     }
 #endif
}
