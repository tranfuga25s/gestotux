#include "formgenerarcomprobantescuotas.h"

#include "eactcerrar.h"
#include "mpagos.h"
#include "mitemplancuota.h"
#include "EReporte.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>

FormGenerarComprobantesCuotas::FormGenerarComprobantesCuotas( QWidget *parent ) :
    EVentana( parent )
{
    setupUi(this);
    this->setWindowTitle( "Generar comprobantes de cuotas" );
    //this->setWindowIcon( QIcon( ":/imagenes/nada.png" ) );

    this->GBProgreso->setVisible( false );

    // Inicializo el modelo de las cuotas
    modelo = new MGenerarCuotas( this );

    // Conecto los totales y demás
    connect( modelo, SIGNAL( cambioTotal( double ) ), DSBTotal, SLOT( setValue( double ) ) );
    connect( modelo, SIGNAL( cambioCantidad( int ) ), LCDNCantidad, SLOT( display( int ) ) );

    TVVista->setModel( modelo );
    TVVista->hideColumn( 0 );
    TVVista->setAlternatingRowColors( true );
    TVVista->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    TVVista->setSelectionBehavior( QAbstractItemView::SelectRows );

    ActCalcular = new QAction( this );
    ActCalcular->setText( "Calcular" );
    ActCalcular->setStatusTip( "Calcula las cuotas necesarias para el mes seleccionado" );
    connect( ActCalcular, SIGNAL( triggered() ), this, SLOT( calcularCuotas() ) );

    ActEmitir = new QAction( this );
    ActEmitir->setText( "Emitir" );
    ActEmitir->setStatusTip( "Emite los comprobantes necesarios" );
    connect( ActEmitir, SIGNAL( triggered() ), this, SLOT( emitirComprobantes() ) );

    addAction( ActCalcular );
    addAction( ActEmitir );
    addAction( new EActCerrar( this ) );

    SBMes->setValue( QDate::currentDate().month() );
}

void FormGenerarComprobantesCuotas::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * @brief FormGenerarComprobantesCuotas::calcularCuotas
 * Llama al sistema para que vea de generar las los datos para modelo
 */
void FormGenerarComprobantesCuotas::calcularCuotas()
{
  modelo->setearMesBusqueda( SBMes->value() );
  if( modelo->calcularComprobantes() ) {
    TVVista->update();
  } else {
      QMessageBox::information( this, "Sin datos", "No existen datos para el mes seleccionado" );
  }
}

void FormGenerarComprobantesCuotas::emitirComprobantes()
{
  // Genero la transaccion
  QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).transaction();

  // Deshabilito las acciones para que no pueda tocar nada mientras trabajo
  ActEmitir->setEnabled( false );
  ActCalcular->setEnabled( false );
  TVVista->setEnabled( false );

  // Muestro la barra de progreso
  GBProgreso->setVisible( true );

  // Coloco el rango ( busqueda de info = 1, emision = 2, guardado de datos = 3 )
  PgBEstado->setRange( 0, modelo->rowCount()*3 );

  MPagos *m = new MPagos( this, false );
  EReporte *rep = new EReporte( this );
  rep->recibo();
  ParameterList lista_parametros;
  // Recorro el modelo y genero los recibos
  int contador = 0;
  while( contador < modelo->rowCount() ) {
      // Busco los datos del modelo
      LProgreso->setText( QString( "Buscando información para el plan de cuota %1" )
                          .arg( modelo->data( modelo->index( contador, 1 ), Qt::DisplayRole ).toString() ) );

      int id_cliente = modelo->data( modelo->index( contador, 2 ), Qt::EditRole ).toInt();
      double monto = modelo->data( modelo->index( contador, 4 ), Qt::EditRole ).toDouble();
      QString contenido = QString( "Pago de cuota %1 del plan de cuotas #%2")
              .arg( modelo->data( modelo->index( contador, 3 ), Qt::EditRole ).toString() )
              .arg( modelo->data( modelo->index( contador, 1 ), Qt::EditRole ).toInt() );
      QDate fecha = QDate::currentDate();

      PgBEstado->setValue( PgBEstado->value() + 1 );
      LProgreso->setText( QString( "Emitiendo recibo Nº %1" ).arg( modelo->data( modelo->index( contador, 5 ), Qt::EditRole ).toString() ) );

      // Emito el recibo con los datos pero lo pongo como "A Pagar luego" o como pagado = false
      int id_recibo = m->agregarRecibo( id_cliente, fecha, contenido, monto, true, false, QString() );
      if ( id_recibo == -1 ) {
          QMessageBox::information( this, "Error", "El recibo No ha sido agregado correctamente" );
          QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).rollback();
          return;
      } else {
          lista_parametros.append( Parameter( "id_recibo", id_recibo ) );
          rep->hacer( lista_parametros, false, false );
      }

      PgBEstado->setValue( PgBEstado->value() + 1 );
      LProgreso->setText( "Recibo generado... Guardando relacion...." );

      // Guardo el numero de recibo en el registro de la cuota
      int id_item_plan_cuota = modelo->data( modelo->index( contador, 0 ), Qt::EditRole ).toInt();
      if( !MItemPlanCuota::setearReciboItemCuota( id_item_plan_cuota, id_recibo ) ) {
          QMessageBox::information( this, "Error", "No se pudo asociar el recibo con el item del plan de cuota" );
          QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).rollback();
      }
      PgBEstado->setValue( PgBEstado->value() + 1 );
      contador++;
      lista_parametros.clear();
  }
  PgBEstado->setValue( PgBEstado->maximum() );
  if( QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).commit() ) {
      QMessageBox::information( this, "Correcto", "La emisión de comprobantes fue correcta" );
      this->close();
      return;
  } else {
      QMessageBox::information( this, "Error", "Hubo un problema guardando los datos" );
      return;
  }

}
