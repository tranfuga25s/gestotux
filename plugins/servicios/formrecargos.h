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

#ifndef FORMRECARGOS_H
#define FORMRECARGOS_H

#include "eventana.h"
#include "ui_FormRecargoBase.h"
class QAction;
class MServicios;
class MRecargos;

/*!
 * \brief Formulario de administracion de recargos de servicios
 *
 *
 * \author Esteban Zeller
 */
class FormRecargos : public EVentana, private Ui::FormRecargoBase
{
    Q_OBJECT

public:
    FormRecargos( QWidget *parent = 0, Qt::WFlags fl = 0 );
    void setearId( const int id );

protected:
    void changeEvent(QEvent *e);

protected slots:
    void cambioServicio( int servicio );
    void agregarRecargo();
    void eliminarRecargo();

private:
    QAction *ActAgregar;
    QAction *ActEliminar;
    QAction *ActCerrar;

    MRecargos *mrecargos;
    MServicios *mservicios;
};

#endif // FORMRECARGOS_H
