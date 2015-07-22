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

#ifndef MCAJAS_H
#define MCAJAS_H

#include <QSqlTableModel>
#include <QDate>
#include <QDateTime>

class MCajas : public QSqlTableModel
{
    Q_OBJECT
    public:
        MCajas(QObject *parent = 0 );
        virtual Qt::ItemFlags flags( const QModelIndex &idx ) const;
        QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;

        bool agregarCaja( QString nombre, QDate fecha_alta = QDate::currentDate(), double saldo_inicial = 0 );

        bool hacerCierre( const int id_caja, QDateTime fecha, double saldo );

        bool existeCaja( const QString nombre );

        bool renombrarCaja( const int id_caja, QString nombre );

        bool tieneDatosRelacionados( const int id_caja );
        bool eliminarCaja( const int id_caja );

        static bool actualizarSaldo( const int id_caja, const double cantidad );
        static double saldo( const int id_caja );

        static QString nombreCaja( const int id_caja );

        static int cajaPredeterminada();
        static int buscarPrimerCaja();
        static bool existen2omas();
};

#endif // MCAJAS_H
