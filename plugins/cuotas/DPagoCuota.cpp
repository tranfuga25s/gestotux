#include "DPagoCuota.h"

#include <QMessageBox>
#include <QDebug>

#include "mplancuota.h"
#include "mitemplancuota.h"
#include "NumeroComprobante.h"
#include "mpagos.h"

DPagoCuota::DPagoCuota(QWidget *parent) :
QDialog(parent)
{
    setupUi(this);
    this->setWindowTitle( QString::fromUtf8( "Ingresar pago de cuota" ) );
    this->setWindowIcon( QIcon( ":/imagenes/ingresar_pago.png" ) );
    this->setObjectName( "dialogo_pago_cuota" );

    _id_plan_cuota = -1;
    _model = 0;
    _emitir = false;

    DSBFaltante->setEnabled( false );
    DSBPagado->setEnabled( false );
    DSBTotal->setEnabled( false );
}

/*!
 * \brief DPagoCuota::setearPlanCuota
 * Setea el identificador del plan de cuota del cual se desea ingresar el pago.
 * \param id_plan_cuota Identificador del plan de cuota
 */
void DPagoCuota::setearPlanCuota( const int id_plan_cuota )
{
    if( id_plan_cuota > 0 )
    {
      this->_id_plan_cuota = id_plan_cuota;
        if( this->_model != 0 )
            this->cargarDatos();
    } else {
        qDebug( "DPagoCuota::setearPlanCuota:: Id plan de cuota incorrecto < 0 " );
    }
}

/*!
 * \brief DPagoCuota::setearModelo
 * Setea el modelo de planes de cuotas que hay que actualizar luego de ingresar el pago
 * \param m Modelo de la clase MVPlanCuota
 */
void DPagoCuota::setearModelo( MVPlanCuota *m )
{
    if( m != 0 ) {
        this->_model = m;
        if( this->_id_plan_cuota > 0 )
            this->cargarDatos();
    } else {
        qDebug( "DPagoCuotas::setearModelo: Puntero a modelo incorrecto" );
      }
}

void DPagoCuota::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

/*!
 * \brief DPagoCuota::cargarDatos
 * Carga los datos para el sistema.
 */
void DPagoCuota::cargarDatos()
{
    qDebug() << "Plan de cuotas: " << this->_id_plan_cuota;
    this->LRazonSocial->setText( MPlanCuota::obtenerRazonSocial( this->_id_plan_cuota ) );
    QPair<int,int> estado_cuotas = MPlanCuota::obtenerEstadoCuotas( this->_id_plan_cuota );
    this->LCDNTotal->display( estado_cuotas.second );
    this->LCDNPagadas->display( estado_cuotas.first );
    QPair<double,double> estado_plata = MPlanCuota::obtenerEstadoImportes( this->_id_plan_cuota );
    this->DSBTotal->setValue( estado_plata.second );
    this->DSBFaltante->setValue( estado_plata.second - estado_plata.first );
    this->DSBPagado->setValue( estado_plata.first );
    double porcentaje = ( estado_plata.first * 100 ) / estado_plata.second;
    this->PrBProgreso->setValue( porcentaje );
    // Busco si el recibo ya se encuentra emitido
    int num = MItemPlanCuota::buscarReciboEmitido( this->_id_plan_cuota );
    if( num == -2 ) {
        LRecibo->setText( "#"+MPagos::buscarNumeroComprobantePorId( num ).aCadena() );
        this->_emitir = false;
    } else {
        LRecibo->setText( "#"+MPagos::proximoSerieNumeroRecibo().aCadena() );
        this->_emitir = true;
    }
    double importe_cuota = MItemPlanCuota::obtenerProximoImporte( this->_id_plan_cuota );
    this->DSBImporte->setValue( importe_cuota );
    this->DSBPago->setValue( importe_cuota );
    this->_id_item_plan_cuota = MItemPlanCuota::obtenerIdProximaCuota( this->_id_plan_cuota );
}

/**
 * @brief DPagoCuota::accept
 * Función llamada cuando se presiona el aceptar en el dialogo
 */
void DPagoCuota::accept()
{
  // Cosas a verificar: coincidencia del total a pagar
  if( this->DSBImporte->value() != this->DSBPago->value() ) {
      QMessageBox::information( this, "Error", QString::fromUtf8( "El importe a pagar no corresponde con la cuota" ) );
      return;
  }
  if( this->_id_item_plan_cuota == -1 ) {
      abort();
  }

  // Genero la transacción
  QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).transaction();

  MPagos *m = new MPagos( this, false );
  int id_recibo = -1;
  if( this->_emitir ) {
      // Hay que emitir un recibo como pagado

      // Genero el recibo
      int id_cliente = MPlanCuota::obtenerIdCliente( this->_id_plan_cuota );
      QDate fecha_recibo = QDate::currentDate();
      QString contenido = QString( "Pago de cuota %1 de %2 del plan de cuotas #%3"). arg( this->LCDNPagadas->value() + 1 ).arg( this->LCDNTotal->value() ).arg( this->_id_plan_cuota );
      double total = this->DSBPago->value();
      id_recibo = m->agregarRecibo( id_cliente, fecha_recibo, contenido, total, CkBEfectivo->isChecked(), true, QString() );
      if ( id_recibo == -1 ) {
          QMessageBox::information( this, "Error", "El recibo No ha sido agregado correctamente" );
          QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).rollback();
      }
      // Actualizo la información en el registro de item de cuota con el dato del recibo y la fecha de pago

  } else {
      // El recibo ya está emitido y es necesario colocarlo como pagado
      id_recibo = MItemPlanCuota::buscarReciboEmitido( this->_id_item_plan_cuota );
      if( !m->setearComoPagado( id_recibo, CkBEfectivo->isChecked(), true ) ) {
          QMessageBox::information( this, "Error", "No se pudo setear como pagado el recibo emitido anteriormente" );
          QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).rollback();
      }
  }
  if( MItemPlanCuota::setearItemCuotaPagado( this->_id_item_plan_cuota, id_recibo ) ) {
      if( QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).commit() ) {
        QMessageBox::information( this, "Correcto", "El item de cuota fue registrado como pagado y el recibo emitido" );
        return;
      } else {
        QMessageBox::information( this, "Incorrecto", "No se pudo hacer el guardado de los datos en lotes" );
      }
  } else {
      QMessageBox::information( this, "Error", "El sistema no pudo registrar el item de cuota como pagado" );
  }
  QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).rollback();
  this->close();
  emit actualizarModelo();
  return;
}
