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

#ifndef MMOVIMIENTOSCAJA_H
#define MMOVIMIENTOSCAJA_H

#include <QSqlRelationalTableModel>
#include <QDateTime>
class QSqlQuery;

class MMovimientosCaja : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MMovimientosCaja(QObject *parent = 0, bool relaciones = false );
    QVariant data( const QModelIndex& idx, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    void ultimosMovimientosCaja( const int id_caja );
    void actualizarMovimiento( const int id_movimiento_caja, QString texto );

    bool agregarMovimiento( int id_caja,  QString razon, QString responsable = QString(), double ingreso = 0.0, double egreso = 0.0, bool agregando_caja = false );
    bool verificarCierreCaja( const int id_caja );
    bool agregarCierre( const int id_caja, const QDateTime fechahora, const double saldo );
    bool eliminarMovimiento( const int id_movimiento = -1 );

    int buscarUltimoCierre( const int id_caja );
    int ultimoIdInsertado();

    double recalcularSaldo( const int id_caja );
    double saldoEnMovimientoAnteriorA( const int id_caja, const int id_movimiento_cierre );

    QSqlQuery buscarMovimientos( const int id_caja, const int id_cierre );

private:
    QString usuarioActual();

};

#endif // MMOVIMIENTOSCAJA_H
