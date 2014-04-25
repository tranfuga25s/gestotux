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
#include "vcategorias.h"

#include "mcategorias.h"
#include <QTableView>
#include <QSqlRecord>
#include <QMessageBox>
#include <QString>
#include <QModelIndex>
#include <QSqlQuery>
#include <QAction>
#include <QSqlError>

VCategorias::VCategorias( QWidget *parent )
: EVLista( parent )
{
 setObjectName( "ListaCategorias" );
 setWindowTitle( "Categorias de Productos" );
 setWindowIcon( QIcon( ":/imagenes/categorias.png" ) );

 modelo = new MCategorias( this );
 vista->setModel( modelo );
 modelo->setFilter( "id != 0" );
 modelo->select();

 vista->hideColumn( 0 );
 vista->resizeColumnsToContents();
 vista->setAlternatingRowColors( true );
 vista->setSelectionBehavior( QAbstractItemView::SelectRows );

 ActEliminar->disconnect( SIGNAL( triggered() ), this, SLOT( eliminar() ) );
 ActEliminar->connect( ActEliminar, SIGNAL( triggered() ), this, SLOT( eliminar() ) );

 addAction( ActAgregar );
 addAction( ActEliminar );
 addAction( ActCerrar );
}


/*!
    \fn VCategorias::eliminar()
        Metodo reimplementado para ver que cuando se intente eliminar una categoria, vea si existen productos
        en ella y evite eliminarlos para no dejarlos huerfanos
 */
void VCategorias::eliminar()
{
 //Preguntar al usuario si esta seguro
 QItemSelectionModel *selectionModel = vista->selectionModel();
 QModelIndexList indices = selectionModel->selectedRows();
 if( indices.size() < 1 )
 {
   QMessageBox::warning( this, "Seleccione un item",
                   "Por favor, seleccione un item para eliminar",
                   QMessageBox::Ok );
   return;
 }
 // Busco para esos indices a ver si existen valores de referencia en productos
 QModelIndex indice;
 QString lista;
 foreach( indice, indices )
 {
  lista.append( modelo->data( modelo->index( indice.row(), 0 ), Qt::EditRole ).toString() );
  lista.append(",");
 }
 if( lista.endsWith( "," ) )
 {
  lista.remove( lista.size() - 1, 1 );
 }
 // Veo si existen productos con esta categoria
 qDebug( QString("SELECT COUNT(id) FROM producto WHERE id_categoria IN ( %1 )" ).arg( lista ).toLocal8Bit() );
 QSqlQuery cola( QString("SELECT COUNT(id) FROM producto WHERE id_categoria IN ( %1 )" ).arg( lista ) );
 if( cola.next() )
 {
   int cantidad = cola.record().value(0).toInt();
   if( cantidad > 0 )
   {
        QMessageBox::warning( this, "No se puede eliminar",
                                QString( "Existen %1 producto(s) que pertenecen a esta categoria, no se podra eliminar" ).arg( cantidad ) );
        return;
   }
   else
   {
        int ret;
        ret = QMessageBox::warning( this, "Esta seguro?",
                        QString( "Esta seguro de eliminar %1 item?").arg( indices.size() ),
                        "Si", "No" );
        if ( ret == 0 )
        {
                QModelIndex indice;
                foreach( indice, indices )
                {
                        if( indice.isValid() )
                        {
                                modelo->removeRow( indice.row() );
                        }
                }
        }
   }
 }
 else
 {
  qDebug( "Error de ejecucion en la cola" );
  qDebug( QString( "Detalle: %1" ).arg( cola.lastError().text() ).toLocal8Bit() );
  return;
 }

}

#include <QInputDialog>
void VCategorias::agregar( bool /*autoeliminarid*/ )
{
    bool ok = false;
    QString nombre = QInputDialog::getText( this, "Agregar categoria de producto", "Nombre", QLineEdit::Normal, QString(), &ok );
    if( ok ) {
        // Verifico que no exista
        if( MCategorias::buscarRepetido( nombre ) ) {
            QMessageBox::critical( this, "Ya existe", QString::fromUtf8( "Una categoría con este nombre ya existe" ) );
            return;
        }
        bool ok2;
        QString descripcion = QInputDialog::getText( this, "Agregar categoria de producto", " Descripcion ( puede estar vacia)", QLineEdit::Normal, QString(), &ok2 );
        if( ! nombre.isEmpty() ) {
            QSqlRecord rec = modelo->record();
            rec.setValue( "nombre", nombre );
            if( descripcion.isEmpty() ) {
                rec.setNull( "descripcion" );
            } else {
                rec.setValue( "descripcion", descripcion );
            }
            if( modelo->insertRecord( -1, rec ) ) {
                QMessageBox::information( this, "Correcto", "La categoria se cargo correctamente" );
                return;
            } else {
                QMessageBox::warning( this, "error", "No se pudo agregar la categoria" );
                qDebug( modelo->lastError().text().toLocal8Bit() );
                return;
            }
        }
    }
}
