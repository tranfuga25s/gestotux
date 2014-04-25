/***************************************************************************
 *   Copyright (C) 2006 by Esteban Zeller   *
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
#ifndef MGASTO_H
#define MGASTO_H

#include <QSqlRelationalTableModel>
class QDate;

/**
 * @brief Modelo de gastos
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MGasto : public QSqlRelationalTableModel
{
Q_OBJECT
public:
    MGasto(QObject *parent = 0, bool relaciones = false );
    QVariant data(const QModelIndex& item, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool agregarGasto( QString descripcion, double costo, QDate Fecha, int categoria );
    bool setearIdMovimiento( int num_mov, double valor );
    bool eliminarFila( const int fila );
    bool existe( QString descripcion, double costo, int categoria );
};

#endif
