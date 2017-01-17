#include "DeudoresServicios.h"
#include "EReporte.h"
#include "mclientes.h"

#include <QDate>
#include <QMessageBox>
#include <QInputDialog>

/*!
 * \brief DeudoresServicios::version
 * \return
 */
double DeudoresServicios::version() const
{ return 0.1; }

/*!
 * \brief DeudoresServicios::nombre
 * \return
 */
QString DeudoresServicios::nombre() const
{ return "DeudoresServicios"; }

/*!
 * \brief ReportesCuotas::inicializar
 * \return
 */
bool DeudoresServicios::inicializar()
{
    ActResumenServiciosDeudaHistorica = new QAction( this );
    ActResumenServiciosDeudaHistorica->setText( "Deuda historica general" );
    ActResumenServiciosDeudaHistorica->setStatusTip( "Muestra un resumen de los estados de todas los servicios" );
    ActResumenServiciosDeudaHistorica->setIcon( QIcon( ":/imagenes/resumen_cuotas.png" ) );
    connect( ActResumenServiciosDeudaHistorica, SIGNAL( triggered() ), this, SLOT( resumenServiciosDeudaHistorica() ) );

    ActResumenServiciosMes = new QAction( this );
    ActResumenServiciosMes->setText( "Deuda por servicio" );
    ActResumenServiciosMes->setStatusTip( "Muestra el resumen de las cuotas por mes" );
    connect( ActResumenServiciosMes, SIGNAL( triggered() ), this, SLOT( resumenServiciosPorServicio() ) );

    ActResumenServiciosCliente = new QAction( this );
    ActResumenServiciosCliente->setText( "Deuda por cliente" );
    ActResumenServiciosCliente->setStatusTip( "Muestra las deudas de un cliente especifico" );
    connect( ActResumenServiciosCliente, SIGNAL( triggered() ), this, SLOT( resumenServiciosCliente() ) );

    return true;
}

/*!
 * \brief DeudoresServicios::hacerMenu
 * \param m
 */
void DeudoresServicios::hacerMenu( QMenu *m )
{
    QMenu *menu = m->addMenu( "Servicios" );
    menu->addAction( ActResumenServiciosDeudaHistorica );
    menu->addAction( ActResumenServiciosMes );
    menu->addAction( ActResumenServiciosCliente );
}

 #include "dialogoresumenservicioshistorico.h"
/*!
 * \brief DeudoresServicios::resumenServiciosDeudaHistorica
 */
void DeudoresServicios::resumenServiciosDeudaHistorica()
{
    if (!this->existenServicios() ) {
        return;
    }
    DialogoResumenServiciosHistorico *dialogo = new DialogoResumenServiciosHistorico();
    dialogo->exec();
}

#include "dialogoresumenporservicio.h"
/*!
 * \brief DeudoresServicios::resumenServiciosPorServicio
 */
void DeudoresServicios::resumenServiciosPorServicio()
{
    if (!this->existenServicios() ) {
        return;
    }
    DialogoResumenPorServicio *dialogo = new DialogoResumenPorServicio();
    dialogo->exec();
}

#include "dialogodeudahistoricaservicioporcliente.h"
/*!
 * \brief DeudoresServicios::resumenServicios Cliente
 * Muestra el resumen de cuotas de un cliente específico
 */
void DeudoresServicios::resumenServiciosCliente()
{
    if (!this->existenServicios() ) {
        return;
    }
    DialogoDeudaHistoricaServicioPorCliente *dialogo = new DialogoDeudaHistoricaServicioPorCliente();
    dialogo->exec();
}

#include "mservicios.h"
#include <QMessageBox>

/*!
 * \brief DeudoresServicios::existenServicios
 */
bool DeudoresServicios::existenServicios() {
    if (MServicios::listaDeServicios().length() == 0) {
        QMessageBox::warning(0, "No hay servicios", "No hay ningun servicio disponible para generar el resumen");
        return false;
    } else {
        return true;
    }
}

Q_EXPORT_PLUGIN2( deudoresservicios, DeudoresServicios )
