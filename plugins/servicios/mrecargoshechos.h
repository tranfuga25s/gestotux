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
#ifndef MRECARGOSHECHOS_H
#define MRECARGOSHECHOS_H

#include <QSqlRelationalTableModel>
#include <QDateTime>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MRecargosHechos : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MRecargosHechos(QObject *parent = 0);
    void inicializar();
    void relacionar();

    bool setData(const QModelIndex& item, const QVariant& value, int role);
    QVariant data(const QModelIndex& item, int role) const;

    static bool agregarRecargo( const int id_periodo_servicio, const int id_servicio, const int id_cliente, const int id_recargo, const QDateTime fecha = QDateTime::currentDateTime(), const QString detalle = QString(), const double costo = 0.0 );
    static double buscarRecargoPorPeriodoServicio( const int id_recibo, const int id_cliente );
    static bool existe( const int id_periodo_servicio, const int id_servicio, const int id_cliente, const int id_recargo );

};

#endif
