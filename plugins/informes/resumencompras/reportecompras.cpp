#include "reportecompras.h"
#include "EReporte.h"

#include "formresumencomprastotales.h"
#include "formresumencomprasproveedor.h"

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

#include "formresumencomprasporproductos.h"
/*!
 * \brief ReporteCompras::resumenComprasPorProducto
 */
void ReporteCompras::resumenComprasPorProducto()
{
    emit agregarVentana( new FormResumenComprasPorProductos() );
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
    emit agregarVentana( new FormResumenComprasProveedor() );
}

Q_EXPORT_PLUGIN2( reportecompras, ReporteCompras )
