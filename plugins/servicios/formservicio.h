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

#ifndef FORMSERVICIO_H
#define FORMSERVICIO_H

#include "eventana.h"
#include "ui_formServicioBase.h"
class MServicios;
class QDataWidgetMapper;

class FormServicio : public EVentana, private Ui::FormServicioBase
{
Q_OBJECT

public:
    FormServicio( MServicios *m, QWidget* parent = 0, Qt::WFlags fl = 0 );
    void setearId( const int id_servicio, const QModelIndex indice = QModelIndex() );

protected slots:
    void guardar();
    void agregarRecargo();
    void cambiarBaja( bool estado );

signals:
    void actualizarVista();

private:
    MServicios *modelo;
    bool _modificando;
    int _id_servicio;
    QDataWidgetMapper *_mapa;
};

#endif
