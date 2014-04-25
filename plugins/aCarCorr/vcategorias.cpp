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
#include "vcategorias.h"

#include "mcategoria.h"
#include <QTableView>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

VCategorias::VCategorias( QWidget *parent )
: EVLista( parent, this )
{
 setAttribute( Qt::WA_DeleteOnClose );
 setObjectName( "car_categorias" );
 modelo = new MCategoria(this);
 vista->setModel( modelo );
 vista->hideColumn( 0 );
 vista->hideColumn( 2 );
 modelo->select();

 addAction( ActAgregar );
 addAction( ActEliminar );
 addAction( ActCerrar );

 this->setWindowTitle( "Listado de Categorias" );
 this->setWindowIcon( QIcon( ":/imagenes/categoria.png" ) );
}


VCategorias::~VCategorias()
{
}


void VCategorias::antes_de_insertar(int row, QSqlRecord& record)
{
 record.value( 1 ).setValue( QString(" ") );
 record.remove( 0 );
 return;
}

void VCategorias::eliminar()
{
	// Verificar que no existan caravanas con esta categoria
	//Busco el id
	QItemSelectionModel *selectionModel = vista->selectionModel();
	QModelIndexList lista = selectionModel->selectedIndexes();
	if( lista.size() < 1 )
	{
	   QMessageBox::warning( this, "Seleccione una categoria",
                   "Por favor, seleccione una o varias categorias para eliminarlas para eliminar",
                   QMessageBox::Ok );
	   return;
	}
	QModelIndex indice;
	QStringList lista2;
	foreach( indice, lista )
	{ lista2.append( vista->model()->data( vista->model()->index( indice.row(), 0 ), Qt::EditRole ).toString() ); 	}
    QSqlQuery cola;
    // Verifico si no es especial
    if( cola.exec( QString( "SELECT especial FROM car_categorias WHERE id_categoria IN ( %1 )" ).arg( lista2.join( "," ) ) ) )
    {
	if( !cola.record().value(0).toBool() )
	{
		QMessageBox::warning( this, "No se puede eliminar", "No se puede eliminar la categoria seleccionada, es una categoria especial" );
		return;
	}
    }
    else
    {
	qWarning( QString( "Error al buscar si una categoria es especial\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
	return;
    }
    // Verifico si tiene caravanas
    if( cola.exec( QString( "SELECT COUNT(id_tri) FROM car_tri WHERE id_categoria IN ( %1 )" ).arg( lista2.join( "," ) ) ) )
    {
	if( cola.record().value(0).toInt() > 0 )
	{
		QMessageBox::warning( this, "No se puede eliminar", "No se puede eliminar la categoria seleccionada, ya que existen TRI que pertenecen a esta categoria." );
		return;
	}
	else
	{
		// No hay tris en esa categoria, entonces la elimino
		EVLista::eliminar();
	}
    }
    else
    {
	qWarning( QString( "Error al buscar la cantidad de tris que pertenecen a una categoria\n Error: %1\n %2" ).arg( cola.lastError().text() ).arg( cola.lastQuery() ).toLocal8Bit() );
	return;
    }

}



/*!
    \fn VCategorias::imprimir()
 */
void VCategorias::imprimir()
{
 qWarning( "No implementado" );
}


/*!
    \fn VCategorias::buscar()
 */
void VCategorias::buscar()
{
 qWarning( "No implementado" );
}
