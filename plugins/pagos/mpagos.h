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
#ifndef MPAGOS_H
#define MPAGOS_H

#include <QSqlRelationalTableModel>
#include "NumeroComprobante.h"
#include <QSqlRecord>

/**
 * \brief Modelo de los pagos
 *
 * Modelo que administra los pagos realizados en el programa.
 *
 * @author Esteban Zeller <juiraze@yahoo.com.ar>
 */
class MPagos : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
   /*!
    * \enum FormaPago
    * Define el estado de pago que tiene el recibo
    */
    enum FormaPago {
        SinPagar = 0, /** Definido para cuando el recibo va a ser impreso pero no pagado ( solo HiComp ) */
        Efectivo = 1, /** Pago en efectivo */
        Otro = 2 /** Otro tipo de pago */
    };

    MPagos (QObject *parent = 0, bool relaciones = false );

    QVariant data( const QModelIndex& item, int role) const;

    int agregarRecibo( int id_cliente, QDate fecha, QString contenido, double total, bool efectivo, bool pagado = true, QString texto_ctacte = QString() );

    int buscarUltimoNumeroRecibo() const;
    QDate buscarFechaUltimoRecibo() const;
    bool setearComoPagado( const int id_recibo, const bool efectivo, const bool ctacte );
    bool buscarSiPagado( const int serie, const int numero );
    bool existe( NumeroComprobante num );
    bool anulado( NumeroComprobante num );
    int buscarIdPorSerieNumero( const NumeroComprobante num );
    int buscarIdPorSerieNumero( const int serie, const int numero );
    int buscarIdCliente( NumeroComprobante num );
    double buscarImporte( NumeroComprobante num );

    QDate buscarFechaEmisionRecibo( const int id_recibo );
    QDate buscarFechaEmisionRecibo( NumeroComprobante mum_recibo );

    bool buscarSiAPagarLuego( const int id_recibo );

    bool cancelarRecibo( const int id_recibo, QString razon, QDateTime fechahora );

    static NumeroComprobante &buscarMenorSerieNumeroPagado();
    static int numeroSerieActual();
    static int numeroReciboActual( const int serie );
    static NumeroComprobante &proximoSerieNumeroRecibo();
    static NumeroComprobante &buscarNumeroComprobantePorId( const int id_recibo );

    static bool verificarFacturaPagada( const QString numero );

    void setearId( const int id );
    QDate getFecha();
    int getIdCliente();
    QString getTexto();
    double getTotal();
    FormaPago getFormaPago();


private:
    void inicializar();
    void relacionar();

    int _id_actual;
    QSqlRecord _registro;
};

#endif
