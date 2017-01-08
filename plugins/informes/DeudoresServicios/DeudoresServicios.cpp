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
    DialogoResumenServiciosHistorico *dialogo = new DialogoResumenServiciosHistorico();
    dialogo->exec();
}

/*!
 * \brief DeudoresServicios::resumenServiciosPorServicio
 */
void DeudoresServicios::resumenServiciosPorServicio()
{
    QMessageBox::information( 0, "Error", "No implementado" );
    return;
    bool ok = false;
    QStringList meses;
    meses << "Enero" << "Febrero" << "Marzo" << "Abril" << "Mayo" << "Junio" << "Julio" << "Agosto" << "Septiembre" << "Octubre" << "Noviembre" << "Diciembre";
    QString mes = QInputDialog::getItem( 0, "Elija el mes", "Elija el mes:", meses, QDate::currentDate().month(), false, &ok );
    if( ok ) {
        EReporte *rep = new EReporte( 0 );
        ParameterList lista;
        lista.append( Parameter( "mes", meses.indexOf( mes ) ) );
        rep->especial( "DeudaCuotasMes", lista );
        rep->hacerPDF( ParameterList(), QString( "Deudas Cuotas del mes %1-%2" ).arg( mes ).arg( QDate::currentDate().toString( "yyyy" ) ) );
        delete rep;
    }
}

/*!
 * \brief DeudoresServicios::resumenServicios Cliente
 * Muestra el resumen de cuotas de un cliente específico
 */
void DeudoresServicios::resumenServiciosCliente()
{
    QMessageBox::information( 0, "Error", "No implementado" );
    return;
    /// @TODO: Implementar reporte de deuda segun cliente
    EReporte *rep = new EReporte( 0 );
    int id_cliente = 1;
    ParameterList lista;
    lista.append( "id_cliente", id_cliente );
    rep->especial( "deudascuotascliente", lista );
    rep->hacerPDF( ParameterList(),
                   QString( "Deudas de Cuotas de %2 Al %1" )
                   .arg( QDate::currentDate().toString( Qt::LocaleDate ) )
                   .arg( MClientes::getRazonSocial( id_cliente ) ) );
    delete rep;
}

Q_EXPORT_PLUGIN2( deudoresservicios, DeudoresServicios )
