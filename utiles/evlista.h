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
#ifndef EVLISTA_H
#define EVLISTA_H

#include "eventana.h"
class QSqlRelationalTableModel;
class QSqlTableModel;
class QSqlRecord;
#include <QAction>
class QTableView;
class QMenu;
class EBusqueda;
#include <QModelIndex>

/**
 * 	\brief Clase base para todas las ventanas de tipo lista
 *
 *	Clase generica para mostrar un listado simple de datos del programa
 *
 * 	@author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class EVLista : public EVentana
{
	Q_OBJECT
public:
    EVLista(QWidget *parent = 0, Qt::WFlags fl = 0 );
    ~EVLista();
    void closeEvent( QCloseEvent * c);

   /*!
    * Visor utilizado para ver los datos
    */
    QTableView *vista;

    // Acciones
   /*!
    * Accion generica de agregar
    */
    QAction *ActAgregar;
   /*!
    * Accion generica de modificar
    */
    QAction *ActModificar;
   /*!
    * Accion generica de eliminar
    */
    QAction *ActEliminar;
   /*!
    * Accion generica de buscar
    */
    QAction *ActBuscar;
   /*!
    * Accion generica de Imprimir
    */
    QAction *ActImprimir;
   /*!
    * Accion generica de cerrar
    */
    QAction *ActCerrar;
   /*!
    * Accion generica para guardar a pdf
    */
    QAction *ActPdf;
    /*!
     * Accion generica para mostrar todos los registros
     */
     QAction *ActVerTodos;
    /*!
     * \fn EVLista::habilitarBusqueda()
     * Habilita la aparicion de la accion y la ventana de filtrado y busqueda
     */
     void habilitarBusqueda() { _busquedaHabilitada = true; }
     void agregarFiltroBusqueda( const QString nombre, const QString filtro );

public slots:
     void cerroBusqueda();

protected:
  /**
   * Puntero a modelo generico para la vista
   */
   QSqlRelationalTableModel *rmodelo;
   QSqlTableModel *modelo;

protected slots:
    virtual void cerrar();
    virtual void agregar( bool autoeliminarid = true );
    virtual void eliminar();
    virtual void antes_de_insertar( int row, QSqlRecord & record );
    virtual void imprimir();
    virtual void buscar();
    virtual void modificar();
    virtual void aPdf();
    virtual void menuContextual( const QModelIndex &indice, QMenu *menu );
    virtual void hacerMenuContextual( const QModelIndex &indice );
    virtual void mostrarTodos();

private:
  /*!
   * Determina si esta habilitada o no la busqueda
   */
   bool _busquedaHabilitada;
  /*!
   * Puntero para la ventana de busqueda
   */
   EBusqueda *dockBusqueda;
  /*!
   * Lista de filtros de busqueda personalizados
   */
   QList< QPair<QString,QString> > filtros;



};

#endif
