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
#ifndef MCARAVANADUENO_H
#define MCARAVANADUENO_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QPair>
#include <QHash>

/**
	@author Esteban Zeller <juiraze@yahoo.com.ar>
*/
class MCaravanaDueno : public QAbstractTableModel
{
Q_OBJECT
public:
    MCaravanaDueno(QObject *parent = 0);
    ~MCaravanaDueno();
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    bool removeRow(int row, const QModelIndex& parent = QModelIndex() );
    bool insertRow(int row, const QModelIndex& parent = QModelIndex() );
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const ;
    QStringList listaCaravanas();
    bool verificarAgregar( const QString &codigo, const QString &dueno = "" );
    bool verificarAgregar( const QStringList &listado, const QString &dueno = "");
    void setDuenosTodos( const QString &dueno );
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const ;
    bool cargarCaravanasTri( int id_tri );
private:
	QHash<int, QPair<QString,QString> > datos;
};

#endif
