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

#ifndef FORMFACTURARSERVICIO_H
#define FORMFACTURARSERVICIO_H

#include "ui_FormFacturarServicioBase.h"
#include "eventana.h"
class QAction;
class EActCerrar;
#include <QDate>

/*!
 * \brief Formulario de facturacion de un servicio
 *
 * Formulario que carga los datos y permite realizar la facturación de un servicio en un periodo.
 * \author Esteban Zeller
 */
class FormFacturarServicio : public EVentana, private Ui::FormFacturarServicio
{
    Q_OBJECT

public:
    explicit FormFacturarServicio( QWidget *parent = 0 );
    explicit FormFacturarServicio( int id_servicio );
    void setearServicio( const int id_servicio = 0 );

protected:
    void changeEvent(QEvent *e);


protected slots:
    void facturar();
    void cancelar() { _cancelar = true; }

private:
    int _id_servicio;
    int _periodo;
    int _ano;
    double _precio_base;
    QDate _fecha_inicio;
    QDate _fecha_emision;
    EActCerrar *ActCerrar;
    QAction *ActFacturar;

    bool _cancelar;

private slots:

    void cargar_datos_servicio();

};

#endif // FORMFACTURARSERVICIO_H
