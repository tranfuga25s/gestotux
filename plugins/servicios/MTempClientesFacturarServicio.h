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

#ifndef MTEMPCLIENTESFACTURARSERVICIO_H
#define MTEMPCLIENTESFACTURARSERVICIO_H

#include <QAbstractTableModel>

class MTempClientesFacturarServicio : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MTempClientesFacturarServicio(QObject *parent = 0);
    bool insertRow(int row, const QModelIndex& parent = QModelIndex() );
    bool removeRow(int row, const QModelIndex& parent = QModelIndex() );
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    int columnCount(const QModelIndex& parent = QModelIndex() ) const;
    int rowCount(const QModelIndex& parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex& idx, int role) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    void cargarClientesDelServicio( const int id );
    void cargarClientesDelServicioAFacturar( const int id );

private:
        QHash<int, QString> *clientes;
        QHash<int, bool> *marcados;
        QHash<int, int> *ids;

};

#endif // MTEMPCLIENTESFACTURARSERVICIO_H
