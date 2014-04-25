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
#ifndef MPRESUPUESTO_H
#define MPRESUPUESTO_H

#include <QSqlRelationalTableModel>
#include "NumeroComprobante.h"

/**
 * \brief Modelo de la tabla de recibos
 *
 * Modelo que permite administrar los datos de los presupuestos.
 *
 * \author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MPresupuesto : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MPresupuesto( QObject *parent = 0 );

    static NumeroComprobante &proximoComprobante();
    int agregarPresupuesto( int id_cliente, QString texto_cliente, QString direccion, QDateTime fechahora, double total, const QString observaciones = QString() );
    bool modificarPresupuesto( int id_presupuesto, int id_cliente, QString texto_cliente, QString direccion, QDateTime fechahora, double total, const QString observaciones = QString() );

    static bool eliminarPresupuesto( int id_presupuesto );

    static int obtenerIdCliente( int id_presupuesto );
    static QDate obtenerFecha( int id_presupuesto );

private:
    void inicializar();
    void relacionar();

};

#endif
