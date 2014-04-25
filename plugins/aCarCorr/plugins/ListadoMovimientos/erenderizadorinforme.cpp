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
#include "erenderizadorinforme.h"

#include <QTextDocument>
#include "formfiltro.h"
#include "../../TipoMovs.h"
#include <QTextCursor>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QTextTable>
#include <QProgressDialog>

ERenderizadorInforme::ERenderizadorInforme( QObject *padre )
: QObject( padre )
{
 _doc = new QTextDocument( this );
}


ERenderizadorInforme::~ERenderizadorInforme()
{}

/*!
    \fn ERenderizadorInforme::setPropiedades( FormFiltro *f, QString estilo, QString cabecera )
	Setea las propiedades de el infome segun lo elegido en el dialogo.
	@param f Formulario de filtro
	@param estilo Hoja de estilo correspondiente
	@param cabecera Datos html de la cabecera
 */
void ERenderizadorInforme::setPropiedades( FormFiltro *f, QString estilo, QString cabecera )
{
 _estilo = estilo;
 _cabecera = cabecera;
 _filtra_tipo = _filtra_categoria = _filtra_fecha = _filtra_rango_fecha = _filtra_estab = false;
 if( f->tieneTipo() )
 {
   _filtra_tipo = true;
   _id_tipo = f->tipo();
 }
 if( f->tieneEstablecimiento() )
 {
	_filtra_estab = true;
	_id_estab = f->establecimiento();
 }
 if( f->tieneCategoria() )
 {
	_filtra_categoria = true;
	_id_cat = f->categoria();
 }
 if( f->tieneFechaEspecifica() )
 {
	_filtra_fecha = true;
	_fecha = f->FechaEspecifica();
 }
 if( f->tieneRangoFecha() )
 {
	_filtra_rango_fecha = true;
	_rango_fechas = f->rangoFechas();
 }
}

/*!
    \fn ERenderizadorInforme::hacerInforme()
 */
bool ERenderizadorInforme::hacerInforme()
{
 d = new QProgressDialog();
 d->setLabelText( "Buscando coincidencias..." );
 d->setMinimumDuration( 0 );
 d->show();
 QStringList tris = buscarTris();
 if( tris.isEmpty() )
 {
	d->setLabelText( "No se encontraron coincidencias" );
	d->setRange( 0, 1 );
	d->setValue( 1 );
 }
 foreach( QString tri, tris )
 {
  // Genero la cabecera para ese tri
  d->setLabelText( "Generando cabecera" );
  if( hacerCabecera( tri ) == true )
  {
	// El metodo de vuelve true si existen caravanas para hacer lista
   	colocarContenido( tri );
  }
  else
  {
 	// insertar el salto de hoja
	QTextCursor cursor(_doc);
	cursor.movePosition( QTextCursor::End );
	cursor.insertHtml( "<table><th><td></td></th></table>" );
  }
 }
 d->setLabelText( "Mostrando Informe.." );
 return true;
}


/*!
    \fn ERenderizadorInforme::hacerCabecera( QString tri )
	Reliza la cabecera de una pagina para el tri especificado
	@param tri Codigo del tri
	@return Verdadero si se pudo hacer, falso si no se encontro el tri
 */
bool ERenderizadorInforme::hacerCabecera( QString tri )
{
 bool devolucion = false;
 QTextCursor *cursor = new QTextCursor( _doc );
 cursor->movePosition( QTextCursor::End );
 // cargar la cabecera
 cursor->insertHtml( _cabecera );
 // Busco los datos del tri
 QSqlQuery *cola = new QSqlQuery();
 cola->exec( QString("SELECT * FROM car_tri WHERE id_tri = '%1'" ).arg( tri ) );
 if( cola->next() )
 {
	/*
	nombre de la empresa arriba en grande centrado
	abajo
	centrado un poquito mas chico
	nombre del sistema (Sistema de  gestion de trazabilidad) Administracion Corrientes S.A nombre de la empresa
	Nro de Tri
	Fecha
	Categoria
	Cantidad de caravanas
	Vendedor (o campo destino y origen)
	Comprador
	Nro de dta
	Nro de Guia
	*/
	// Fecha
	cursor->insertText( QString( "Fecha: %1.\n" ).arg( cola->record().value( "fecha" ).toDate().toString( "dd/MM/yyyy" ) ) );
	// Numero de Tri
	cursor->insertText( QString( "#TRI: %1.\n" ).arg( cola->record().value( "id_tri" ).toString() ) );
	// Categoria
	QSqlQuery *colaAuxiliar = new QSqlQuery();
	bool especial = false;
	colaAuxiliar->exec(  QString("SELECT nombre, especial FROM car_categorias WHERE id_categoria = '%1'").arg( cola->record().value( "id_categoria" ).toString() ) );
	if(colaAuxiliar->next())
	{
 		cursor->insertText( QString( "Categoria: %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
		especial = colaAuxiliar->record().value(1).toBool();
	} else { qDebug( QString( "Error al ejecutar la cola de nombre de categoria: %1" ).arg( colaAuxiliar->lastError().text() ).toLocal8Bit() ); }
	// Cantidad de Caravanas
	// Si el tri es de categoria especial la cantidad va a ser mayor que cero, si es normal, va a ser cero
	if( especial == true )
	{
		// No genero la lista de caravanas
		cursor->insertText( QString( "Cantidad de Animales: %L1 \n" ).arg( cola->record().value( "cantidad_caravanas" ).toInt() ) );
		devolucion = false;
	}
	else
	{
		colaAuxiliar->exec( QString( "SELECT COUNT(id_caravana) FROM car_carv_tri WHERE id_tri ='%1'" ).arg( tri ) );
		if(colaAuxiliar->next())
		{
			cursor->insertText( QString( "Cantidad de Caravanas: %L1 \n" ).arg( colaAuxiliar->record().value(0).toInt() ) );
		} else { qDebug( QString( "Error al ejecutar la cola de cantidad de caravanas: %1" ).arg( colaAuxiliar->lastError().text() ).toLocal8Bit() ); }
		devolucion = true;
	}
	QString texti;
	switch( cola->record().value( "razon" ).toInt() )
	{
		case venta:
		{
			texti = "Venta de Caravanas";
			// Establecimiento de origen
			colaAuxiliar->exec( QString("SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'").arg( cola->record().value( "id_estab_origen" ).toInt() ) );
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Establecimiento de origen:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
			// Comprador
			if( QSqlDatabase::database().driverName() == "QSQLITE" )
			{
				colaAuxiliar->exec( QString( "SELECT apellido || ', ' || nombre FROM clientes WHERE id = '%1'" ).arg( cola->record().value( "id_comprador" ).toInt() ) );
			}
			else if( QSqlDatabase::database().driverName() == "QMYSQL" )
			{
				colaAuxiliar->exec( QString( "SELECT CONCAT( CONCAT( apellido, \", \" ), nombre ) FROM clientes WHERE id = '%1'" ).arg( cola->record().value( "id_comprador" ).toInt() ) );
			}
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Comprador:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de cliente" ); }
			break;
		}
		case compra:
		{
			texti = "Ingreso de Caravanas por Compra";
			colaAuxiliar->exec( QString("SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'").arg( cola->record().value( "id_estab_destino" ).toInt() ) );
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Establecimiento de destino:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
			// Vendedor
			if( QSqlDatabase::database().driverName() == "QSQLITE" )
			{
				colaAuxiliar->exec( QString( "SELECT apellido || ', ' || nombre FROM clientes WHERE id = '%1'" ).arg( cola->record().value( "id_vendedor" ).toInt() ) );
			}
			else if( QSqlDatabase::database().driverName() == "QMYSQL" )
			{
				colaAuxiliar->exec( QString( "SELECT CONCAT( CONCAT( apellido, \", \" ), nombre ) FROM clientes WHERE id = '%1'" ).arg( cola->record().value( "id_vendedor" ).toInt() ) );
			}
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Vendedor:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
			break;
		}
		case mudanza:
		{
			texti = "Movimiento de Caravanas entre Establecimientos";
			colaAuxiliar->exec( QString("SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'").arg( cola->record().value( "id_estab_origen" ).toInt() ) );
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Establecimiento de origen:  %1 \n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
			colaAuxiliar->exec( QString("SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'").arg( cola->record().value( "id_estab_destino" ).toInt() ) );
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Establecimiento de destino:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
			break;
		}
		case stock:
		{
			texti = "Ingreso por stock";
			colaAuxiliar->exec( QString("SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'").arg( cola->record().value( "id_estab_destino" ).toInt() ) );
			if( colaAuxiliar->next() )
			{
				cursor->insertText( QString( "Establecimiento de destino:  %1\n" ).arg( colaAuxiliar->record().value(0).toString() ) );
			}
			else
			{ qDebug( "Error al ejecutar la cola de nombre de establecimiento" ); }
			break;
		}
	}
	delete colaAuxiliar;
	cursor->insertText( texti + " \n" );
	cursor->insertText( QString( "#DTA: %1 \n" ).arg( cola->record().value( "dta" ).toString() ) );
	if( !cola->record().value("guia").toString().isEmpty() )
	{ cursor->insertText( QString( "#Guia: %1 \n" ).arg( cola->record().value( "guia" ).toString() ) ); }

 }
 else
 {
  qDebug( "Error al seleccionar el tipo de movimiento" );
  qDebug( cola->lastError().text().toLocal8Bit() );
  devolucion = false;
 }
 delete cursor;
 delete cola;
 return devolucion;
 // listo
}

void ERenderizadorInforme::colocarContenido( QString tri )
{
 if( tri == 0 )
 {
  qDebug( "Error, tri == 0" );
  return;
 }
 QTextCursor *cursor = new QTextCursor( _doc );
 d->setLabelText( "Buscando los datos.." );
 QSqlQuery *cola = new QSqlQuery();
 cola->exec( QString( "SELECT codigo FROM car_caravana WHERE id_caravana IN ( SELECT id_caravana FROM car_carv_tri WHERE id_tri = '%1' )" ).arg( tri ) );
 if( cola->size() == 0 )
 {
   cursor->movePosition( QTextCursor::End );
   cursor->insertText( "\n\nNo existen resultados" );
 }
 else
 {
	d->setLabelText( "Generando informe" );
	d->setRange( 0, cola->size() );
	// Genero la tabla
	cursor->movePosition( QTextCursor::End );
	tabla = cursor->insertTable( 1, 2 );
	QTextTableFormat formatoTabla = tabla->format();
	formatoTabla.setHeaderRowCount(1);
	tabla->setFormat( formatoTabla );
	tabla->cellAt( 0,0 ).firstCursorPosition().insertHtml( "#Num" );
	tabla->cellAt( 0,1 ).firstCursorPosition().insertHtml( "#Caravana" );
	while( cola->next() )
	{
		int pos = tabla->rows();
		tabla->insertRows( pos, 1 );
		tabla->cellAt( pos, 0 ).firstCursorPosition().insertHtml( QString( "%L1" ).arg( pos ) );
		tabla->cellAt( pos, 1 ).firstCursorPosition().insertHtml( cola->record().value(0).toString() );
		d->setValue( d->value() + 1 );
	}
 } // fin else cola != 0
 delete cola;
 delete cursor;
}

/*!
    \fn ERenderizadorInforme::cerrarDialogo()
 */
void ERenderizadorInforme::cerrarDialogo()
{
 d->close();
}

#include <QPrinter>
#include <QTextEdit>
/*!
    \fn ERenderizadorInforme::imprimir( QPrinter *impresora )
 */
void ERenderizadorInforme::imprimir( QPrinter *impresora )
{
 QTextDocument _d;
 _d.setDefaultStyleSheet( _estilo );
 _d.setHtml( _doc->toHtml() );
 _d.print( impresora );
}


/*!
    \fn ERenderizadorInforme::buscarTris()
	Busca la lista de tris que coinciden con el codigo y los devuelve en una lista
 */
QStringList ERenderizadorInforme::buscarTris()
{
  QStringList lista;
  QString where;
  bool filtra_algo = false;
 if( _filtra_categoria )
 {
  if( !where.isEmpty() )
  {
    where += " AND ";
  }
  where += QString("id_categoria = '%1'").arg( _id_cat );
  filtra_algo = true;
 }
 if( _filtra_tipo )
 {
  if( !where.isEmpty() )
  {
    where += " AND ";
  }
  where += QString("razon = '%1'").arg( _id_tipo );
  filtra_algo = true;
 }
 if( _filtra_estab )
 {
  if( !where.isEmpty() )
  {
    where += " AND ";
  }
  where += QString( " id_tri IN ( SELECT id_tri FROM car_tri WHERE ( id_estab_origen = '%1' OR id_estab_destino = '%1' ) )" ).arg( _id_estab );
  filtra_algo = true;
 }
 if( _filtra_fecha )
 {
  if( !where.isEmpty() )
  {
   where += " AND ";
  }
  where += QString( " fecha = '%1' " ).arg( _fecha.toString(Qt::ISODate) );
  filtra_algo = true;
 }
 if( _filtra_rango_fecha )
 {
  if( !where.isEmpty() )
  {
   where += " AND ";
  }
  where += QString( " fecha BETWEEN( '%1', '%2' )  " ).arg( _rango_fechas.first.toString(Qt::ISODate) ).arg( _rango_fechas.second.toString(Qt::ISODate) );
  filtra_algo = true;
 }
 if( filtra_algo )
 {
  where.prepend( "WHERE " );
 }
 QSqlQuery *cola = new QSqlQuery();
 if( cola->exec( QString( "SELECT id_tri FROM car_tri %1" ).arg( where ) ) )
 {
  qDebug( cola->lastQuery().toLocal8Bit() );
  while( cola->next() )
  {
	lista.append( cola->record().value(0).toString() );
  }
 }
 else
 {
  	qWarning( QString( "Error al hacer el exec de la cola de obtener tris\n Error: %1\n %2" ).arg( cola->lastError().text() ).arg( cola->lastQuery() ).toLocal8Bit() );
 }
 delete cola;
 return lista;
}

