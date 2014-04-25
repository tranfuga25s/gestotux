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

#include <QDebug>
/*
CREATE TABLE IF NOT EXISTS `categoria_gastos` (
  `id_categoria_gastos` int(11) NOT NULL AUTO_INCREMENT,
  `nombre` tinytext NOT NULL,
  `descripcion` text,
  PRIMARY KEY (`id_categoria_gastos`)
) ENGINE=InnoDB;
*/


#include "mcategoriasgastos.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

MCategoriasGastos::MCategoriasGastos(QObject *parent) :
    QSqlTableModel(parent)
{
   this->setTable( "categoria_gastos" );
   this->setHeaderData( 0, Qt::Horizontal, "#ID" );
   this->setHeaderData( 1, Qt::Horizontal, "Nombre" );
   this->setHeaderData( 2, Qt::Horizontal, "Descripcion" );
}

/*!
 * \fn MCategoriasGastos::vacio()
 * Devuelve verdadero si esta vacío la cantidad de categorías de gastos.
 * \return Verdadero si  esta vacio o error.
 */
bool MCategoriasGastos::vacio()
{
 QSqlQuery cola;
 if( cola.exec( "SELECT COUNT(id_categoria_gastos) FROM categoria_gastos" ) ) {
     if( cola.next() ) {
         if( cola.record().value(0).toInt() > 0 ) {
             return false;
         } else { return true; }
     } else {
         qDebug( "Gastos:MCategoriasGastos:Error al hacer next en el conteo de cantidad de items.");
         return true;
     }
 } else {
     qDebug( "Gastos:MCategoriasGastos:Error al hacer exec de la cola" );
     qDebug() << "Gastos:MCategoriasGastos:cola = " << cola.lastQuery();
     qDebug() << "Gastos:MCategoriasGastos:error= " << cola.lastError().text();
     return true;
 }
}

/*!
 * \fn MCategoriasGastos::tieneGastosAsociados( const int id_categoria )
 * Devuelve verdadero si la categoría de gastos tiene algun gasto declarado
 * \param id_categoria Identificador de la categoría
 * \return
 */
bool MCategoriasGastos::tieneGastosAsociados( const int id_categoria )
{
 QSqlQuery cola;
 if( cola.exec( QString( "SELECT COUNT(id_gasto) FROM gastos WHERE id_categoria = %1" ).arg( id_categoria ) ) ) {
     if( cola.next() ) {
         if( cola.record().value(0).toInt() > 0 ) {
             return true;
         } else { return false; }
     } else {
         qDebug( "Gastos:MCategoriasGastos:Error al hacer next en el conteo de cantidad de items de una cateogria.");
         return true;
     }
 } else {
     qDebug( "Gastos:MCategoriasGastos:Error al hacer exec de la cola" );
     qDebug() << "Gastos:MCategoriasGastos:cola = " << cola.lastQuery();
     qDebug() << "Gastos:MCategoriasGastos:error= " << cola.lastError().text();
     return true;
 }
}

/*!
 * \fn MCategoriasGastos::eliminarCategoria( const int id_categoria )
 * Devuelve verdadero si se pudo eliminar correctamente la categoria
 * \param id_categoria Identificador de la categoría
 * \return Verdadero si esta vacio o error.
 */
bool MCategoriasGastos::eliminarCategoria( const int id_categoria )
{
 QSqlQuery cola;
 if( cola.exec( QString( "DELETE FROM categoria_gastos WHERE id_categoria_gastos = %1" ).arg( id_categoria ) ) ) {
     return true;
 } else {
     qDebug( "Gastos:MCategoriasGastos:Error al hacer exec de la cola de eliminacion de categoria." );
     qDebug() << "Gastos:MCategoriasGastos:cola = " << cola.lastQuery();
     qDebug() << "Gastos:MCategoriasGastos:error= " << cola.lastError().text();
 }
 return false;
}
