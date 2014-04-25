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

#ifndef MCLIENTESSERVICIOS_H
#define MCLIENTESSERVICIOS_H

#include <QSqlRelationalTableModel>

class MClientesServicios : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MClientesServicios( QObject *parent = 0, bool relacion = true );

    void filtrarPorServicio( const int id_servicio );
    void filtrarPorCliente( const int id_cliente );
    bool darDeBaja( int id_cliente, int id_servicio, QString razon = QString() );
    void setearVerBaja( bool estado );
    bool eliminarRelacion( const int id_cliente, const int id_servicio );

    QVariant data(const QModelIndex &item, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

private:
    bool _baja;
    int _cliente;
    int _servicio;

    void inicializar();
    void relacionar();
    void crearFiltro();

};

#endif // MCLIENTESSERVICIOS_H
