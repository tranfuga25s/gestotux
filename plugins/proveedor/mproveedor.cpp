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
#include "mproveedor.h"
#include <QColor>
#include <QDebug>
#include "preferencias.h"

MProveedor::MProveedor(QObject *parent)
 : QSqlTableModel(parent)
{
 setTable( "proveedor" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Nombre" );
 setHeaderData( 2, Qt::Horizontal, "Direccion" );
 setHeaderData( 3, Qt::Horizontal, "Em@il" );
 setHeaderData( 4, Qt::Horizontal, "Fax" );
 setHeaderData( 5, Qt::Horizontal, "Telefono Celular" );
 setHeaderData( 6, Qt::Horizontal, "Telefono Linea" );
 setHeaderData( 7, Qt::Horizontal, "CUIT/CUIL" );
}

QVariant MProveedor::data(const QModelIndex& idx, int role) const
{
 if( !idx.isValid() )
 { return QVariant(); }
 switch( role )
 {
        case Qt::DisplayRole:
        {
         switch ( idx.column() )
         {
                 case 7:
                 {
                         QString ret = QSqlTableModel::data( idx, role ).toString();
                         if( ret.isEmpty() || ret.length() == 2 ) {
                             return QVariant();
                         } else {
                             return ret.insert( 2, '-' ).insert( ret.length(), '-' );
                         }
                         break;
                 }
                 default:
                 {
                         return QSqlTableModel::data( idx, role );
                         break;
                 }
         }
         break;
        }
	case Qt::TextColorRole:
	{
		switch ( idx.column() )
		{
			case 4:
			{
			        preferencias *p = preferencias::getInstancia();
                                p->beginGroup( "General" );
                                QString color = p->value( "color_mail", "#0000FF" ).toString();
                                p->endGroup(); p=0;
                                return QColor( color );
        			break;
			}
			default:
			{
				return QColor(Qt::black);
				break;
			}
		}
		break;
	}
	case Qt::TextAlignmentRole:
	{
		switch ( idx.column() )
		{
			case 0:
			case 4:
			{
				return int( Qt::AlignHCenter | Qt::AlignVCenter );
				break;
			}
			case 5:
			case 6:
                        case 7:
			{
				return int( Qt::AlignRight | Qt::AlignVCenter );
				break;
			}
			default:
			{
				return int( Qt::AlignLeft | Qt::AlignVCenter );
				break;
			}
		}
		break;
	}
	case Qt::ToolTipRole:
	case Qt::StatusTipRole:
	{
		return QVariant( "Haga doble click o seleccione y F2 para editar" );
		break;
	}
	default:
	{
		return QSqlTableModel::data( idx, role);
		break;
	}
 }
 return QVariant();
}

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

/*!
 * \fn MProveedor::tieneDatosRelacionados( const int id_proveedor )
 * Devuelve verdadero si el ID del proveedor posee algun dato relacionado
 * \param id_proveedor Identificador del proveedor
 * \returns Verdadero si existe algun dato relacionado - Falso en caso contrario ( es posible su eliminacion )
 */
bool MProveedor::tieneDatosRelacionados( const int id_proveedor )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT(id_proveedor) FROM compras WHERE id_proveedor = %1" ).arg( id_proveedor ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() <= 0 ) {
                return false;
            }
        }
    } else {
        qWarning( "Existio un error al intentar obtener los datos relacionados con este proveedor. No se podrá eliminarlo." );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return true;
}

/*!
 * \fn MProveedor::existenProveedores()
 * Devuelve verdadero si existe al menos un proveedor declarado en la base de datos
 */
bool MProveedor::existenProveedores()
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT count(id) FROM proveedor" ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            }
        } else {
            qDebug( "Error al hacer next en la cola de averiguación de cantidad de proveedores." );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else {
        qWarning() << QString::fromUtf8( "Existió un error al intentar ejecutar la consulta de la cantidad de proveedores" );
        qDebug() << "Error al hacer exec en la cola de averiguación de cantidad de proveedores.";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MProveedor::existeProveedor()
 * Devuelve verdadero si existe al menos un proveedor declarado con el mismo nombre
 * \param razon_social Razon social a verificar su existencia
 */
bool MProveedor::existeProveedor( QString razon_social )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT count(id) FROM proveedor WHERE nombre = '%1'" ).arg( razon_social ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            }
        } else {
            qDebug() << QString::fromUtf8("Error al hacer next en la cola de averiguación de proveedor segun nombre." );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else {
        qWarning() << QString::fromUtf8( "Existió un error al intentar ejecutar la consulta de existencia de proveedor" );
        qDebug() << "Error al hacer exec en la cola de averiguación de existencia de proveedor.";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MProveedor::existeCuitCuil( QString cuit )
 * Devuelve verdadero si existe al menos un proveedor declarado con el mismo numero de CUIT o CUIL
 * \param cuit CUIT o CUIL a verificar su existencia
 */
bool MProveedor::existeCuitCuil( QString cuit )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT count( id ) FROM proveedor WHERE cuit_cuil = '%1'" ).arg( cuit ) ) ) {
        if( cola.next() ) {
            if( cola.record().value(0).toInt() > 0 ) {
                return true;
            }
        } else {
            qDebug() << QString::fromUtf8("Error al hacer next en la cola de averiguación de proveedor segun CUIT/CUIL." );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else {
        qWarning() << QString::fromUtf8( "Existió un error al intentar ejecutar la consulta de existencia de proveedor por CUIT/CUIL" );
        qDebug() << "Error al hacer exec en la cola de averiguación de existencia de proveedor por CUIT/CUIL.";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}
