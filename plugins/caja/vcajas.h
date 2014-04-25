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

#ifndef VCAJAS_H
#define VCAJAS_H

#include "evlista.h"

class VCajas : public EVLista
{
    Q_OBJECT
public:
    VCajas(QWidget *parent = 0);

public slots:
    void ingreso();
    void retiro();
    void transferir();

protected slots:
    void agregar( bool autoeliminarid  );
    void eliminar();
    void resumen();
    void resumen(  const QModelIndex & ) { resumen(); }
    void menuContextual(const QModelIndex &indice, QMenu *menu);
    void cambiarNombre();

private:
    QAction *ActTransferire;
    QAction *ActIngreso;
    QAction *ActEgreso;
    QAction *ActResumen;
    QAction *ActCambiarNombre;

};

#endif // VCAJAS_H
