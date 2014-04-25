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

#include "MItemPresupuesto.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

MItemPresupuesto::MItemPresupuesto(QObject *parent) :
    QSqlRelationalTableModel(parent) {
    inicializar();
    relacionar();
}


void MItemPresupuesto::inicializar() {
    setTable( "item_presupuesto" );
    setHeaderData( 0, Qt::Horizontal, "#Orden" );
    setHeaderData( 1, Qt::Horizontal, "#Presupuesto" );
    setHeaderData( 2, Qt::Horizontal, "Cantidad" );
    setHeaderData( 3, Qt::Horizontal, "Texto" );
    setHeaderData( 4, Qt::Horizontal, "Precio Unitario" );
    setHeaderData( 5, Qt::Horizontal, "#Producto" );

}

void MItemPresupuesto::relacionar() {}

/*!
 * \fn MItemPresupuesto::agregarItemPresupuesto( const int id_presupuesto, const double cantidad, const QString texto, const double precio_unitario )
 * Agrega un item de presupuesto con el presupuesto relacionado pasado como parametro
 * \param id_presupuesto Identificador del Presupuesto
 * \param cantidad Cantidad del item
 * \param texto Texto del item
 * \param precio_unitario Precio unitario del item
 * \returns Verdadero si se pudo guardar correctamente
 */
bool MItemPresupuesto::agregarItemPresupuesto( const int id_presupuesto, const double cantidad, const QString texto, const double precio_unitario ) {
 QSqlQuery cola;
 if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QSQLITE" ) {
     if( cola.exec( QString( "SELECT COUNT(id_presupuesto) FROM presupuestos WHERE id_presupuesto = %1" ).arg( id_presupuesto ) ) ) {
         if( cola.next() ) {
             if( cola.record().value(0).toInt() <= 0 ) {
                 qDebug( "Conteo de cantidad de presupuestos con el identificador pasado fue menor o igual que uno" );
                 return false;
             }
         } else {
             qDebug( "Error al intentar hacer next de la cola de verificacion de si existe el presupuesto" );
             return false;
         }
     } else {
          qDebug( "Error de exec al intentar ejecutar la cola de verificacion de si existe el presupeusto" );
          qDebug( cola.lastQuery().toLocal8Bit() );
          qDebug( cola.lastError().text().toLocal8Bit() );
          return false;
     }
 }
 if( !cola.prepare( "INSERT INTO item_presupuesto( id_presupuesto, cantidad, texto, precio_unitario ) VALUES ( :id_presupuesto, :cantidad, :texto, :precio_unitario );" ) ) {
     qDebug( "Error al intentar preparar la cola de inserción" );
     qDebug( QString( "Error: %1 - %2" ).arg( cola.lastError().number() ).arg( cola.lastError().text() ).toLocal8Bit() );
 }
 cola.bindValue( ":id_presupuesto", id_presupuesto );
 cola.bindValue( ":cantidad", cantidad );
 cola.bindValue( ":texto", texto );
 cola.bindValue( ":precio_unitario", precio_unitario );
 if( cola.exec() ) {
     return true;
 } else {
     qDebug( "Error al intentar insertar valor de item de presupuesto" );
     qDebug( QString( "Error: %1 - %2 - %3" ).arg( cola.lastError().number() ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
     return false;
 }

}

void MItemPresupuesto::setearId(const int id_presupuesto)
{
    this->setFilter( QString( " id_presupuesto = %1 " ).arg( id_presupuesto ) );
}

/*!
 * \fn MItemPresupuesto::eliminarItemsDePresupuesto( const int id_presupuesto )
 * Elimina todos los items de un presupuesto pasado como parametro
 * \param id_presupuesto Identificador del presupuesto al que se le desean eliminar los items
 * \returns Verdadero si se pudo eliminar los items
 */
bool MItemPresupuesto::eliminarItemsDePresupuesto(const int id_presupuesto)
{
    QSqlQuery cola;
    if( cola.exec( QString( "DELETE FROM item_presupuesto WHERE id_presupuesto = %1" ).arg( id_presupuesto ) ) ) {
        return true;
    } else {
        qDebug( "Error de exec al intentar ejecutar la cola de eliminacion de items del presupeusto" );
        qDebug( cola.lastQuery().toLocal8Bit() );
        qDebug( cola.lastError().text().toLocal8Bit() );
        return false;
    }
}
