#include "reportecompras.h"
#include "EReporte.h"

#include "formresumencomprastotales.h"

#include <QDate>
#include <QMessageBox>
#include <QInputDialog>

/*!
 * \brief ReporteCompras::version
 * \return
 */
double ReporteCompras::version() const
{ return 0.1; }

/*!
 * \brief ReporteCompras::nombre
 * \return
 */
QString ReporteCompras::nombre() const
{ return "ReporteCompras"; }

/*!
 * \brief ReporteCompras::inicializar
 * \return
 */
bool ReporteCompras::inicializar()
{
    Q_INIT_RESOURCE( resumencompras );

    ActResumenComprasTotales = new QAction( this );
    ActResumenComprasTotales->setText( "Totales" );
    ActResumenComprasTotales->setStatusTip( "Muestra un resumen total de compras" );
    ActResumenComprasTotales->setIcon( QIcon( ":/imagenes/resumen_cuotas.png" ) );
    connect( ActResumenComprasTotales, SIGNAL( triggered() ), this, SLOT( resumenComprasTotales() ) );

    ActResumenComprasPorProveedor = new QAction( this );
    ActResumenComprasPorProveedor->setText( "Compras por proveedor" );
    ActResumenComprasPorProveedor->setStatusTip( "Muestra el resumen de las compras realizadas según proveedor" );
    connect( ActResumenComprasPorProveedor, SIGNAL( triggered() ), this, SLOT( resumenComprasPorProveedor() ) );

    ActResumenComprasPorProducto = new QAction( this );
    ActResumenComprasPorProducto->setText( "Compras por producto" );
    ActResumenComprasPorProducto->setStatusTip( "Muestra las compras realizadas por producto especifico" );
    connect( ActResumenComprasPorProducto, SIGNAL( triggered() ), this, SLOT( resumenComprasPorProducto() ) );

    return true;
}

/*!
 * \brief ReporteCompras::hacerMenu
 * \param m
 */
void ReporteCompras::hacerMenu( QMenu *m )
{
    QMenu *menu = m->addMenu( "Compras" );
    menu->addAction( ActResumenComprasTotales );
    menu->addAction( ActResumenComprasPorProveedor );
    menu->addAction( ActResumenComprasPorProducto );
}

/*!
 * \brief ReporteCompras::resumenComprasTotales
 */
void ReporteCompras::resumenComprasTotales()
{
    emit agregarVentana( new FormResumenComprasTotales() );
}

/*!
 * \brief ReporteCompras::resumenComprasPorProducto
 */
void ReporteCompras::resumenComprasPorProducto()
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

/**
 * @brief ReporteCompras::seCierraGestotux
 */
void ReporteCompras::seCierraGestotux()
{
    Q_CLEANUP_RESOURCE( resumencompras );
}

/*!
 * \brief ReporteCompras::resumenComprasPorProveedor
 * Muestra el resumen de compras realizadas por proveedor
 */
void ReporteCompras::resumenComprasPorProveedor()
{
    QMessageBox::information( 0, "Error", "No implementado" );
    return;
    /// @TODO: Implementar reporte de deuda segun cliente
    EReporte *rep = new EReporte( 0 );
    int id_cliente = 1;
    ParameterList lista;
    lista.append( "id_cliente", id_cliente );
    rep->especial( "deudascuotascliente", lista );
    /*rep->hacerPDF( ParameterList(),
                   QString( "Deudas de Cuotas de %2 Al %1" )
                   .arg( QDate::currentDate().toString( Qt::LocaleDate ) )
                   .arg( MClientes::getRazonSocial( id_cliente ) ) ); */
    delete rep;
}

Q_EXPORT_PLUGIN2( reportecompras, ReporteCompras )
