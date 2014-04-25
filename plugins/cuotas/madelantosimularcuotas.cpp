#include "madelantosimularcuotas.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QColor>

MAdelantoSimularCuotas::MAdelantoSimularCuotas( QObject *parent ) :
MSimularCuotas( parent )
{
    cuotas = new QHash<int,double>();
    pagados = new QHash<int,bool>();
    modificables = new QHash<int,bool>();
    adelantos = new QHash<int,double>();
}

/*!
 * \brief MAdelantoSimularCuotas::~MAdelantoSimularCuotas
 */
MAdelantoSimularCuotas::~MAdelantoSimularCuotas()
{
    delete cuotas;
    cuotas=0;
    delete pagados;
    pagados=0;
    delete modificables;
    modificables=0;
    delete adelantos;
    adelantos=0;
}

/*!
 * \brief MAdelantoSimularCuotas::cargarCuotasPagadas
 * \param id_plan_cuota Identificador del Plan de Cuotas
 */
void MAdelantoSimularCuotas::cargarCuotasPagadas( const int id_plan_cuota )
{
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT num_cuota, fecha_pago, id_recibo FROM item_cuota WHERE id_plan_cuota = %1" ).arg( id_plan_cuota ) ) ) {
        while( cola.next() ) {
            int contador = cola.record().value("num_cuota").toInt();
            if( cola.record().value("fecha_pago").toDate().isValid() ) {
                pagados->insert( contador, true );
                modificables->insert( contador, false );
            } else if( cola.record().value( "id_recibo").toInt() > 0 ) {
                modificables->insert( contador, false );
                pagados->insert( contador, false );
            } else {
                modificables->insert( contador, true );
                pagados->insert( contador, false );
            }
        }
        pagados->insert( 0, true );
        modificables->insert( 0, false );
    } else {
        qDebug() << "Error al descargar los datos de cuotas pagadas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
}

/*!
 * \brief MAdelantoSimularCuotas::regenerar
 */
void MAdelantoSimularCuotas::regenerar()
{
    sumatoria->clear();
    faltante->clear();
    cuotas->clear();
    this->reset();
    faltante->insert( 0, importe*( 1 + ( interes/100 ) ) );
    sumatoria->insert( 0, 0.0 );
    double valor_cuota = importe/cantidad;
    valor_cuota *= 1+(interes/100);
    for( int i=1; i<=cantidad; i++ ) {
        if( pagados->value( i ) ) {
            cuotas->insert( i, 0.0 );
        } else {
            cuotas->insert( i, valor_cuota );
        }
    }
    // Recorro las cuotas al reves para restar el importe entregado
    double temporal_adelanto = _adelanto;
    for( int i=cantidad; i>=0; i-- ) {
        if( modificables->value( i ) ) {
            double temp = cuotas->value( i );
            temp -= temporal_adelanto;
            if( temp <= 0.0 ) {
                temporal_adelanto = (-1.0)*temp;
                cuotas->insert( i, 0.0 );
                adelantos->insert( i, valor_cuota );
            } else {
                cuotas->insert( i, temp );
                adelantos->insert( i, valor_cuota - temp );
                temporal_adelanto = 0.0;
            }
        } else {
            adelantos->insert( i, 0.0 );
        }
    }
    // Calculo el subtotal y la sumatoria
    for( int i=1; i<=cantidad; i++ ) {
        sumatoria->insert( i, sumatoria->value( i - 1 ) + cuotas->value( i ) );
        if( pagados->value( i ) ) {
            faltante->insert( i, faltante->value( i-1 ) - valor_cuota );
        } else {
            faltante->insert( i, faltante->value( i-1 ) - cuotas->value( i ) - adelantos->value( i ) );
        }
    }
    emit dataChanged( index( 0, 0 ), index( rowCount(), columnCount() ) );
}

/*!
 * \brief MAdelantoSimularCuotas::data
 * \param index
 * \param role
 * \return
 */
QVariant MAdelantoSimularCuotas::data(const QModelIndex &index, int role) const
{
    switch( index.column() ) {
        case 2:
        {
            if( role == Qt::DisplayRole ) {
                return QString( "$ %L1" ).arg( cuotas->value( index.row() ), 10, 'f', 2 );
            }
            break;
        }
        default:
        {
            if( role == Qt::BackgroundColorRole ) {
                if( pagados->value( index.row() ) ) {
                    return QColor( Qt::green );
                }
            }
        }
    }
    return MSimularCuotas::data( index, role );
}
