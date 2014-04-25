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
#include "mcaravanadueno.h"
#include <QStringList>

MCaravanaDueno::MCaravanaDueno(QObject *parent)
 : QAbstractTableModel(parent)
{
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Codigo de Caravana" );
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
 setHeaderData( 2, Qt::Horizontal, "Dueño" );
#endif
}


MCaravanaDueno::~MCaravanaDueno()
{
}


bool MCaravanaDueno::setData( const QModelIndex& index, const QVariant& value, int role )
{
 if( !index.isValid() )
 {
  return false;
 }
 switch( index.column() )
 {
	case 0:
	{
		if( role == Qt::EditRole )
		{
			return false;
		}
		else
		{
			return false;
		}
		break;
	}
	// Codigo de la caravana
	case 1:
	{
		if( role == Qt::EditRole )
		{
			datos[index.row()].first = value.toString();
			emit dataChanged( index, index );
			return true;
		}
		else
		{
			return false;
		}
		break;
	}
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
	//Dueño
//#warning "Edicion de columna de dueno"
	case 2:
	{
		if( role == Qt::EditRole )
		{
			datos[index.row()].second = value.toString();
			emit dataChanged( index, index );
			return true;
		}
		else
		{
			return false;
		}
		break;
	}
#endif
 	default:
	{
		return false;
		break;
	}
 }
}

bool MCaravanaDueno::removeRow( int row, const QModelIndex& parent )
{
 beginRemoveRows( parent, row, row );
 datos.remove( row );
 endRemoveRows();
 return true;
}

bool MCaravanaDueno::insertRow( int row, const QModelIndex& parent )
{
 beginInsertRows( parent, rowCount(), rowCount() );
 datos.insert( rowCount(), QPair<QString,QString>() );
 endInsertRows();
 return true;
}

Qt::ItemFlags MCaravanaDueno::flags( const QModelIndex& index ) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant MCaravanaDueno::data( const QModelIndex& index, int role ) const
{
 switch( index.column() )
 {
	case 0:
	{
		switch( role )
		{
			case Qt::DisplayRole:
			{
				return index.row();
				break;
			}
			default:
			{
				return QVariant();
				break;
			}
		}
		break;
	}
	// Codigo de la caravana
	case 1:
	{
		switch( role )
		{
			case Qt::DisplayRole:
			{
				return datos.value( index.row() ).first;
				break;
			}
			case Qt::EditRole:
			{
				return datos.value( index.row() ).first;
				break;
			}
			default:
			{
				return QVariant();
				break;
			}
		}
		break;
	}
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
//#warning "Mostrando columna de dueno"
	case 2:
	{
		switch( role )
		{
			case Qt::DisplayRole:
			{
				return datos.value( index.row() ).second;
				break;
			}
			case Qt::EditRole:
			{
				return datos.value( index.row() ).second;
				break;
			}
			default:
			{
				return QVariant();
				break;
			}
		}
		break;
	}
#endif
 	default:
	{
		return QVariant();
		break;
	}
 }
}

QModelIndex MCaravanaDueno::index( int row, int column, const QModelIndex &index ) const
{
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
 if( row < rowCount()  && column < 3 )
 {
#else
 if( row < rowCount() )
 {
#endif
  return createIndex( row, column, row );
 }
 else
 { return QModelIndex(); }
}


int MCaravanaDueno::columnCount( const QModelIndex & parent ) const
{
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
//#warning "columnas=3"
return 3;
#else
//#warning "columnas=2"
return 2;
#endif
}

int MCaravanaDueno::rowCount ( const QModelIndex & parent ) const
{
 return datos.size();
}


/*!
    \fn MCaravanaDueno::listaCaravanas()
 */
QStringList MCaravanaDueno::listaCaravanas()
{
 QStringList l;
 for( int i = 0; i < rowCount(); i++ )
 {
  l.append( datos[i].first );
 }
 return l;
}


/*!
    \fn MCaravanaDueno::verificarAgregar( const QString &codigo, const QString &dueno )
	Verifica si existe el codigo de caravana y el dueño y si no existe lo agrega a la lista
	@param codigo Codigo de la caravana
	@param dueno Dueño de la caravana
	@return verdadero si fue exitoso
 */
bool MCaravanaDueno::verificarAgregar( const QString &codigo, const QString &dueno )
{
 if( listaCaravanas().contains( codigo ) )
 {
  return false;
 }
 int anterior = rowCount();
 this->insertRow( -1 );
 this->setData( this->index( anterior, 1 ), codigo );
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
 this->setData( this->index( anterior, 2 ), dueno );
#endif
 return true;
}


/*!
    \fn MCaravanaDueno::verificarAgregar( const QStringList &listado, const QString &dueno )
	Verifica si existe la lista de caravanas con el dueño especificado
	@param listado Lista de strings con los codigos de caravanas
	@param dueno Dueño de las caravanas
	@return verdadero si se agregaron todas, falso si se agregaron solo algunas
 */
bool MCaravanaDueno::verificarAgregar( const QStringList &listado, const QString &dueno )
{
 QString temp;
 foreach( temp, listado )
 {
  if( !verificarAgregar( temp, dueno ) )
  {
	///@todo verificar esto que puede retornar antes de tiempo
    return false;
  }
 }
 return true;
}


/*!
    \fn MCaravanaDueno::setDuenosTodos( const QString &dueno )
 */
void MCaravanaDueno::setDuenosTodos( const QString &dueno )
{
///@todo implementar
}


/*!
    \fn MCaravanaDueno:::headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const
	Devuelve las cabeceras de las columnas usadas por el modelo
	@param section numero de sección
	@param orientation orientación de la seccion
	@param role Rol que se desea usar
	@return QVariant con el dato
 */
QVariant MCaravanaDueno::headerData( int section, Qt::Orientation orientation, int role ) const
{
 if( orientation == Qt::Horizontal )
 {
  switch( section )
  {
	case 1:
	{
		if( role == Qt::DisplayRole )
		{
			return "Codigo";
		}
		else
		{
			return QAbstractTableModel::headerData( section, orientation, role );
		}
		break;
	}
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
#warning "cabecera"
	case 2:
	{
		if( role == Qt::DisplayRole )
		{
			return "Dueño";
		}
		else
		{
			return QAbstractTableModel::headerData( section, orientation, role );
		}
		break;
	}
#endif
 	default:
	{
		return QAbstractTableModel::headerData( section, orientation, role );
		break;
	}
  }
 }
 else
 {
  return QAbstractTableModel::headerData( section, orientation, role );
 }
}


#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
/*!
    \fn MCaravanaDueno::cargarCaravanasTri( int id_tri )
	Metodo que carga cada caravana que corresponde al tri en su propia estructura y le coloca el dueño correspondiente
 */
bool MCaravanaDueno::cargarCaravanasTri( int id_tri )
{
 // Busco todas las caravanas que tiene ese tri
 QSqlQuery cola;
 cola.prepare( QString( "SELECT id_caravana FROM car_carv_tri WHERE id_tri = '%1'" ).arg( id_tri ) );
 if( cola.exec() )
 {
  while( cola.next() )
  {
   // por cada caravana busco el codigo y su dueño
   QSqlQuery cola1;
   if( cola1.exec( QString( "SELECT codigo FROM car_caravana WHERE id_caravana = '%1'" ).arg( cola.record().value(0).toInt() ) ) )
   {
	if( cola1.next() )
	{
		int anterior = rowCount();
		this->insertRow( -1 );
 		this->setData( this->index( anterior, 1 ), cola1.record().value(0).toString() );
#ifdef GESTOTUX_CARAVANAS_TIENEN_DUENOS
		QSqlQuery cola2;
		if( QSqlDatabase::database().driverName() == "QSQLITE" )
		{
			cola2->prepare(QString( "SELECT apellido || ', ' || nombre FROM clientes WHERE id IN  ( SELECT id_cliente FROM car_carv_duenos WHERE id_caravana = '%1' )" ).arg( cola.record().value(0).toInt() ) )
		}
		else if( QSqlDatabase::database().driverName() == "QMYSQL" )
		{
			cola2->prepare( QString( "SELECT CONCAT( CONCAT( apellido, \", \" ), nombre ) FROM clientes WHERE id IN  ( SELECT id_cliente FROM car_carv_duenos WHERE id_caravana = '%1' )" ).arg( cola.record().value(0).toInt() ) )
		}
		if( cola2.exec() )
		{
			if( cola2.next() )
			{
				this->setData( this->index( anterior, 1 ), cola2.record().value(0).toString() );
			}
			else
			{
				qWarning( "Error al ejecutar next de la cola de nombre cliente en MCaravanaDueno" );
			}
		}
		else
		{
			qWarning( "Error al ejecutar la cola de nombre de dueño en MCaravanaDueno" );
		}
#endif
	}
	else
	{
                qDebug( QString( "Error al ejecutar next de: %1, error: %2" ).arg( cola1.lastQuery() ).arg( cola1.lastError().text() ).toLocal8Bit() );
	}
   }
   else
   {
    qDebug( QString( "Error al ejecutar la cola: %1, error: %2" ).arg( cola1.lastQuery() ).arg( cola1.lastError().text() ).toLocal8Bit() );
    return false;
   }
  } // Fin de cada caravana en este tri
 }
 else
 {
  qDebug( QString( "Error al ejecutar la cola: %1, error: %2" ).arg( cola.lastQuery() ).arg( cola.lastError().text() ).toLocal8Bit() );
  return false;
 }
 return true;
}
