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
#ifndef VSERVICIOS_H
#define VSERVICIOS_H

#include <evlista.h>
#include <QModelIndex>
/**
Listado de Servicios

        @author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class VServicios : public EVLista
{
Q_OBJECT

public:
    VServicios( QWidget *parent = 0 );

protected slots:
    void agregar( bool autoeliminarid );
    void eliminar();
    void verClientesAdheridos();
    void verRecargos();
    void generarFacturacion();
    void darDeBaja();
    void verPeriodos();
    void actualizarVista();

private slots:
    void modificar( const QModelIndex &indice );
    void darAltaServicioCliente();
    void cambioVerBaja( bool estado );

protected:
    void menuContextual( const QModelIndex &indice, QMenu *menu );

protected slots:
    void modificar();

signals:
    void editarCliente( int );

private:
    QModelIndex indiceMenu;
    QAction *ActRecargos;
    QAction *ActGenerarFacturacion;
    QAction *ActVerClientes;
    QAction *ActNuevoCliente;
    QAction *ActDarDeBaja;
    QAction *ActVerPeriodosFacturados;
    QAction *ActVerDadosBaja;

};

#endif
