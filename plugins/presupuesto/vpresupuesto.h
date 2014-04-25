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
#ifndef VPRESUPUESTO_H
#define VPRESUPUESTO_H

#include <evlista.h>
#include <QDate>
class MProductosTotales;

/**
 * \brief Listado de presupuestos emitidos
 *
 * Vista para los presupuestos emitidos y sus correspondientes acciones
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class VPresupuesto : public EVLista
{
Q_OBJECT
public:
    VPresupuesto(QWidget *parent = 0);

protected slots:
    void agregar( bool autocompletar );
    void imprimir();
    void aPdf();
    void verContenido();
    void menuContextual( const QModelIndex &indice, QMenu *menu );
    void modificar();
    void eliminar();
    void aFactura();

signals:
    void emitirFactura( int, QDate, MProductosTotales * );

private:
    QModelIndex indiceMenu;
    QAction *ActVerContenido;
    QAction *ActAFactura;

};

#endif
