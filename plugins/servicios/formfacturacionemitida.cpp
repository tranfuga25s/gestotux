#include "formfacturacionemitida.h"

#include "mservicios.h"
#include "mperiodoservicio.h"
#include "eactcerrar.h"
#include "EReporte.h"
#include "eregistroplugins.h"
#include "mfacturacionemitida.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QMessageBox>

FormFacturacionEmitida::FormFacturacionEmitida(QWidget *parent) :
    EVentana(parent)
{
    setupUi(this);
    setObjectName( "visor-facturacion-servicio" );
    setWindowTitle( QString::fromUtf8( "Facturación emitida de servicio" ) );

    // Cargo los servicios
    CBServicio->setearTabla( "servicios" );
    CBServicio->setearCampoId( "id_servicio" );
    CBServicio->setearCampoTexto( "nombre" );

    CBPeriodo->setearTabla( "periodo_servicio" );
    CBPeriodo->setearCampoId( "id_periodo_servicio" );
    if( QSqlDatabase::database( QSqlDatabase::defaultConnection, false ).driverName() == "QMYSQL" ) {
        CBPeriodo->setearCampoTexto( "CONCAT( periodo, '/', ano )" );
    } else {
        CBPeriodo->setearCampoTexto( " periodo || '/' || ano " );
    }

    mdeudor = new MFacturacionEmitida( true, this );
    mpagado = new MFacturacionEmitida( false, this );

    TVDeudor->setModel( mdeudor );
    TVDeudor->setAlternatingRowColors( true );
    TVDeudor->setSortingEnabled( true );
    TVDeudor->setSelectionMode( QAbstractItemView::SingleSelection );
    TVDeudor->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVDeudor->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    TVDeudor->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    TVDeudor->setTextElideMode( Qt::ElideRight );

    TVPagada->setModel( mpagado );
    TVPagada->setSelectionMode( QAbstractItemView::SingleSelection );
    TVPagada->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVPagada->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    TVPagada->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    TVPagada->setTextElideMode( Qt::ElideRight );
    TVPagada->setAlternatingRowColors( true );
    TVPagada->setSortingEnabled( true );

    ActVerPagado = new QAction( this );
    ActVerPagado->setCheckable( true );
    ActVerPagado->setChecked( false );
    ActVerPagado->setText( "Ver pagados" );
    connect( ActVerPagado, SIGNAL( toggled( bool ) ), this, SLOT( cambioHabilitadoPagados( bool ) ) );

    ActListadoGeneral = new QAction( this );
    ActListadoGeneral->setText( "Listado Deudores" );
    ActListadoGeneral->setIcon( QIcon( ":/imagenes/imprimir.png" ) );
    ActListadoGeneral->setStatusTip( "Imprime un listado de todos los deudores de todos los periodos del servicio seleccionado" );
    connect( ActListadoGeneral, SIGNAL( triggered() ), this, SLOT( imprimirListadoGeneral() ) );

    ActListadoGeneralPDF = new QAction( this );
    ActListadoGeneralPDF->setText( "Listado Deudores" );
    ActListadoGeneralPDF->setIcon( QIcon( ":/imagenes/acroread.png" ) );
    ActListadoGeneralPDF->setStatusTip( "Imprime un listado de todos los deudores de todos los periodos del servicio seleccionado" );
    connect( ActListadoGeneralPDF, SIGNAL( triggered() ), this, SLOT( imprimirListadoGeneralPDF() ) );

    GBPagado->setVisible( ActVerPagado->isChecked() );

    addAction( ActListadoGeneral );
    addAction( ActListadoGeneralPDF );
    addAction( ActVerPagado );
    addAction( new EActCerrar( this ) );
}

void FormFacturacionEmitida::setearIdServicio( const int id_servicio )
{
    _id_servicio = id_servicio;
    CBServicio->setearId( id_servicio );
    CBPeriodo->setearFiltro( QString( " WHERE id_servicio = %1" ).arg( _id_servicio ) );
    connect( CBServicio, SIGNAL( cambioId( int ) ), this, SLOT( cambioServicio( int ) ) );
}

void FormFacturacionEmitida::setearIdPeriodo( const int id_periodo )
{
    _id_periodo_servicio = id_periodo;
    CBPeriodo->setearId( id_periodo );
    cargarDatos();
    connect( CBPeriodo , SIGNAL( cambioId( int ) ), this, SLOT( cambioPeriodo(  int ) ) );
}

void FormFacturacionEmitida::changeEvent(QEvent *e)
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

void FormFacturacionEmitida::cambioServicio( int id_servicio )
{
  if( id_servicio <= 0 )
      return;

  _id_servicio = id_servicio;
  CBPeriodo->setearFiltro( QString(" WHERE id_servicio = %1" ).arg( _id_servicio ) );
  this->mpagado->clear();
  this->mdeudor->clear();
}

void FormFacturacionEmitida::cambioPeriodo( int id_periodo )
{
    _id_periodo_servicio = id_periodo;
    cargarDatos();
}

void FormFacturacionEmitida::cargarDatos()
{
    this->mpagado->setearServicio( _id_servicio );
    this->mpagado->setearPeriodo( _id_periodo_servicio );
    if( ActVerPagado->isChecked() ) {
        this->mpagado->generarDatos();
    }

    this->mdeudor->setearServicio( _id_servicio );
    this->mdeudor->setearPeriodo( _id_periodo_servicio );
    this->mdeudor->generarDatos();
}

void FormFacturacionEmitida::cambioHabilitadoPagados( bool estado )
{
    if( estado ) {
        GBPagado->setVisible( true );
        this->mpagado->generarDatos();
    } else {
        mpagado->clear();
        GBPagado->setVisible( false );
    }
}

void FormFacturacionEmitida::imprimirListadoGeneral()
{
    // Busco el id que está del servicio
    EReporte *rep = new EReporte( 0 );
    ParameterList lista;
    lista.append( "id_servicio", CBServicio->idActual() );
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
        rep->especial( "ListaDeudoresServicioHC", lista );
    } else {
        rep->especial( "ListaDeudoresServicio", lista );
    }
    rep->hacer();
    delete rep;
}

void FormFacturacionEmitida::imprimirListadoGeneralPDF()
{
    // Busco el id que está del servicio
    EReporte *rep = new EReporte( 0 );
    ParameterList lista;
    int id_servicio = CBServicio->idActual();
    lista.append( "id_servicio", id_servicio );
    if( ERegistroPlugins::getInstancia()->existePluginExterno( "hicomp" ) ) {
        rep->especial( "ListaDeudoresServicioHC", lista );
    } else {
        rep->especial( "ListaDeudoresServicio", lista );
    }
    rep->hacerPDF( lista, QString( "Lista Deudores del servicio %1 al %2" ).arg( MServicios::getNombreServicio( id_servicio ) ).arg( QDate::currentDate().toString( Qt::SystemLocaleShortDate ) ) );
    delete rep;
}
