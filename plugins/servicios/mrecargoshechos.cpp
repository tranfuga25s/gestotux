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
#include "mrecargoshechos.h"

#include <QSqlRelation>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include "mcuentacorriente.h"
#include "mitemcuentacorriente.h"
#include "mrecargos.h"

MRecargosHechos::MRecargosHechos(QObject *parent)
 : QSqlRelationalTableModel(parent)
{
    inicializar();
}

/*!
 * \brief MRecargosHechos::inicializar
 */
void MRecargosHechos::inicializar()
{
    setTable( "recargos_hechos" );
    setHeaderData( 0, Qt::Horizontal, tr( "#ID Periodo Servicio" ) );
    setHeaderData( 1, Qt::Horizontal, tr( "#ID Servicio" ) );
    setHeaderData( 2, Qt::Horizontal, tr( "#ID Cliente" ) );
    setHeaderData( 3, Qt::Horizontal, tr( "#ID Recargo" ) );
    setHeaderData( 4, Qt::Horizontal, tr( "Fecha Generado" ) );
    setHeaderData( 5, Qt::Horizontal, tr( "Detalle" ) );
    setHeaderData( 6, Qt::Horizontal, tr( "Costo" ) );
    setHeaderData( 7, Qt::Horizontal, tr( "#ID Item Cta Cte" ) );
    setHeaderData( 8, Qt::Horizontal, tr( "#ID Recibo" ) );
}

/*
CREATE TABLE `recargo_cobro_servicio_cliente` (
      `id_periodo_servicio` BIGINT NOT NULL,
      `id_servicio` BIGINT NOT NULL,
      `id_cliente` BIGINT NOT NULL,
      `id_recargo` BIGINT NOT NULL,
      `fecha_generado` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
      `texto_detalle` TEXT NOT NULL,
      `precio` DOUBLE NOT NULL,
      `id_ctacte` BIGINT NOT NULL,
      `id_recibo` BIGINT,
      FOREIGN KEY ( id_periodo_servicio, id_servicio, id_cliente ) REFERENCES `cobro_servicio_cliente_periodo`( id_periodo_servicio, id_servicio, id_cliente ),
      FOREIGN KEY ( id_recargo ) REFERENCES `recargos`( id_recargo ),
      FOREIGN KEY ( id_ctacte ) REFERENCES `item_ctacte`( id_op_ctacte ),
      FOREIGN KEY ( id_recibo ) REFERENCES `recibos`( id_recibo ),
      PRIMARY KEY ( id_periodo_servicio, id_servicio, id_cliente, id_recargo )
) ENGINE = InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_spanish_ci;
*/

/*!
 * \brief MRecargosHechos::relacionar
 */
void MRecargosHechos::relacionar()
{
    setRelation( 2, QSqlRelation( "clientes", "id", "razon_social" ) );
    setRelation( 1, QSqlRelation( "servicios", "id_servicio", "nombre" ) );
}

/*!
 * \brief MRecargosHechos::setData
 * \param item
 * \param value
 * \param role
 * \return
 */
bool MRecargosHechos::setData(const QModelIndex& item, const QVariant& value, int role)
{
    return QSqlRelationalTableModel::setData(item, value, role);
}

/*!
 * \brief MRecargosHechos::data
 * \param item
 * \param role
 * \return
 */
QVariant MRecargosHechos::data(const QModelIndex& item, int role) const
{
    switch( role )
    {
        case Qt::DisplayRole:
        {
            switch( item.column() ) {
                case 4:
                {
                    return QSqlRelationalTableModel::data( item, role ).toDateTime().toLocalTime();
                    break;
                }
                case 6:
                {
                    return QString( "$ %L1" ).arg( QSqlRelationalTableModel::data(item,role).toDouble(), 10, 'f', 2 );
                    break;
                }
                default:
                {
                    return QSqlRelationalTableModel::data( item, role );
                    break;
                }
            }
            break;
        }
        default:
        {   return QSqlRelationalTableModel::data(item, role);  }
    }
}

/*!
 * \fn MRecargosHechos::agregarRecargo( const int id_periodo_servicio, const int id_servicio, const int id_cliente, const int id_recargo, const QDateTime fecha, const QString detalle, const double costo )
 * Agrego un registro del tipo recargo para un servicio especifico y el cliente especificado.
 * Si no se especifica detalle se generará una entrada predeterminada.
 * Si no se especifica el costo se autocalculara.
 * Aparte de generar el registro correspondiente agrega la operacion en la cuenta corriente y guarda el identificador en el registro.
 * El registro va a estar como no pagado automaticamente. ( id_recibo = null )
 * \param id_periodo_servicio Identificador del periodo para el servicio.
 * \param id_servicio Identificador del servicio
 * \param id_cliente Identificador del cliente
 * \param id_recargo Identificador del recargo que estoy haciendo.
 * \param fecha Fecha y hora generado
 * \param detalle Detalle del item para la cuenta corriente y la factura
 * \param costo Costo total del recargo
 * \returns Verdadero si ha podido ser guardado.
 */
bool MRecargosHechos::agregarRecargo( const int id_periodo_servicio, const int id_servicio, const int id_cliente, const int id_recargo, const QDateTime fecha, QString detalle, double costo )
{
    // Verifico los defaults
    if( detalle.isEmpty() ) {
        detalle.append( "Recargo por pago fuera de termino" );
    }
    if( costo <= 0.0 ) {
        costo = MRecargos::calcularRecargo( id_recargo, false );
    }

    // Veo de actualizar la cuenta corriente del cliente
    QString ctacte = MCuentaCorriente::obtenerNumeroCuentaCorriente( id_cliente );
    if( ctacte == QString::number( MCuentaCorriente::ErrorNumeroCuenta )
        || ctacte == QString::number( MCuentaCorriente::ErrorClienteInvalido ) ) {
        qDebug() << "El cliente no posee una cuenta corriente!!! ( aseveracion que implica error anterior en el sistema!)";
        qDebug() << "ID Cliente: " << id_cliente;
        return false;
    }
    // Teoricamente el identificador sirve para vincular de vuelta el item con su origen. ¿Que numero colocamos aca?
    // Que numero de comprobante posee?? teoricamente ninguno,a si que le mandamos una cadena vacia.
    int id_ctacte = MItemCuentaCorriente::agregarOperacion( ctacte,
                                                            QString(), // no tenemos numero de comprobante
                                                            0, // tampoco tenemos refencia
                                                            MItemCuentaCorriente::RecargoCobroServicio,
                                                            fecha.date(),
                                                            detalle,
                                                            costo );
    if( id_ctacte == -1 ) {
        qDebug() << "No se pudo guardar el item en la cuenta corriente. Verificar log.";
        return false;
    }

    // Intento generar el registro
    QSqlQuery cola;
    if( !cola.prepare(
                "INSERT INTO recargo_cobro_servicio_cliente"
                "(  id_periodo_servicio,  id_servicio,  id_cliente,  id_recargo,  fecha_generado,  texto_detalle,  precio,  id_ctacte ) VALUES"
                "( :id_periodo_servicio, :id_servicio, :id_cliente, :id_recargo, :fecha_generado, :texto_detalle, :precio, :id_ctacte )"
                ) ) {
        qDebug() << "Error de prepare";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    cola.bindValue( ":id_periodo_servicio", id_periodo_servicio );
    cola.bindValue( ":id_servicio", id_servicio );
    cola.bindValue( ":id_cliente", id_cliente );
    cola.bindValue( ":id_recargo", id_recargo );
    cola.bindValue( ":fecha_generado", fecha );
    cola.bindValue( ":texto_detalle", detalle );
    cola.bindValue( ":precio", costo );
    cola.bindValue( ":id_ctacte", id_ctacte );
    // id_recibo es colocado cuando es pagado
    if( cola.exec() ) {
        qDebug() << "Registro de recargo insertado correctamente";
        return true;
    } else {
        qDebug() << "Cola de insercion de recargo incorrecta";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}

/*!
 * \fn MRecargosHechos::buscarRecargoPorPeriodoServicio( const int id_periodo_servicio, const int id_cliente )
 * Devuelve la suma de los recargos que se hayan realizado para el periodo de servicio para el cliente especificado
 * \param id_periodo_servicio Periodo del servicio que corresponda
 * \param id_cliente Identificador del cliente
 * \returns Sumatoria ( puede ser cero ) y cero en caso de error
 */
double MRecargosHechos::buscarRecargoPorPeriodoServicio( const int id_periodo_servicio, const int id_cliente )
{
    // Busco el ID de periodoservicio en los recargos realizados que coincidan
    if( id_periodo_servicio <= 0  ) {
        qDebug() << "Id de periodo servicio incorrecto al buscar recargos hechos";
        return 0.0;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT SUM( precio ) FROM recargo_cobro_servicio_cliente WHERE id_periodo_servicio = %1 AND id_cliente = %2" ).arg( id_periodo_servicio ).arg( id_cliente ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDouble();
        } else {
            qDebug() << "MRecargosHechos::buscarRecargoPorPeriodoServicio: Error al hacer next en buscar recargos";
        }
    } else {
        qDebug() << "MRecargosHechos::buscarRecargoPorPeriodoServicio: Error al hacer exec";
        qDebug() << cola.lastQuery();
        qDebug() << cola.lastError().text();
    }
    return 0.0;
}

/*!
 * \brief MRecargosHechos::existe
 * Permite saber si existe algun recargo con los datos pasados como parametro
 * \param id_periodo_servicio Identificador del periodo de servicio
 * \param id_servicio Identificador del servicio
 * \param id_cliente Identificador del Cliente
 * \param id_recargo Identificador del recargo
 * \return Verdadero si existe algun recargo cobrado al cliente que corresponda con los 4 parametros
 */
bool MRecargosHechos::existe( const int id_periodo_servicio, const int id_servicio, const int id_cliente, const int id_recargo )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT COUNT( id_cliente ) FROM recargo_cobro_servicio_cliente WHERE id_periodo_servicio = %1 AND id_cliente = %2 AND id_servicio = %3 AND id_recargo = %4" ).arg( id_periodo_servicio ).arg( id_cliente ).arg( id_servicio ).arg( id_recargo ) ) ) {
        cola.next();
        if( cola.record().value(0).toInt() > 0 ) {
            return true;
        }
    } else {
        qDebug() << "MRecargosHechos::existe: Error al hacer exec";
        qDebug() << cola.lastQuery();
        qDebug() << cola.lastError().text();
    }
    return false;
}
