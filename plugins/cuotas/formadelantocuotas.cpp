#include "formadelantocuotas.h"

#include "mitemplancuota.h"
#include "mpagos.h"
#include "EReporte.h"

#include <QSqlDatabase>
#include <QMessageBox>

FormAdelantoCuotas::FormAdelantoCuotas( QWidget *parent ) :
FormDetalleCuotas( parent )
{
    GBAdelanto->setVisible( true );
    GBDetalles->setVisible( false );

    connect( DSBImporteAdelanto, SIGNAL( valueChanged( double ) ), this, SLOT( cambiarImporte( double ) ) );

    ActConfirmar = new QAction( this );
    ActConfirmar->setText( "Confirmar" );
    ActConfirmar->setStatusTip( "Confirma el adelanto y la modificacion del plan de cuotas" );
    connect( ActConfirmar, SIGNAL( triggered() ), this, SLOT( confirmar() ) );

    addAction( ActConfirmar );

    _id_plan_cuota = 0;
}

void FormAdelantoCuotas::setearIdPlanCuota( int id )
{
    FormDetalleCuotas::setearIdPlanCuota( id );
    this->_id_plan_cuota = id;
    modelo_item = new MAdelantoSimularCuotas( this );
    TVSimulacion->setModel( modelo_item );
    modelo_item->setCuotas( SBCantidad->value() );
    modelo_item->setFechaInicio( DEInicio->date() );
    modelo_item->setImporte( DSBImporte->value() );
    modelo_item->setInteres( DSBInteres->value() );
    modelo_item->setPeriodo( (MPlanCuota::Periodo)CBPeriodo->currentIndex() );
    modelo_item->setPagoInicial( DSBEntrega->value() );
    modelo_item->cargarCuotasPagadas( id );
    modelo_item->setearImporteAdelanto( 100.0 );
    modelo_item->regenerar();
}

/*!
 * \brief FormAdelantoCuotas::cambiarImporte
 * \param importe Importe cambiado
 */
void FormAdelantoCuotas::cambiarImporte( double importe )
{ modelo_item->setearImporteAdelanto( importe ); }

/*!
 * \brief FormAdelantoCuotas::confirmar
 */
void FormAdelantoCuotas::confirmar()
{
    QSqlDatabase::database().transaction();
    if( !MItemPlanCuota::agregarAdelanto( this->_id_plan_cuota, DSBImporteAdelanto->value() ) ) {
        QMessageBox::information( this, "Error", "No se pudo cambiar los datos de los pagos" );
        QSqlDatabase::database().rollback();
        return;
    }
    // Imprimo un recibo por la cantidad
    QString contenido = QString( "Pago de adelanto sobre plan de cuotas #%1" ).arg( _id_plan_cuota );
    MPagos *mp = new MPagos();
    int id_recibo = mp->agregarRecibo( CBCliente->idClienteActual(),
                                       QDate::currentDate(),
                                       contenido,
                                       DSBImporteAdelanto->value(),
                                       CkBAdelantoEfectivo->isChecked(),
                                       true,
                                       QString() );
    if( id_recibo == -1 ) {
        QMessageBox::information( this, "Error", "No se pudo guardar el recibo del pago del adelanto" );
        delete mp;
        QSqlDatabase::database().rollback();
        return;
    }
    delete mp;
    // Si los datos se guardaron correctamente, imprimo el recibo. Si al impresión falla, quedó emitido.
    if( QSqlDatabase::database().commit() ) {
        // Imprimo el recibo
        EReporte *e = new EReporte( this );
        e->recibo();
        ParameterList lista;
        lista.append( Parameter( "id_recibo", id_recibo ) );
        e->hacer( lista, false, true );
        QMessageBox::information( this, "Correcto", "El adelanto se registro correctamente. Se emitió el recibo correspondiente y se modificaron las cuotas." );
        emit actualizarModelo();
        this->close();
    } else {
        QMessageBox::information( this, "Incorrecto", "No se pudo guardar la transacción!");
    }
    return;
}
