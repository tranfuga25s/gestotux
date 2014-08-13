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

#ifndef MREMITO_H
#define MREMITO_H

#include <QSqlRelationalTableModel>
class NumeroComprobante;
class MProductosTotales;
#include <QDateTime>

class MRemito : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    enum FormaPago {
        Contado = 1,
        CuentaCorriente = 2,
        Cuotas = 3,
        Otro = 4
    };
    MRemito(QObject *parent = 0);
    int agregarVenta( QDateTime fecha, int id_cliente, MRemito::FormaPago id_forma_pago, MProductosTotales *mcp, QString observaciones = QString() );
    int agregarRemito( const int id_cliente, const QDateTime fecha, MRemito::FormaPago id_forma_pago, const double total, bool registrar_operacion = true );

    QSqlRecord obtenerDatos( const int id_remito );

    static NumeroComprobante &proximoComprobante();
    NumeroComprobante &obtenerComprobante();
    static NumeroComprobante &obtenerComprobante( const int id_remito );

    static bool anularRemito( const int id_remito, QString razon = QString(), QDateTime fechahora = QDateTime::currentDateTime() );
    static int idRemitoPorComprobante( const QString numero );
    static QDate fechaUltimoRemito();
    static int obtenerIdCliente(const int id_remito );

private:
    void inicializar();
    void relacionar();

};

#endif // MFACTURA_H
