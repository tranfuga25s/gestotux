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

#ifndef MITEMPRESUPUESTO_H
#define MITEMPRESUPUESTO_H

#include <QSqlRelationalTableModel>

class MItemPresupuesto : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit MItemPresupuesto(QObject *parent = 0);
    bool agregarItemPresupuesto( const int id_presupuesto, const double cantidad, const QString texto, const double precio_unitario );
    void setearId( const int id_presupuesto );
    bool eliminarItemsDePresupuesto( const int id_presupuesto );

private:
    void inicializar();
    void relacionar();

};

#endif // MITEMPRESUPUESTO_H
