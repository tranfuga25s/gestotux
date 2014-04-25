#include "mplancuota.h"

#include "mitemplancuota.h"
#include "math.h"
#include "mpagos.h"
#include "EReporte.h"

#include <QDate>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlDriver>
#include <QDebug>

MPlanCuota::MPlanCuota(QObject *parent) :
QSqlTableModel(parent)
{
    setTable( "plan_cuota" );
    setHeaderData( 0, Qt::Horizontal, "#Plan" );
    setHeaderData( 1, Qt::Horizontal, "#Factura" );
    setHeaderData( 2, Qt::Horizontal, "Cantidad" );
    setHeaderData( 3, Qt::Horizontal, "Periodo" );
    setHeaderData( 4, Qt::Horizontal, "Fecha inicio" );
    setHeaderData( 5, Qt::Horizontal, "Entrega inicial" );
    setHeaderData( 6, Qt::Horizontal, "Recargo total" );
}

/*!
 * \fn MPlanCuota::agregarPlanCuota( int id_cliente, double cantidad, double interes, int periodo, double entrega )
 * Genera un nuevo plan de cuotas con los datos pasados como parametros
 * \param id_cliente Identificador de cliente
 * \param cantidad Importe total a generar en cuotas
 * \param interes Intereses a aplicar
 * \param periodo Tipo de periodo
 * \param entrega Entrega inicial realizada
 * \param fecha_inicio Fecha desde cuando se inicia el plan de cuotas
 * \param id_plan Variable donde se devolverÃ¡ el numero de plan de cuota
 * \param cant_cuotas Cantidad de cuotas en que se dividio el pago
 * \param efectivo Muestra si el recibo de entrega inicial tiene que ser en efectivo o no
 * \returns Verdadero si se pudo generar el plan y sus items
 */
bool MPlanCuota::agregarPlanCuota( int id_cliente, double cantidad, double interes, int periodo, double entrega, QDate fecha_inicio, int cant_cuotas, int *id_plan, bool recibo_efectivo, TipoComprobante tipo_comprobante )
{
    // Verifico los parametros
    if( id_cliente <= 0 && cantidad <= 0.0 && interes <= 0.0 && periodo <= 0  ) {
        qWarning( "Parametros de plan de cuota incorrecto" );
        return false;
    }

    QSqlRecord rec = this->record();
    rec.setValue( "id_cliente", id_cliente );
    rec.setValue( "total", cantidad );
    rec.setValue( "cantidad_cuotas", cant_cuotas );
    rec.setValue( "tipo_periodo", periodo );
    rec.setValue( "fecha_inicio", fecha_inicio );
    rec.setValue( "entrega_inicial", entrega );
    rec.setValue( "recargo", interes );
    // Agrego el FK de ID factura en un valor random
    rec.setValue( "id_factura", -2 );
    rec.setValue( "tipo_comprobante", tipo_comprobante );
    if( this->insertRecord( -1, rec ) ) {
        // Genero los items de cuota
        if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driver()->hasFeature( QSqlDriver::LastInsertId ) ) {
           *id_plan =this->query().lastInsertId().toInt();
           qWarning() << QString( "Numero de plan de cuota emitido: %1" ).arg( *id_plan );
        } else {
           QSqlQuery cola;
           if( cola.exec( "SELECT MAX( id_plan_cuota ) FROM plan_cuota" ) ) {
               cola.next();
               *id_plan = cola.record().value(0).toInt();
               //qWarning( QString( "Numero de plan de cuota emitido: %1" ).arg( *id_plan ).toLocal8Bit() );
           } else {
               qDebug() << "Error de obtencion del id del registro de plan de cuota";
               qDebug() << QString( "Detalles: tipo: %1, errno: %2, descripcion: %3" ).arg( cola.lastError().type() ).arg( cola.lastError().number() ).arg( cola.lastError().text() );
               qDebug() << cola.lastQuery();
               *id_plan = -1;
               return false;
           }
        }

        // Calculo los valores
        double impcuota = ( cantidad - entrega ) * ( 1 + interes/100 );
        double importe = impcuota/cant_cuotas;
        QDate fecha_venc = fecha_inicio.addDays( diasEnPeriodo( periodo, fecha_inicio ) );
        // Genero los elementos de el plan de cuotas
        for( int i = 0; i < cant_cuotas; i++ ) {
            if( !MItemPlanCuota::agregarItem( *id_plan, i+1, fecha_venc, importe ) ) {
                qDebug( "Error al agregar un item del plan de cuotas" );
                *id_plan = -1;
                return false;
            } else {
                // Actualizo la fecha de la proxima cuota
                fecha_venc = fecha_venc.addDays( diasEnPeriodo( periodo, fecha_venc ) );
            }
        }

        if( entrega > 0.0 ) {
            MPagos *mp = new MPagos( this, false );
            QString contenido = QString( "Pago de entrega inicial de plan de cuota #%1" ).arg( *id_plan );
            int id_recibo =  mp->agregarRecibo( id_cliente,
                                                QDate::currentDate(),
                                                contenido,
                                                entrega,
                                                recibo_efectivo,
                                                true );
            if( id_recibo != -1 ) {
                // Emitir el comprobante
                EReporte *rep = new EReporte( 0 );
                rep->recibo();
                ParameterList lista;
                lista.append( "id_recibo", id_recibo );
                if( !rep->hacer( lista ) ) {
                    qWarning() << QString::fromUtf8( "No se pudo emitir el recibo por la entrega inicial del plan de cuotas; Pero el recibo quedó emitido." );
                }
                delete rep;rep=0;
            } else {
                // Reporto el problema
                qWarning( "No se pudo agregar el registro correspondiente al pago de la entrega inicial del plan de cuotas" );
                delete mp; mp=0;
                *id_plan=-1;
                return false;
            }
            delete mp; mp=0;
        }
        return true;
    } else {
        qWarning( "Error al intentar insertar el registro del plan de cuotas" );
        qDebug() << "Error al insert record de plan_cuota";
        qDebug() << this->lastError().text();
        qDebug() << this->query().lastQuery();
        *id_plan=-1;
        return false;
    }
}

/*!
 * \fn MPlanCuota::diasEnPeriodo( const int tipo_periodo, QDate fecha_inicio )
 * Considerando que todos los periodos se ajustan dentro de un aÃ±o, devolverÃ¡ el numero de dÃ­as que tiene el periodo seleccionado en la fecha elegida ( sin parametro fecha actual ) segun la fecha de alta del servicio.
 * En el caso de que sea mensual, se devolverÃ¡ la cantidad de dÃ­as que tiene el mes de fecha_calculo
 * En el caso de que sea bimestral, se devolverÃ¡ la cantidad de dÃ­as que tiene el mes de fecha_calculo mas la cantidad de dÃ­as que tiene el mes siguiente.
 * En el caso de que sea trimestra, se devolverÃ¡ la cantidad de dÃ­as que tiene el mes de fecha_calculo mas la cantidad de dÃ­as que tienen los 2 meses siguientes.
 * @param tipo_periodo Tipo de periodo que estamos considerando
 * @param fecha_inicio Fecha que se desea averiguar el periodo ( predeterminada fecha actual )
 * @return Cantidad de DÃ­as que tiene el periodo que corresponde a la fecha solicitada
 */
int MPlanCuota::diasEnPeriodo( const int tipo_periodo, QDate fecha_calculo )
{
    switch( tipo_periodo )
    {
        case MPlanCuota::Semanal:
        {
            // Semanal -> La semana siempre tiene 7 dias independientemente del dÃ­a del mes
            /// \todo Corte anual Â¿Considerarlo? ( cuando la semana no esta completa un aÃ±o )
            return 7;
        }
        case MPlanCuota::Quincenal:
        {
            // Quincenal -> se considera como "medio mes"
            if( floor( fecha_calculo.daysInMonth() / 2 ) > fecha_calculo.day() ) {
                // Segunda quincena
                QDate f1(floor( fecha_calculo.daysInMonth() / 2 ), fecha_calculo.month(), fecha_calculo.year() );
                QDate f2(  fecha_calculo.year(), fecha_calculo.month(),fecha_calculo.daysInMonth() );
                return f1.daysTo( f2 ) - 1;
            } else {
                // Primera quincena
                QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
                QDate f2( fecha_calculo.year(), fecha_calculo.month(), floor( fecha_calculo.daysInMonth() / 2 ) );
                return f1.daysTo( f2 ) - 1;
            }
        }
        case MPlanCuota::Mensual:
        {
            // Mensual
            // Verificar el mes del periodo y devolver la cantidad de di­as
            return QDate( fecha_calculo.year(), fecha_calculo.month(), 1 ).daysInMonth();
            // Eso se encarga automaticamnete de los aÃ±os bisiestos
        }
        case MPlanCuota::BiMensual:
        {
            // BiMensual
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(1);
            QDate f3( f2.year(), f2.month(),f2.daysInMonth() );
            return f1.daysTo( f3 ) - 1;
        }
        case MPlanCuota::Trimestral:
        {
            // Trimensual
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(2);
            QDate f3( f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) - 1;
        }
        case MPlanCuota::Cuatrimestral:
        {
            // Cuatrimestral
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(3);
            QDate f3( f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) -1 ;
        }
        case MPlanCuota::Seximestral:
        {
            //Seximestral
            // Siempre voy a pedir estos datos al inicio del periodo
            QDate f1( fecha_calculo.year(), fecha_calculo.month(), 1 );
            QDate f2 = f1.addMonths(5);
            QDate f3(  f2.year(), f2.month(), f2.daysInMonth() );
            return f1.daysTo( f3 ) - 1 ;
        }
        case MPlanCuota::Anual:
        {
            // Como consideramos los servicios con base en 1 año, siempre es periodo 1
            return fecha_calculo.daysInYear() - 1;
        }
        default:
        { qDebug( "Tipo de periodo desconocido" ); qDebug() << QString::number( tipo_periodo ); return 0; }
    }
}

/*!
 * \brief MPlanCuota::asociarConFactura
 * \param id_plan Plan al cual asociar el identificador de factura o remito
 * \param id_factura Identificador de la factura al cual pertenece
 */
void MPlanCuota::asociarConFactura( int id_plan, int id_factura )
{
    if( id_factura <= 0 && id_plan <= 0 ) {
        qWarning( "Numero de factura o plan invalido!" );
        return;
    }
    QSqlQuery cola;
    if( cola.exec( QString( "UPDATE plan_cuota SET id_factura = %1 WHERE id_plan_cuota = %2" ).arg( id_factura ).arg( id_plan ) ) ) {
        qDebug( "Plan de cuotas actualizado correctamente" );
    } else {
        qWarning( "No se pudo asociar el plan de cuotas con la factura recién emitida!" );
        qDebug() << "Error al ejecutar la cola de actualziacion de id de factura en el plan  de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
 * \brief MPlanCuota::obtenerRazonSocial
 * Devuelve la razón social del cliente asociado con el plan de cuotas pasado como parametro
 * \param id_plan Identificador del plan de cuotas
 * \return Razon social del cliente o nulo si no se encuentra el cliente.
 */
QString MPlanCuota::obtenerRazonSocial( const int id_plan )
{
    QSqlQuery cola;
    MPlanCuota::TipoComprobante tipo = MPlanCuota::obtenerTipoComprobante( id_plan );
    if( tipo == MPlanCuota::Factura ) {
        if( cola.exec( QString( "SELECT c.razon_social FROM plan_cuota AS pc, clientes AS c, factura AS f WHERE c.id = f.id_cliente AND f.id_factura = pc.id_factura AND pc.id_plan_cuota = %1" ).arg( id_plan ) ) ) {
            if( cola.next() ) {
                return cola.record().value(0).toString();
            } else {
                qDebug() << "Error de next al obtención de razon social de un plan de cuotas";
                qDebug() << cola.lastQuery();
            }
        } else {
            qDebug() << "Error al ejecutar la cola de obtención de razon social de un plan de cuotas";
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    } else if( tipo == MPlanCuota::Remito ){
        if( cola.exec( QString( "SELECT c.razon_social FROM plan_cuota AS pc, clientes AS c, remito AS r WHERE c.id = r.id_cliente AND r.id_remito = pc.id_factura AND pc.id_plan_cuota = %1" ).arg( id_plan ) ) ) {
            if( cola.next() ) {
                return cola.record().value(0).toString();
            } else {
                qDebug() << "Error de next al obtención de razon social de un plan de cuotas";
                qDebug() << cola.lastQuery();
            }
        } else {
            qDebug( "Error al ejecutar la cola de obtención de razon social de un plan de cuotas" );
            qDebug() << cola.lastError().text();
            qDebug() << cola.lastQuery();
        }
    }
    return QString();
}

/*!
 * \brief MPlanCuota::obtenerTipoComprobante
 * Devuelve el tipo de comprobante que tiene un plan de cuota
 * \param id_plan Identificador del plan
 * \return Tipo de comprobante
 */
MPlanCuota::TipoComprobante MPlanCuota::obtenerTipoComprobante( const int id_plan ) {
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT pc.tipo_comprobante FROM plan_cuota AS pc WHERE pc.id_plan_cuota = %1" ).arg( id_plan ) ) ) {
        if( cola.next() ) {
            return (MPlanCuota::TipoComprobante)cola.record().value(0).toInt();
        } else {
            qDebug( "Error de next al obtención del tipo de comprobante de un plan de cuotas" );
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de razon social de un plan de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return MPlanCuota::TipoInvalido;
}

/*!
 * \brief MPlanCuota::obtenerIdCliente
 * Devuelve el ID del cliente asociado con el plan de cuotas pasado como parametro
 * \param id_plan Identificador del plan de cuotas
 * \return id del cliente o -1 si hubo algún error
 */
int MPlanCuota::obtenerIdCliente( const int id_plan )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT c.id FROM plan_cuota AS pc, clientes AS c, factura AS f, remito AS r WHERE ( c.id = f.id_cliente OR c.id = r.id_cliente ) AND r.id_remito = pc.id_factura AND f.id_factura = pc.id_factura AND pc.id_plan_cuota = %1" ).arg( id_plan ) ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toInt();
        } else {
            qDebug( "Error de next al obtención del Id de cliente de un plan de cuotas" );
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug( "Error al ejecutar la cola de obtención del id de cliente de un plan de cuotas" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return -1;
}

/*!
 * \brief MPlanCuota::obtenerEstadoCuotas
 * Devuelve el par de datos cantidad_cuotas/cuotas_pagadas de un plan de cuotas.
 * \param id_plan Identificador del plan de cuotas
 * \return los datos o un par -1,-1 si hubo un error.
 */
QPair<int, int> MPlanCuota::obtenerEstadoCuotas( const int id_plan )
{
    QSqlQuery cola;
    QPair<int, int> par( -1, -1 );
    if( cola.exec( QString( "SELECT pc.cantidad_cuotas FROM plan_cuota AS pc WHERE pc.id_plan_cuota = %1" ).arg( id_plan ) ) ) {
        if( cola.next() ) {
            par.second = cola.record().value(0).toInt();
        } else {
            qDebug() << "Error de next al obtención de cantidad de cuotas de un plan de cuotas";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de cantidad de cuotas de un plan de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    if( cola.exec( QString( "SELECT COUNT( id_plan_cuota ) FROM item_cuota  WHERE id_plan_cuota = %1 AND fecha_pago IS NOT NULL" ).arg( id_plan ) ) ) {
        if( cola.next() ) {
            par.first = cola.record().value(0).toInt();
        } else {
            qDebug() << "Error de next al obtención de cantidad de cuotas de un plan de cuotas";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de cantidad de cuotas de un plan de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return par;
}

/*!
 * \brief MPlanCuota::obtenerEstadoImportes
 * Devuelve el par de datos cantidad pagada/importe total del plan de cuotas
 * \param id_plan Identificador del plan
 * \return importes mencionados o -1.0/-1.0
 */
QPair<double, double> MPlanCuota::obtenerEstadoImportes( const int id_plan )
{
    QPair<double, double> par( -1.0, -1.0 );
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT SUM( monto ) FROM item_cuota WHERE id_plan_cuota = %1 AND fecha_pago IS NOT NULL" ).arg( id_plan ) ) ) {
        if( cola.next() ) {
            par.first = cola.record().value(0).toDouble();
        } else {
            qDebug() << "Error de next al obtención de estado financiero de un plan de cuotas";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de estado financiero de un plan de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    if( cola.exec( QString( "SELECT pc.total FROM plan_cuota AS pc WHERE pc.id_plan_cuota = %1" ).arg( id_plan ) ) ) {
        if( cola.next() ) {
            par.second = cola.record().value(0).toDouble();
        } else {
            qDebug() << "Error de next al obtención de estado financiero de un plan de cuotas";
            qDebug() << cola.lastQuery();
        }
    } else {
        qDebug() << "Error al ejecutar la cola de obtención de estado financiero de un plan de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return par;
}

/*!
 * \brief MPlanCuota::cancelarPlan
 * Pone el plan de cuotas especificado como cancelado, con su razon y fecha, eliminando además todos los items de pago a futuro que no esten pagados o emitidos.
 * \param id_plan_cuota Identificador del plan de cuotas
 * \param razon Razon de la cancelación
 * \param fecha_hora Fecha y hora de la acción
 * \return Verdadero si se pudo realizar todo el proceso correctamente. Falso en caso contrario.
 */
bool MPlanCuota::cancelarPlan( const int id_plan_cuota, QString razon, QDateTime fecha_hora )
{
    if( id_plan_cuota <= 0 ) {
        qDebug() << "Identificador de plan de cuota invalido";
        return false;
    }
    if( razon.isEmpty() || razon.isNull() ) {
        // Coloco la razón predeterminada
        razon.append( QString::fromUtf8( "Razón desconocida" ) );
    }
    QSqlQuery cola;
    if( cola.prepare( "UPDATE plan_cuota SET razon_cancelado = :razon, fechahora_cancelacion = :fechahora_cancelacion WHERE id_plan_cuota = :id_plan_cuota" ) ) {
        return false;
    }
    cola.bindValue( ":razon", razon );
    cola.bindValue( ":fechahora_cancelacion", fecha_hora );
    cola.bindValue( ":id_plan_cuota", id_plan_cuota );
    if( cola.exec() ) {
        // Empiezo a eliminar los items de plan de cuota
        return MItemPlanCuota::eliminarItemsNoPagadosNoEmitidos( id_plan_cuota );
    } else {
        qDebug() << "Error al ejecutar la cola de seteo de plan de cuota";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return false;
}
