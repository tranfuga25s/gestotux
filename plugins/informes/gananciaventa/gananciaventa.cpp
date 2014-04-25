#include "gananciaventa.h"
#include "EReporte.h"
#include "formrangofechas.h"
#include "util.h"

#include <QDateTime>
#include <QDate>
#include <QTime>

double GananciaVenta::version() const
{ return 0.5; }

QString GananciaVenta::nombre() const
{ return "GananciaVenta"; }

bool GananciaVenta::inicializar()
{
    ActResumenTotal = new QAction( this );
    ActResumenTotal->setText( "Ganancias general" );
    ActResumenTotal->setToolTip( "Genera un resumen de ganancias de todo el sistema" );
    connect( ActResumenTotal, SIGNAL( triggered() ), this, SLOT( hacerResumenTotal() ) );

    ActResumenFechas = new QAction( this );
    ActResumenFechas->setText( "Ganancias entre fechas" );
    ActResumenFechas->setToolTip( "Genera un balance de ventas y ganancias entre dos fechas específicas" );
    connect( ActResumenFechas, SIGNAL( triggered() ), this, SLOT( hacerResumenFechas() ) );

    ActResumenDia = new QAction( this );
    ActResumenDia->setText( QString::fromUtf8( "Ganancias del día" ) );
    ActResumenDia->setToolTip( "Genera un resumen del día de hoy" );
    connect( ActResumenDia, SIGNAL( triggered() ), this, SLOT( hacerResumenDia() ) );

    return true;
}

void GananciaVenta::hacerMenu( QMenu *m )
{
    QMenu *r = m->addMenu( "Ganancia" );
    r->addAction( ActResumenTotal );
    r->addAction( ActResumenFechas );
    r->addAction( ActResumenDia );
}

/*!
 * \brief GananciaVenta::hacerResumenTotal
 * Genera un resumen con toda la información del sistema.
 */
void GananciaVenta::hacerResumenTotal()
{
    QDate min_venta, min_compra, max_compra, max_venta;
    min_venta = MFactura::obtenerFechaMinimaVenta().addDays( -1 );
    min_compra = MCompra::obtenerFechaMinimaCompra().addDays( -1 );
    max_venta = MFactura::fechaUltimaVenta().addDays( 1 );
    max_compra = MCompra::obtenerFechaMaximaCompra().addDays( 1 );
    EReporte *rep = new EReporte( this );
    ParameterList lista;
    lista.append( "fecha_inicio", Util::formateoFechas( qMin( min_venta, min_compra ) ) );
    lista.append( "fecha_fin", Util::formateoFechas( qMax( max_venta, max_compra ) ) );
    rep->especial( "ListadoGananciaVenta", lista );
    rep->hacer();
}

/*!
 * \brief GananciaVenta::hacerResumenFechas
 * Hace el resumen de ganancia y ventas entre fechas designadas
 */
void GananciaVenta::hacerResumenFechas()
{
    FormRangoFechas *f = new FormRangoFechas();
    f->exec();
    EReporte *rep = new EReporte( this );
    ParameterList lista;
    lista.append( "fecha_inicio", Util::formateoFechas( f->fechaInicio()  ) );
    lista.append( "fecha_fin", Util::formateoFechas( f->fechaFin() ) );
    rep->especial( "ListadoGananciaVenta", lista );
    rep->hacer();
    delete f;
}

/*!
 * \brief GananciaVenta::hacerResumenDia
 * Hace el resumen de ventas y ganancias para el día de hoy
 */
void GananciaVenta::hacerResumenDia()
{
    EReporte *rep = new EReporte( this );
    ParameterList lista;
    QTime hi( 0, 0, 0, 0 );
    QTime hf( 23, 59, 59 );
    QDateTime inicio = QDateTime::currentDateTime();
    QDateTime fin = QDateTime::currentDateTime();
    inicio.setTime( hi );
    fin.setTime( hf );
    lista.append( "fecha_inicio", Util::formateoFechas( inicio ) );
    lista.append( "fecha_fin", Util::formateoFechas( fin ) );
    rep->especial( "ListadoGananciaVenta", lista );
    rep->hacer();
}

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
/*!
 * \brief MFactura::obtenerFechaMinimaVenta
 * Devuelve la fecha de la primera factura
 * \return Fecha de la primera factura
 */
QDate MFactura::obtenerFechaMinimaVenta()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT fecha FROM factura ORDER BY fecha ASC LIMIT 1" ) ) {
        if( cola.next() ) {
            return cola.record().value(0).toDate();
        }
    } else {
        qWarning( "Error al buscar la fecha de la factura minima" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate( 1900, 1, 1);
}

/*!
 * \brief MCompra::obtenerFechaMinimaCompra
 * Devuelve la fecha minima de compra echa
 * \return fecha de primera compra
 */
QDate MCompra::obtenerFechaMinimaCompra()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT fecha FROM compras ORDER BY fecha ASC LIMIT 1" ) ) {
        if( cola.next() )
            return cola.record().value(0).toDate();
    } else {
        qWarning( "Error al buscar el minimo de compra fecha" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate( 1900, 1, 1 );
}

/*!
 * \brief MCompra::obtenerFechaMaximaCompra
 * Busca la fecha de la compra más actual
 * \return fecha de la ultima compra
 */
QDate MCompra::obtenerFechaMaximaCompra()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT fecha FROM compras ORDER BY fecha DESC LIMIT 1" ) ) {
        if( cola.next() )
            return cola.record().value(0).toDate();
    } else {
        qWarning( "Error al buscar el minimo de compra fecha" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate( 1900, 1, 1 );
}

QDate MFactura::fechaUltimaVenta()
{
    QSqlQuery cola;
    if( cola.exec( "SELECT fecha FROM factura ORDER BY id_factura DESC LIMIT 1" ) ) {
        cola.next();
        return cola.record().value(0).toDate();
    } else {
        qWarning( "Error al buscar la fecha de la ultima factura" );
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return QDate();
}

Q_EXPORT_PLUGIN2( gananciaventa, GananciaVenta )
