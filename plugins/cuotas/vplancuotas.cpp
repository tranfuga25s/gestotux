#include "vplancuotas.h"

#include "formsimularcuotas.h"
#include "formdetallecuotas.h"
#include "formadelantocuotas.h"
#include "mvplancuota.h"
#include "DPagoCuota.h"

#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>

VPlanCuotas::VPlanCuotas(QWidget *parent) :
 EVLista(parent)
{
    this->setObjectName( "visorplancuotas" );
    this->setWindowTitle( "Planes de cuota activos" );
    this->setWindowIcon( QIcon( ":/imagenes/cuotas_detalle.png" ) );

    ActSimular = new QAction( this );
    ActSimular->setText( "Simular" );
    ActSimular->setStatusTip( "Simular un plan de cuotas" );
    ActSimular->setIcon( QIcon( ":/imagenes/simular_cuota.png" ) );
    connect( ActSimular, SIGNAL( triggered() ), this, SLOT( simular() ) );

    ActTerminado = new QAction( this );
    ActTerminado->setText( "Terminados" );
    ActTerminado->setCheckable( true );
    //ActTerminado->setIcon( QIcon( ":/imagenes/" ) );
    connect( ActTerminado, SIGNAL( toggled( bool ) ), this, SLOT( cambioTerminado( bool ) ) );

    ActDetalles = new QAction( this );
    ActDetalles->setText( "Detalles" );
    ActDetalles->setStatusTip( "Ver los detalles de un plan de cuotas seleccionado" );
    ActDetalles->setIcon( QIcon( ":/imagenes/cuotas_detalle.png" ) );
    connect( ActDetalles, SIGNAL( triggered() ), this, SLOT( verDetalles() ) );

    ActIngresarPago = new QAction( this );
    ActIngresarPago->setText( "Pago" );
    ActIngresarPago->setIcon( QIcon( ":/imagenes/ingresar_pago.png" ) );
    ActIngresarPago->setStatusTip( "Ingresa un nuevo pago mediante un recibo" );
    connect( ActIngresarPago, SIGNAL( triggered() ), this, SLOT( ingresarPago() ) );

    ActIngresarAdelanto = new QAction( this );
    ActIngresarAdelanto->setText( "Adelanto" );
    ActIngresarAdelanto->setIcon( QIcon( ":/imagenes/adelanto_cuota.png" ) );
    connect( ActIngresarAdelanto, SIGNAL( triggered() ), this, SLOT( ingresarAdelanto() ) );

    ActCancelar = new QAction( this );
    ActCancelar->setText( "Cancelar" );
    ActCancelar->setIcon( QIcon( ":/imagenes/cancelar_cuota.png" ) );
    ActCancelar->setStatusTip( "Cancelar un plan de cuotas" );
    connect( ActCancelar, SIGNAL( triggered() ), this, SLOT( cancelar() ) );

    QAction *ActSep = new QAction( this );
    ActSep->setSeparator( true );
    QAction *ActSep2 = new QAction( ActSep );
    ActSep2->setSeparator( true );

    addAction( ActSimular );
    addAction( ActSep );
    addAction( ActDetalles );
    addAction( ActIngresarPago );
    addAction( ActIngresarAdelanto );
    addAction( ActCancelar );
    addAction( ActCerrar );
    addAction( ActSep2 );
    addAction( ActTerminado );
    addAction( ActBuscar );

    modelo = new MVPlanCuota( this );
    vista->setModel( modelo );

    modelo->select();

    agregarFiltroBusqueda( QString::fromUtf8( "Razón Social" ), "cliente LIKE '%%%1%'" );
    habilitarBusqueda();
}

void VPlanCuotas::actualizar()
{
    modelo->select();
    vista->update();
}

/*!
 * \fn VPlanCuotas::simular()
 * Muestra la ventana de simular cuotas
 */
void VPlanCuotas::simular()
{ emit agregarVentana( new FormSimularCuotas() ); }

/*!
 * \fn VPlanCuotas::cambioTerminado( bool e )
 * Muestra los planes terminados en la lista de planes
 * \param e Estado de la accion
 */
void VPlanCuotas::cambioTerminado( bool e )
{
    if( e ) {
        this->modelo->setFilter( " total_faltante = 0 " );
    } else {
        this->modelo->setFilter( "" );
    }
    this->modelo->select();
}

/*!
 * \fn VPlanCuotas::verDetalles()
 * Muestra la lista de cuotas del plan y su estado
 */
void VPlanCuotas::verDetalles()
{
    // Busco el plan de cuota seleccionado
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Por favor, seleccione una sola fila para ver su detalle" );
        return;
    }
    QModelIndex idx = this->vista->selectionModel()->selectedRows().first();
    int id_plan = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();
    FormDetalleCuotas *f = new FormDetalleCuotas();
    f->setearIdPlanCuota( id_plan );
    emit agregarVentana( f );
}

/*!
 * \fn VPlanCuotas::cancelar()
 * Cancela un plan o planes de cuota seleccionados
 */
void VPlanCuotas::cancelar()
{
    // Busco el plan de cuota seleccionado
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Por favor, seleccione una sola fila para ver su detalle" );
        return;
    }

    int resp = QMessageBox::question( this,
                                      QString::fromUtf8( "¿Está seguro?" ),
                                      QString::fromUtf8( "¿Está seguro que desea cancelar %1 plan de cuotas? El sistema cancelará todos los proximos pagos que no hayan sido emitidos. EL SISTEMA NO MODIFICARÁ LA CUENTA CORRIENTE DEL CLIENTE!" ).arg( this->vista->selectionModel()->selectedRows().size() )
                                    );
    if( resp == QMessageBox::Ok ) {
        // Cancelando el plan de cuota
        // Inicio la transacción
        QSqlDatabase::database( QSqlDatabase::defaultConnection, true ).transaction();
        foreach( QModelIndex idx, this->vista->selectionModel()->selectedRows() ) {
            int id_plan_cuota = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();
            bool ok = false;
            QString razon = QInputDialog::getText( this,
                                                   "Razon de cancelacion",
                                                   QString( "Ingrese la razon de cancelacion del plan de cuotas %1" ).arg( id_plan_cuota ),
                                                   QLineEdit::Normal,
                                                   QString(),
                                                   &ok );
            if( !MPlanCuota::cancelarPlan( id_plan_cuota, razon ) ) {
                QMessageBox::warning( this, "Error", QString::fromUtf8( "No se pudo eliminar el plan de cuotas %1" ).arg( id_plan_cuota ) );
                QSqlDatabase::database().rollback();
                return;
            } else {
                qDebug() << "Plan de cuotas " << id_plan_cuota << " cancelado correctamente";
            }
        }
        if( QSqlDatabase::database().commit() ) {
            QMessageBox::information( this, "Correcto", "La cancelación fue correcta" );
        } else {
            QMessageBox::information( this, "Incorrecto", "No se pudieron guardar los datos cancelados" );
        }
    }
}

/*!
 * \fn VPlanCuotas::ingresarPago()
 * Slot llamado para ingresar a la ventana de ingreso de un pago.
 */
void VPlanCuotas::ingresarPago()
{
    // Busco el plan de cuota seleccionado
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Por favor, seleccione una sola fila para ver su detalle" );
        return;
    }
    QModelIndex idx = this->vista->selectionModel()->selectedRows().first();
    int id_plan_cuota = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();

    DPagoCuota *dialogo = new DPagoCuota( this );
    dialogo->setearPlanCuota( id_plan_cuota );
    dialogo->setearModelo( qobject_cast<MVPlanCuota *>(this->modelo) );
    connect( dialogo, SIGNAL( actualizarModelo() ), this, SLOT( actualizar() ) );
    dialogo->exec();
    this->actualizar();
}

/*!
 * \brief VPlanCuotas::ingresarAdelanto
 * Slot llamado para abrir la ventana de ingresar un adelanto a un plan de cuota
 */
void VPlanCuotas::ingresarAdelanto()
{
    // Busco el plan de cuota seleccionado
    if( this->vista->selectionModel()->selectedRows().isEmpty() ) {
        QMessageBox::warning( this, "Error", "Por favor, seleccione una sola fila para ver su detalle" );
        return;
    }
    QModelIndex idx = this->vista->selectionModel()->selectedRows().first();
    int id_plan_cuota = idx.model()->data( idx.model()->index( idx.row(), 0 ), Qt::EditRole ).toInt();

    FormAdelantoCuotas *f = new FormAdelantoCuotas( this );
    f->setearIdPlanCuota( id_plan_cuota );
    connect( f, SIGNAL( actualizarModelo() ), this, SLOT( actualizar() ) );
    emit agregarVentana( f );
}
