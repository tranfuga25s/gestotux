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
#include "irenderizador.h"

#include <QTextDocument>


IRenderizador::IRenderizador( QObject *parent )
: QObject( parent )
{
 _doc = new QTextDocument( this );
 cargarEstilo();
}


IRenderizador::~IRenderizador()
{
}


/*!
    \fn IRenderizador::imprimir( QPrinter *impresora )
 */
void IRenderizador::imprimir( QPrinter *impresora )
{
  QTextDocument _d;
 _d.setDefaultStyleSheet( _estilo );
 _d.setHtml( _doc->toHtml() );
 _d.print( impresora );
}


/*!
    \fn IRenderizador::setEstablecimiento( int idDB )
 */
void IRenderizador::setEstablecimiento( int idDB )
{ this->_idEstablecimiento = idDB; }


#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlError>

/*!
    \fn IRenderizador::setEstablecimiento( QString nombre )
 */
void IRenderizador::setEstablecimiento( QString nombre )
{
 QSqlQuery *cola = new QSqlQuery();
 if( cola->exec( QString( "SELECT id_establecimiento FROM car_establecimientos WHERE nombre = '%1'" ).arg( nombre ) ) )
 {
  if( cola->next() )
  {
   this->_idEstablecimiento = cola->record().value(0).toInt();
  }
 }
 else
 {
  qWarning( QString( "Error al buscar el id de establecimiento\n Error: %1\n %2" ).arg( cola->lastError().text() ).arg( cola->lastQuery() ).toLocal8Bit() );
 }
 delete cola;
}

#include <QDate>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>

/*!
    \fn IRenderizador::hacerInforme()
 */
void IRenderizador::hacerInforme()
{
 QTextCursor *cursor = new QTextCursor( _doc );
 cursor->movePosition( QTextCursor::End );
 // cargar la cabecera
 cursor->insertHtml( cargarCabecera() );
 // Pongo la fecha del informe
 cursor->insertText( QString( "Fecha: %1.\n" ).arg( QDate::currentDate().toString( "dd/MM/yyyy" ) ) );
 // Establecimiento en cuestion
 QSqlQuery *colaAuxiliar = new QSqlQuery();
 colaAuxiliar->exec( QString("SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'").arg( _idEstablecimiento ) );
 if( colaAuxiliar->next() )
 {
 	cursor->insertText( QString( "Establecimiento:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
 }
 else
 { qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
 // Busco las caravanas que estan en ese establecimiento
 colaAuxiliar->exec( QString( "SELECT codigo FROM car_caravana WHERE id_caravana IN (  SELECT id_caravana FROM car_carv_tri WHERE id_tri IN (  SELECT id_tri FROM car_tri WHERE ( id_estab_origen = '%1' OR id_estab_destino = '%1' ) AND razon IN ( 2, 3 ) ) )" ).arg( _idEstablecimiento ) );
 if( colaAuxiliar->size() == 0 )
 {
   cursor->movePosition( QTextCursor::End );
   cursor->insertText( "\n\nNo existen resultados" );
 }
 else
 {
	// Genero la tabla
	cursor->movePosition( QTextCursor::End );
	tabla = cursor->insertTable( 1, 2 );
	QTextTableFormat formatoTabla = tabla->format();
	formatoTabla.setHeaderRowCount(1);
	tabla->setFormat( formatoTabla );
	tabla->cellAt( 0,0 ).firstCursorPosition().insertHtml( "#Num" );
	tabla->cellAt( 0,1 ).firstCursorPosition().insertHtml( "#Caravana" );
	while( colaAuxiliar->next() )
	{
		int pos = tabla->rows();
		tabla->insertRows( pos, 1 );
		tabla->cellAt( pos, 0 ).firstCursorPosition().insertHtml( QString( "%L1" ).arg( pos ) );
		tabla->cellAt( pos, 1 ).firstCursorPosition().insertHtml( colaAuxiliar->record().value(0).toString() );
	}
 } // fin else cola != 0
 delete colaAuxiliar;
 delete cursor;
 return;
}

#include <QFile>
#include <QApplication>

/*!
    \fn IRenderizador::cargarCabecera()
 */
QString IRenderizador::cargarCabecera()
{
 QString _cabecera;
 QFile *archivoEstilo;
 if( QFile::exists( QApplication::applicationDirPath() + "/plugins/informes/cabecera.html" ) )
 {
  archivoEstilo = new QFile( QApplication::applicationDirPath() + "/plugins/informes/cabecera.html" );
  if( !archivoEstilo->open( QIODevice::ReadOnly ) )
  {
    qDebug( QString( "Error al abrir el archivo de cabecera: %1" ).arg( archivoEstilo->fileName() ).toLocal8Bit() );
    return "";
  }
  _cabecera = archivoEstilo->readAll();
  archivoEstilo->close();
  delete( archivoEstilo );
  archivoEstilo = 0;
  return _cabecera;
 }
 else
 { return ""; }
}


/*!
    \fn IRenderizador::cargarEstilo()
 */
QString IRenderizador::cargarEstilo()
{
  QString nombre;
 QFile *archivoEstilo;
 if( QFile::exists( QApplication::applicationDirPath() + "/plugins/informes/estilo.css" ) )
 {
  nombre = QApplication::applicationDirPath() + "/plugins/informes/estilo.css";
 }
 else
 {
  qDebug( QString( "archivo %1 no encontrado" ).arg( QApplication::applicationDirPath() + "/plugins/informes/estilo.css" ).toLocal8Bit() );
  nombre = ":/informes/estiloinforme.css";
 }
 archivoEstilo = new QFile( nombre );
 if( !archivoEstilo->open( QIODevice::ReadOnly ) )
 {
  qDebug( QString( "Error al abrir el archivo de estilo: %1" ).arg( nombre ).toLocal8Bit() );
  return "";
 }
 _estilo = archivoEstilo->readAll();
 archivoEstilo->close();
 delete( archivoEstilo );
 archivoEstilo = 0;
 return _estilo;
}
