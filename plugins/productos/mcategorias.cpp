/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller               		   *
 *   juiraze@yahoo.com.ar                       			   *
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
#include "mcategorias.h"

#include <QSqlIndex>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

MCategorias::MCategorias(QObject *parent)
 : QSqlTableModel(parent)
{
 setTable( "categoria_producto" );
 setHeaderData( 0, Qt::Horizontal, "#ID" );
 setHeaderData( 1, Qt::Horizontal, "Nombre" );
 setHeaderData( 2, Qt::Horizontal, "Descripcion" );
 setSort( 1, Qt::AscendingOrder );
 setEditStrategy( QSqlTableModel::OnRowChange );
 select();
}

/*!
  \fn MCategorias::buscarRepetido( const QString nombre )
  Busca si existe alguna categoria con el mismo nombre
 */
bool MCategorias::buscarRepetido(const QString nombre)
{
  return false;
  QSqlQuery cola;
  if( cola.exec( QString( "SELECT count(nombre) FROM categoria_producto WHERE nombre LIKE '%1'" ).arg( nombre ) ) ) {
      if( cola.next() ) {
          if( cola.record().value(0).toInt() > 1 ) {
              return false;
          } else {
              return true;
          }
      } else {
          qWarning( "Error al hacer next" );
          qDebug( "Error al hacer next en la cola de averiguación de si existe una categoría ya." );
          qDebug( cola.lastError().text().toLocal8Bit() );
          return false;
      }
  } else {
      qWarning( "Error de ejecucion de cola" );
      qDebug( "Error al hacer exec en la cola de averiguacion de si existe una cateogria ya." );
      qDebug( cola.lastError().text().toLocal8Bit() );
      return false;
  }
  return false;
}

/*!
 * \brief MCategorias::getListado
 * Obtiene el listado de categorías para un QInputDialog::getItem()
 * \return Listado de categorías
 */
QStringList MCategorias::getListado()
{
    QStringList retorno;
    QSqlQuery cola;
    if( cola.exec( "SELECT id, nombre FROM categoria_producto" ) ) {
        while( cola.next() ) {
            retorno.append( QString( "%1 ~ %2" )
                            .arg( cola.record().value(0).toInt() )
                            .arg( cola.record().value(1).toString() ) );
        }
    } else {
        qWarning()  << "Error de ejecucion de cola";
        qDebug() << "Error al hacer exec en la cola de listado de categorias.";
        qDebug() << cola.lastError().text();
    }
    return retorno;
}
