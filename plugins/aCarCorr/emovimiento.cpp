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
#include "emovimiento.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPair>
#include <QSqlError>
#include <QVariant>
#include <QProgressDialog>
#include <QErrorMessage>

EMovimiento::EMovimiento(QObject *parent)
 : QObject(parent)
{
 tipo_mov = invalido;
 id_db = -1;
 _cantidad_animales = 0;
}


EMovimiento::~EMovimiento()
{}

int EMovimiento::tipoMov() const
{ return tipo_mov; }


void EMovimiento::setTipoMov( const int& theValue )
{
 // Esto evita cambiar el tipo de mov despues de setearlo por primera vez
 if( tipo_mov == invalido && theValue > 1 )
 {
  tipo_mov = theValue;
  qDebug( QString( "Seteado tipo de movimiento a %1 " ).arg( tipo_mov ).toLocal8Bit() );
 }
 else
 {
  qWarning( "Intentando resetear el tipo de movimiento" );
 }
}


/*!
    \fn EMovimiento::cargarMovimiento( int idDb )
	Carga todos los datos relacionados con el id de la base de datos de un movimiento
	@param idDB Identificador de clave primaria que indica el movimiento que se debe cargar
 */
bool EMovimiento::cargarMovimiento( int idDb )
{
 if( idDb <= 0 )
 {
  qWarning( "Error al indicar el identificador de TRI" );
  return false;
 }
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT * FROM car_tri WHERE id_tri = '%1'" ).arg( idDb ) ) )
 {
  qDebug( cola.lastQuery().toLocal8Bit() );
  if( cola.next() )
  {
	id_db = idDb;
	DTA = cola.record().value( "dta" ).toString();
	id_db = cola.record().value( "id_tri" ).toInt();
	setCategoria( cola.record().value( "id_categoria" ).toInt() );
	setFecha( cola.record().value("fecha").toDate() );
	setCantidadAnimales( cola.record().value("cantidad_caravanas" ).toInt() );
	// Busco el establecimiento
	setTipoMov( cola.record().value( "razon" ).toInt() );
	switch( tipoMov() )
	{
		case compra:
		{
			//cargo el vendedor y el destino
			setVendedor( cola.record().value( "id_vendedor" ).toInt() );
			comprador.first = -1;
			origen.first = -1;
			setEstablecimientoDestino( cola.record().value( "id_estab_destino" ).toInt() );
			break;
		}
		case venta:
		{
			//cargo el comprador y el origen
			setComprador( cola.record().value( "id_comprador" ).toInt() );
			vendedor.first = -1;
			destino.first = -1;
			setEstablecimientoOrigen(cola.record().value( "id_estab_origen" ).toInt());
			break;
		}
		case mudanza:
		{
			//cargo el destino y el origen
			vendedor.first = -1;
			comprador.first = -1;
			setEstablecimientoOrigen( cola.record().value( "id_estab_origen" ).toInt() );
			setEstablecimientoDestino( cola.record().value( "id_estab_destino" ).toInt() );
			break;
		}
		case stock:
		{
			vendedor.first = -1;
			comprador.first = -1;
			origen.first = -1;
			setEstablecimientoDestino( cola.record().value( "id_estab_destino" ).toInt() );
			break;
		}
	}
	// Cargo las caravanas que corresponden a este tri si no son categoria especial
	return cargarCaravanas();
  }
  else
  {
 	qWarning( QString( "Error al next buscar el tri\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
	return false;
  }
 }
 else
 {
	qWarning( QString( "Error al buscar el tri\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
  	return false;
 }
}


/*!
    \fn EMovimiento::caravanas()
	Devuleve la lista de caravanas que componen este tri
	@return Objecto QStringList con los codigos de caravanas
 */
QStringList EMovimiento::caravanas()
{
	//Devulve la lista de los codigos de caravanas que corresponden a este TRI
	if( !_caravanas.isEmpty() )
	{
		return _caravanas;
	}
	else
	{
		return QStringList();
	}

}


QString EMovimiento::getDTA() const
{
	return DTA;
}


bool EMovimiento::setDTA ( const QString& theValue, bool nuevo )
{
	if( nuevo )
	{
		// Verifico que no exista antes
		QSqlQuery cola;
		if( cola.exec(QString( "SELECT id_tri FROM car_tri WHERE dta = '%1'" ).arg( theValue ) ) )
		{
			if( cola.next() )
			{
				qWarning( "El #DTA que esta intentando utilizar para este TRI ya existe" );
				return false;
			}
			else
			{
				qDebug( "Seteando DTA" );
				DTA = theValue;
				return true;
			}
		}
		else
		{
			qWarning( QString( "Error al buscar si existe anteriormente un dta\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
			return false;
		}
	}
	else
	{
		// El Dta ya tiene que existir
		if( theValue != DTA )
		{
			// tengo que verificar que no exista ya el nuevo
			return setDTA( theValue, true );
		}
		else
		{
			// Es el valor igual al que estaba antes... No hago nada
			return true;
		}
	}
}


/*!
    \fn EMovimiento::cargarNombreCategoria( int idDB )
	Funcion que carga el nombre de la categoria en la variable del objeto
	@param idDB identificador de clave primaria de la categoria
 */
void EMovimiento::cargarNombreCategoria( const int idDB )
{
 QSqlQuery cola2;
 if( cola2.exec( QString( "SELECT nombre FROM car_categorias WHERE id_categoria = '%1'" ).arg( idDB ) ) )
 {
	if( cola2.next() )
	{
		categoria.second = cola2.record().value(0).toString();
		categoria.first = idDB;
	}
 }
 else
 {
 	qWarning( QString( "Error al buscar el nombre de la categoria\n Error: %1\n %2" ).arg( cola2.lastError().text() ).arg( cola2.lastQuery() ).toLocal8Bit() );
 }
}


/*!
    \fn EMovimiento::cargarNombreComprador( int idDB )
	Funcion que carga el nombre del comprador en la variable del objeto
	@param idDB identificador de clave primaria del cliente comprador
 */
void EMovimiento::cargarNombreComprador( const int idDB )
{
 QSqlQuery cola2;
 if( cola2.exec( QString( "SELECT apellido || ', ' || nombre FROM clientes WHERE id = '%1'" ).arg( idDB ) ) )
 {
	if( cola2.next() )
	{
		comprador.second = cola2.record().value(0).toString();
		comprador.first = idDB;
	}
 }
 else
 {
 	qWarning( QString( "Error al buscar el cliente como comprador\n Error: %1\n %2" ).arg( cola2.lastError().text() ).arg( cola2.lastQuery() ).toLocal8Bit() );
 }
}


/*!
    \fn EMovimiento::setComprador( int id )
	Funcion idem anterior, coloca el id y busca los datos
        @param id Identificadode clave primaria del comprado
 */
void EMovimiento::setComprador( const int id )
{
 if( id <= 0 )
 {
  return;
 }
 comprador.first = id;
 cargarNombreComprador( id );
}


/*!
    \fn EMovimiento::setCategoria( int id )
 */
void EMovimiento::setCategoria( const int id )
{
 categoria.first = id;
 cargarNombreCategoria( id );
}


/*!
    \fn EMovimiento::cargarNombreVendedor( int idDB )
	Funcion que carga el nombre del vendedor en la variable del objeto
	@param idDB identificador de clave primaria del cliente vendedor
 */
void EMovimiento::cargarNombreVendedor( const int idDB )
{
  QSqlQuery cola2;
 if( cola2.exec( QString( "SELECT apellido || ', ' || nombre FROM clientes WHERE id = '%1'" ).arg( idDB ) ) )
 {
	if( cola2.next() )
	{
		vendedor.second = cola2.record().value(0).toString();
		vendedor.first = idDB;
	}
 }
 else
 {
 	qWarning( QString( "Error al buscar el cliente como vendedor\n Error: %1\n %2" ).arg( cola2.lastError().text() ).arg( cola2.lastQuery() ).toLocal8Bit() );
 }
}


/*!
    \fn EMovimiento::setVendedor( int idDB )
	Setea en los datos del objeto el ID de cliente que se usara como vendedor.
	@param idDB Identificador de cliente
 */
void EMovimiento::setVendedor( const int idDB )
{
 vendedor.first = idDB;
 cargarNombreVendedor( idDB );
}


/*!
    \fn EMovimiento::cargarNombreEstablecimientoDestino( int idDB )
	Coloca en las variables del objeto el nombre del establecimiento de destino.
	@param idDB Identificador del establecimiento de destino.
 */
void EMovimiento::cargarNombreEstablecimientoDestino( const int idDB )
{
 QSqlQuery cola2;
 if( cola2.exec( QString( "SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'" ).arg( idDB ) ) )
 {
 	if( cola2.next() )
	{
		destino.second = cola2.record().value(0).toString();
		destino.first = idDB;
	}
 }
 else
 {
 	qWarning( QString( "Error al buscar el establecimiento de destino\n Error: %1\n %2" ).arg( cola2.lastError().text() ).arg( cola2.lastQuery() ).toLocal8Bit() );
 }
}


/*!
    \fn EMovimiento::setEstablecimientoDestino( int idDB )
	Setea las variables internas de establecimiento de destino y carga el nombre
	@param idDB Identificador del Establecimiento de destino
 */
void EMovimiento::setEstablecimientoDestino( const int idDB )
{
 destino.first = idDB;
 cargarNombreEstablecimientoDestino( idDB );
}

/*!
    \fn EMovimiento::cargarNombreEstablecimientoDestino( int idDB )
	Carga el nombre del establecimiento de destino en las variables del objeto.
	@param idDB Identificador del establecimiento
 */
void EMovimiento::cargarNombreEstablecimientoOrigen( const int idDB )
{
 QSqlQuery cola2;
 if( cola2.exec( QString( "SELECT nombre FROM car_establecimientos WHERE id_establecimiento = '%1'" ).arg( idDB ) ) )
 {
 	if( cola2.next() )
	{
		origen.second = cola2.record().value(0).toString();
		origen.first = idDB;
	}
 }
 else
 {
 	qWarning( QString( "Error al buscar el establecimiento de origen\n Error: %1\n %2" ).arg( cola2.lastError().text() ).arg( cola2.lastQuery() ).toLocal8Bit() );
 }
}


/*!
    \fn EMovimiento::setEstablecimientoDestino( int idDB )
	Setea los datos internos del establecimiento de destino. Id y nombre son cargados.
	@param idDB Identificador de base de datos del establecimiento
 */
void EMovimiento::setEstablecimientoOrigen( const int idDB )
{
 origen.first = idDB;
 cargarNombreEstablecimientoOrigen( idDB );
}


QDate EMovimiento::getFecha() const
{ return fecha; }

void EMovimiento::setFecha ( const QDate& theValue )
{ fecha = theValue; }

/*!
    \fn EMovimiento::guardar( QProgressDialog *dialogo )
	Guarda los datos que existan en la variables de la clase a la base de datos y actualiza las caravanas y sus dueños.
	@param dialogo Dialogo para indicar el avance de la operacion
 */
int EMovimiento::guardar( QProgressDialog *dialogo )
{
 if( tipoMov() > invalido )
 {
	// inicio la transaccion
	if( !iniciarTransaccion() )
	{
		return -1;
	}
	bool estado = true;
	bool nuevo  = false;
	// Calculo la cantidad para el dialogo
	dialogo->setRange( 0, (_caravanas.size() * 2) + 1 );
	dialogo->setValue( 0 );
 	QSqlQuery cola;
	QString scola = QString( " car_tri( dta, guia, fecha, razon, id_categoria, id_estab_destino, id_estab_origen, id_comprador, id_vendedor, cantidad_caravanas ) VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10' )" ).arg( DTA ).arg( _numGuia ).arg( fecha.toString(Qt::ISODate) ).arg( tipoMov() ).arg( categoria.first ).arg( destino.first ).arg( origen.first ).arg( comprador.first ).arg( vendedor.first ).arg( _cantidad_animales );
   	if( id_db  == -1 )
 	{
  		// Agrego un nuevo registro
		scola.prepend( "INSERT INTO" );
		nuevo = true;
 	}
 	else
 	{
  		// Actualizo el registro actual
		scola.prepend( "UPDATE" );
		scola.append( QString( " WHERE id_tri = '%1' LIMIT 1").arg( id_db ) );
 	}
	dialogo->setValue( dialogo->value() + 1 );
	if( cola.exec( scola ) )
	{
		//Devulevo el id que inserte
		if( id_db == -1 )
		{
			QSqlQuery cola1( "SELECT seq FROM sqlite_sequence WHERE name = 'car_tri' LIMIT 1" );
			if( cola1.next() )
			{
				if( cola1.record().value("seq").toInt() > 0 )
				{
					id_db =  cola1.record().value(0).toInt();
					qDebug( QString("usando id de insercion %1" ).arg( id_db ).toLocal8Bit() );
				}
				else
				{
					qWarning( "Error al obtener el id insertado" );
					deshacerCambiosDb();
					return -4;
				}

			}
			else
			{
				qWarning( QString( "Error al obtener el numero de tri al hacer insercion\n Error: %1\n %2" ).arg( cola1.lastError().text() ).arg( cola1.lastQuery() ).toLocal8Bit() );
				deshacerCambiosDb();
				return -1;
			}
		}
		// Si el id no es -1, es el valor que estuvimos usando
	}
	else
	{
		qWarning( QString( "Error al insertar o actualizar el registro de tri\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
		deshacerCambiosDb();
		return -2;
	}
	if( _cantidad_animales == 0 ) // Si no es de categoria especial...
	{
		// Si es registro nuevo agrego simplemente las caravanas
		if( nuevo == true )
		{
			// Guardo las caravanas
			QString codigo;
			foreach( codigo, _caravanas )
			{
				if( !estado )
				{
					qDebug( "Error en el fro" );
					deshacerCambiosDb();
					return -2;
				}
				estado = guardarCaravana( codigo );
				dialogo->setValue( dialogo->value() + 1 );
				if( !estado )
				{
					qDebug( "Error al intenar guardar la caravana" );
					deshacerCambiosDb();
					return -3;
				}
				int id_caravana = getIDCaravana( codigo );
				estado = asociarCaravana( id_caravana );
				dialogo->setValue( dialogo->value() + 1 );
				if( !estado )
				{
					qDebug( "Error al intenar asociar la caravana" );
					deshacerCambiosDb();
					return -3;
				}
			#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
				/// Asociar el dueño!
				estado = aduenarCaravana( id_caravana, 0, fecha, false );
				if( !estado )
				{
					qDebug( "Error al asociar el dueño" );
					deshacerCambiosDb();
					return -3;
				}
			#endif
			}
		}
		else
		{
			// Registro actualizado, actualizar las caravanas
			// busco los actuales porque _caravanas tiene los actualizados
			QSqlQuery *xcola = new QSqlQuery();
			if( xcola->exec( QString( "SELECT id, codigo FROM car_caravana WHERE id_tri = '%1'" ).arg( id_db ) ) )
			{
				QStringList _caravanas2;
				while( xcola->next() )
				{
					_caravanas2.append( xcola->record().value(1).toString() );
				}
				QStringList comp1;
				comp1 = _caravanas2;
				// Comparacion 1 a uno para ver si estan
				QString caravana;
				foreach( caravana, _caravanas )
				{
					if( _caravanas2.contains( caravana ) )
					{
						_caravanas.removeOne( caravana );
						// la elimino de las 2 listas asi quedan solo las ocurrencias unicas en cada una
						comp1.removeOne( caravana );
					}
				}
				if( _caravanas.size() > 0 )
				{
					// existen elementos en caravanas que no estan en _caravanas porque cambiaron o porque son nuevas
					QString agregar;
					foreach( agregar, _caravanas )
					{
						estado = guardarCaravana( agregar );
						dialogo->setValue( dialogo->value() + 1 );
						if( !estado )
						{
							qDebug( "Error al intenar guardar la caravana" );
							deshacerCambiosDb();
							return -3;
						}
						int id_caravana = getIDCaravana( agregar );
						estado = asociarCaravana( id_caravana );
						dialogo->setValue( dialogo->value() + 1 );
						if( !estado )
						{
							qDebug( "Error al intenar asociar la caravana" );
							deshacerCambiosDb();
							return -3;
						}
					#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
						/// Asociar el dueño!
						estado = aduenarCaravana( id_caravana, 0, fecha, false );
						if( !estado )
						{
							qDebug( "Error al asociar el dueño" );
							deshacerCambiosDb();
							return -3;
						}
					#endif
					}//fin bucle lista nueva
				}
				if( comp1.size() > 0 )
				{
					// existen caravanas que fueron eliminadas
					QString eliminame;
					foreach( eliminame, comp1 )
					{
						estado = borrarCaravana( eliminame, /*Disaciociar*/ true );
						if( !estado )
						{
							qDebug( "Error al intentar eliminar una caravana de la lista" );
							deshacerCambiosDb();
							return -12;
						}

					}
					comp1.clear();
				}
			}
			else
			{
				qWarning( "Error al buscar las caravanas para actualizar los datos" );
				deshacerCambiosDb();
			}
			delete xcola;
		}
	}// Fin cantidad de animales == 0
	//@ todo fue ok hasta ahora
	if( hacerCommit() )
	{  return id_db; }
	else
	{ return -16; }
 }
 else
 {
  qWarning( QString("Error al intentar guardar o acutalizar el registro, su tipo fue invalido. Tipo: %1").arg( tipoMov() ).toLocal8Bit() );
  dialogo->setValue( dialogo->maximum() );
  return -14;
 }
}


/*!
    \fn EMovimiento::setCaravanas( QStringList caravanas )
	Actualiza la lista de caravanas asociadas a este tri.
	Si se eliminaron o cambiaron codigos de caravanas, se actualizaran o cambiaran en la db tambien.
	@param caravanas Lista de las caravanas de este tri.
 */
void EMovimiento::setCaravanas( QStringList caravanas )
{
 if( _caravanas == caravanas )
 {
  qWarning( "Las listas de caravanas son iguales" );
  return;
 }
 else
 {
  // Comparacion 1 a uno para ver si estan
  QString caravana;
  foreach( caravana, caravanas )
  {
   if( _caravanas.contains( caravana ) )
   {
    caravanas.removeOne( caravana );
   }
  }
  if( caravanas.size() > 0 )
  {
   // existen elementos en caravanas que no estan en _caravanas porque cambiaron o porque son nuevas
   foreach( caravana, caravanas )
   {
    agregarCaravana( caravana, true );
   }
  }
  return;
 }
}


/*!
    \fn EMovimiento::agregarCaravana( QString codigo, bool verificar )
	Agregega caravanas a la lista interna de caravanas.
	Si verificar es verdadero, se mirara que el codigo no exista ya como caravana.
	@param codigo Codigo de la caravana
	@param verificar Si es verdadero comprueba que no exista y retorna falso si existe, si es falso, la agrega automaticamente.

 */
bool EMovimiento::agregarCaravana( QString codigo, bool verificar )
{
 if( !verificar )
 {
  _caravanas.append( codigo );
  return true;
 }
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT COUNT(codigo) FROM car_caravana WHERE codigo = '%1'" ).arg( codigo ) ) )
  {
   if( cola.next() )
   {
    if( cola.record().value(0).toInt() >= 1 )
    {
 	qWarning( QString( "La caravana de codigo %1 ya existe al menos %2 veces en el sistema, no sera guardada en este tri" ).arg( codigo ).arg( cola.record().value(0).toInt() ).toLocal8Bit() );
	return false;
    }
    else
    {
	_caravanas.append( codigo );
	return true;
    }
   }
   else
   {
     qWarning( QString( "Error al hace next en buscar codigos de caravanas varificando\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
     return false;
   }
  }
  else
  {
    qWarning( QString( "Error al buscar codigos de caravanas con verificacion\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
    return false;
  }
}


/*!
    \fn EMovimiento::eliminarCaravana( QString codigo )
	Elimina de la lista temporal interna la caravana
	@param codigo Codigo de la caravana
 */
void EMovimiento::eliminarCaravana( QString codigo )
{
 // Verifico que este en la lista
 if( _caravanas.contains( codigo ) )
 {
  _caravanas.removeAt( _caravanas.indexOf( codigo ) );
 }
}


/*!
    \fn EMovimiento::guardarCaravana( QString codigo )
	Coloca en la base de dato el codigo de caravana.
	@param codigo Codigo de la caravana
	@return Verdadero si fue exitosa la insercion, falso en caso contrario.
 */
bool EMovimiento::guardarCaravana( QString codigo )
{
 if( tipoMov() == invalido )
 {
  qWarning( "Error en el tipo de movimiento!" );
  return false;
 }
 switch( tipoMov() )
 {
 	case compra:
	case stock:
	{
		QSqlQuery cola;
		if( !cola.exec( QString( "INSERT INTO car_caravana( codigo ) VALUES ( '%1' )" ).arg( codigo ) ) )
		{
			qWarning( QString( "Error al agregar nueva caravana\n Error: %1\n cola: %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
			return false;
		}
		else
		{
			qDebug( "Caravana Agregada" );
			return true;
		}
		break;
	}
	case venta:
	case mudanza:
	{
		/* Eliminacion temporal
		qDebug( "La caravana ya debe existir en la db" );
		return true;
		*/
		QSqlQuery cola;
		if( !cola.exec( QString( "INSERT OR IGNORE INTO car_caravana( codigo ) VALUES ( '%1' )" ).arg( codigo ) ) )
		{
			qWarning( QString( "Error al agregar nueva caravana\n Error: %1\n cola: %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
			return false;
		}
		else
		{
			qDebug( "Caravana Agregada" );
			return true;
		}
		break;
		break;
	}
	default:
	{
		qDebug( "No se guardo ninguna caravana. Tipo de movimiento sin implmenetacion" );
		qDebug( QString::number( tipoMov() ).toLocal8Bit() );
		return false;
		break;
	}
 }
 // Sino es de esos tipos, ya esta dada de alta en la db
 return false;
}


/*!
    \fn EMovimiento::asociarCaravana( int id_caravana )
	Genera la asociación en la base de datos para una caravana con el tri de este movimiento.
	@param id_caravana Numero de caravana ( id de registro )
	@return Verdadero si fue exitosa la asociación
 */
bool EMovimiento::asociarCaravana( int id_caravana )
{
 // Siempre existe el codigo de caravana dado de alta al llamar esta funcion
 QSqlQuery cola1;
 if( !cola1.exec( QString( "INSERT INTO car_carv_tri( id_caravana, id_tri ) VALUES ( '%1' ,'%2' )" ).arg( id_caravana ).arg( id_db ) ) )
 {
	qWarning( QString( "Error al agregar nueva caravana\n Error: %1\n cola: %2" ).arg( cola1.lastError().text() ).arg( cola1.lastQuery() ).toLocal8Bit() );
	return false;
 }
 else
 {
 	qDebug( "Asociacion exitosa" );
 	return true;
 }
}


/*!
    \fn EMovimiento::cargarCaravanas()
 */
bool EMovimiento::cargarCaravanas()
{
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT codigo FROM car_caravana WHERE id_caravana IN ( SELECT id_caravana FROM car_carv_tri WHERE id_tri = '%1' )" ).arg( id_db ) ) )
 {
	while( cola.next() )
	{
		// Le pongo false para que no verifique que existe la caravana
		agregarCaravana( cola.record().value(0).toString(), false );
	}
	return true;
 }
 else
 {
  qWarning( QString( "Error al cargar las caravanas de un tri especifico.\n Error: %1\n cola: %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
  return false;
 }
}


/*!
    \fn EMovimiento::aduenarCaravana( int id_caravana, int id_cliente, QDate fecha, bool cambiar_dueno )
	Funcion que asocia una caravana con un dueño en una fecha especifica
	@param id_caravana Id de registro de la caravana
	@param id_cliente Id del registro del cliente
	@param fecha Fecha de la asociacion
	@param cambiar_dueno Permite el cambio de dueño modificando el registro
	@return verdadero si la operacion se llevo a cabo o falso si no
 */
bool EMovimiento::aduenarCaravana( int id_caravana, int id_cliente, QDate fecha, bool cambiar_dueno )
{
 // Genero la asociacion
 QSqlQuery colas;
 // Busco si tiene una asociacion anterior
 if( colas.exec( QString( "SELECT COUNT(id_caravana) FROM car_carv_duenos WHERE id_caravana = '%1' AND id_cliente = '%2' AND fecha_fin IS NULL" ).arg( id_caravana ).arg( id_cliente ) ) )
 {
  if( colas.next() )
  {
   if( colas.record().value(0).toInt() > 0 )
   {
    // Tiene un dueño anterior
    if( cambiar_dueno )
    {
	// pongo fecha de fin en el anterior
	if( colas.exec( QString( "UPDATE INTO car_carv_duenos SET fecha_fin = '%1' WHERE id_caravana = ( SELECT id_caravana FROM car_caravanas WHERE codigo = '%1' )' AND id_cliente = '%3'" ).arg( id_caravana ).arg( id_cliente ).arg( fecha.toString( Qt::ISODate ) ) ) )
	{
		qDebug( "Escrito fin de duenño anterior correcto." );
		// Escribo el dueño nuevo
		return aduenarCaravana( id_caravana, id_cliente, fecha, false );
	}
	else
	{
		qWarning( QString( "Error al actualizar dueño de caravana.escritura del anterior.\n Error: %1\n cola: %2" ).arg( colas.lastError().text() ).arg( colas.lastQuery() ).toLocal8Bit() );
		return false;
	}
    }
    else
    {
     qWarning( "No se cambio el dueño de la caravana!" );
     return false;
    }
   }
  }
  else
  {
    qWarning( QString( "Error al next en buscar dueño de caravana.\n Error: %1\n cola: %2" ).arg( colas.lastError().text() ).arg( colas.lastQuery() ).toLocal8Bit() );
    return false;
  }
 }
 else
 {
  qWarning( QString( "Error al buscar dueño de caravana.\n Error: %1\n cola: %2" ).arg( colas.lastError().text() ).arg( colas.lastQuery() ).toLocal8Bit() );
  return false;
 }
 // Escribo el dueño nuevo
 if( colas.exec( QString( "INSERT INTO car_carv_duenos( id_caravana, id_cliente, fecha_inicio, fecha_fin ) VALUES ( '%1', '%2', '%3', null )" ).arg( id_caravana ).arg( id_cliente ).arg( fecha.toString( Qt::ISODate ) ) ) )
 {
  return true;
 }
 else
 {
  qWarning( QString( "Error al insertar dueño de caravana.\n Error: %1\n cola: %2" ).arg( colas.lastError().text() ).arg( colas.lastQuery() ).toLocal8Bit() );
  return false;
 }
}


/*!
    \fn EMovimiento::getIDCaravana( QString codigo )
	Conversor de codigo a id de base de datos
	@param codigo codigo de la caravana
	@return id de el registro con ese codigo
 */
int EMovimiento::getIDCaravana( QString codigo )
{
  QSqlQuery cola;
 if( !cola.exec( QString( "SELECT id_caravana FROM car_caravana WHERE codigo = '%1' LIMIT 1" ).arg( codigo ) ) )
 {
	qWarning( QString( "Error al buscar el id de caravana\n Error: %1\n cola: %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
	return -1;
 }
 else
 {
	if( cola.next() )
	{
		return cola.record().value(0).toInt();
	}
	else
	{
		qWarning( QString( "Error al next de buscar numero de caravanab\n Error: %1\n cola: %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
		return -1;
	}
 }
}


void EMovimiento::setNumGuia( const QString numeroGuia )
{ _numGuia = numeroGuia; }

int EMovimiento::getTri() const
{ return id_db; }

QPair< int, QString > EMovimiento::getVendedor() const
{ return vendedor; }

QPair< int, QString > EMovimiento::getComprador() const
{ return comprador; }

QPair< int, QString > EMovimiento::getEstablecimientoOrigen() const
{ return origen; }

QPair< int, QString > EMovimiento::getEstablecimientoDestino() const
{ return destino; }

QString EMovimiento::getNumeroGuia() const
{ return _numGuia; }

QPair< int, QString > EMovimiento::getCategoria() const
{ return categoria; }

int EMovimiento::getCantidadAnimales() const
{ return _cantidad_animales; }

void EMovimiento::setCantidadAnimales ( int theValue )
{ _cantidad_animales = theValue; }


/*!
    \fn EMovimiento::iniciarTransaccion()
	Inicia una transaccion directamente en la base de datos ya que no estamos trabajando con un modelo.
	De producirse un error, guarda los detalles en la salida de debug del programa
	@return Verdadero o falso segun salio la operacion
 */
bool EMovimiento::iniciarTransaccion()
{
 if( QSqlDatabase::database().transaction() )
 {
  return true;
 }
 else
 {
  qWarning( "No se pudo iniciar el guardado de los datos en la base de datos. Consulte la salida del programa para mas detalles del error" );
  qDebug( QString( "Error de transaccion: %1" ).arg( QSqlDatabase::database().lastError().text() ).toLocal8Bit() );
  return false;
 }
}


/*!
    \fn EMovimiento::hacerCommit()
	Inicia el commit de la transaccion en la base de datos ya que no estamos trabajando con un modelo.
	De producirse un error, guarda los detalles en la salida de debug del programa
	@return verdadero o false segurn corresponda a la operacion
 */
bool EMovimiento::hacerCommit()
{
 if( QSqlDatabase::database().commit() )
 {
  return true;
 }
 else
 {
  qWarning( "No se pudo guardar los datos en la base de datos. Consulte la salida del programa para mas detalles del error" );
  qDebug( QString( "Error de commit: %1" ).arg( QSqlDatabase::database().lastError().text() ).toLocal8Bit() );
  return false;
 }
}


/*!
    \fn EMovimiento::deshacerCambiosDb()
 */
bool EMovimiento::deshacerCambiosDb()
{
 if( QSqlDatabase::database().rollback() )
 {
   return true;
   qDebug( "Echo un rollback de la base de datos" );
 }
 else
 {
  qWarning( "No se pudo deshacer el guardado de los datos en la base de datos. Consulte la salida del programa para mas detalles del error" );
  qDebug( QString( "Error de commit: %1" ).arg( QSqlDatabase::database().lastError().text() ).toLocal8Bit() );
  return false;
 }
}


/*!
    \fn EMovimiento::borrarCaravana( QString codigo, bool disasociar = true )
	Elimina efectivamente una caravana de la base de datos y elimina todas las asocaciaciones que hay en la db
	@param codigo Codigo de la caravana
	@param disasociar Elimina las relaciones en las que se haya la caravana respecto al tri
 */
bool EMovimiento::borrarCaravana( QString codigo, bool disasociar )
{
 bool estado = false;
 ///@todo controlar esto!
 int id_caravana = getIDCaravana( codigo );
 QSqlQuery *bcola = new QSqlQuery();
 if( bcola->exec( QString( "DELETE FROM car_carv_tri WHERE id_caravana = '%1' AND id_tri = '%1'" ).arg( id_caravana ).arg( id_db ) ) )
 {
  if( bcola->exec( QString( "DELETE FORM car_caravana WHERE id_caravana = '%1'" ).arg( id_caravana ) ) )
  {
	estado = true;
  }
  else
  {
	qDebug( " Error al eliminar la caravana de su tabbla " );
	estado = false;
  }
 }
 else
 {
  qDebug( "Error al hacer eliminacion de las relaciones de la caravana" );
  estado = false;
 }
 delete bcola;
 return estado;
}


/*!
    \fn EMovimiento::eliminarTRI( const int idDB )
	Elimina el tri que se indica, eliminado las relaciones con las caravanas, pero no elimina ninguna caravana.
	@param idDB Identificador de TRI
 */
bool EMovimiento::eliminarTRI( const int idDB )
{
 bool estado = true;
 QSqlQuery *cola = new QSqlQuery();
 if( !iniciarTransaccion() )
 { delete cola; return false; }
 // Elimino todas las relaciones caravana-tri donde este involucrado este tri
 if( !cola->exec( QString( "DELETE FROM car_carv_tri WHERE id_tri = '%1'" ).arg( idDB ) ) )
 {
  estado = false;
  qDebug( "Error al eliminar relaciones de un tri" );
  deshacerCambiosDb();
 }
 if( !cola->exec( QString( "DELETE FROM car_tri WHERE id_tri = '%1'" ).arg( idDB ) ) )
 {
  estado = false;
  qDebug( "Error al eliminar el tri" );
  deshacerCambiosDb();
 }
 if( !hacerCommit() )
 {
  estado = false;
 }
 delete cola;
 return estado;
}
