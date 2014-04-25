#include "formsimularcuotas.h"

#include "msimularcuotas.h"
#include "mplancuota.h"
#include "eactcerrar.h"
#include "preferencias.h"
#include "eregistroplugins.h"
#include "einfoprogramainterface.h"
#include "mclientes.h"

#include <QTextTable>
#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

FormSimularCuotas::FormSimularCuotas(QWidget *parent) :
EVentana(parent), Ui::FormSimularCuotasBase()
{
    setupUi(this);
    setWindowTitle( "Simulador de cuotas" );
    setWindowIcon( QIcon( ":/imagenes/simular_cuota.png" ) );

    ActSimular = new QAction( this );
    ActSimular->setText( "Simular" );
    ActSimular->setStatusTip( "Simula los pagos segÃºn los datos ingresados" );
    ActSimular->setIcon( QIcon( ":/imagenes/simular_cuota.png" ) );
    connect( ActSimular, SIGNAL( triggered() ), this, SLOT( simular() ) );

    ActImprimir = new QAction( this );
    ActImprimir->setText( "Imprimir" );
    ActImprimir->setText( QString::fromUtf8( "Imprimir" ) );
    ActImprimir->setIcon( QIcon( ":/imagenes/impresora.png" ) );
    connect( ActImprimir, SIGNAL( triggered() ), this, SLOT( imprimir() ) );

    ActPdf = new QAction( this );
    ActPdf->setText( "PDF" );
    ActPdf->setStatusTip( "Convierte a PDF la simulacion actual" );
    ActPdf->setIcon( QIcon( ":/imagenes/acroread.png" ) );
    connect( ActPdf, SIGNAL( triggered() ), this, SLOT( pdf() ) );

    ActConfirmar = new QAction( this );
    ActConfirmar->setText( "Confirmar" );
    connect( ActConfirmar, SIGNAL( triggered() ), this, SLOT( confirmar() ) );

    modelo = new MSimularCuotas( this );

    connect( DSBEntrega, SIGNAL( valueChanged( double ) ), this, SLOT( cambioEntrega( double ) ) );
    connect( DSBImporte, SIGNAL( valueChanged( double ) ), this, SLOT( cambioImporte( double ) ) );
    connect( DSBInteres, SIGNAL( valueChanged( double ) ), this, SLOT( cambioInteres( double ) ) );

    connect( SBCantidad, SIGNAL( valueChanged( int ) ), this, SLOT( cambioCantidad( int ) ) );

    connect( CBPeriodo , SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioPeriodo( int ) ) );

    connect( DEInicio  , SIGNAL( dateChanged( QDate ) ), this, SLOT( cambioFechaInicio( QDate ) ) );

    CBPeriodo->insertItem( MPlanCuota::Semanal      , "Semanal"       , MPlanCuota::Semanal       );
    CBPeriodo->insertItem( MPlanCuota::Quincenal    , "Quincenal"     , MPlanCuota::Quincenal     );
    CBPeriodo->insertItem( MPlanCuota::Mensual      , "Mensual"       , MPlanCuota::Mensual       );
    CBPeriodo->insertItem( MPlanCuota::BiMensual    , "Bimensual"     , MPlanCuota::BiMensual     );
    CBPeriodo->insertItem( MPlanCuota::Trimestral   , "Trimestral"    , MPlanCuota::Trimestral    );
    CBPeriodo->insertItem( MPlanCuota::Cuatrimestral, "Cuatrimestral" , MPlanCuota::Cuatrimestral );
    CBPeriodo->insertItem( MPlanCuota::Seximestral  , "Semestral"     , MPlanCuota::Seximestral   );
    CBPeriodo->insertItem( MPlanCuota::Anual        , "Anual"         , MPlanCuota::Anual         );

    DEInicio->setDate( QDate::currentDate() );

    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "Cuotas" );
    CBPeriodo->setCurrentIndex( p->value( "id-periodo", 2 ).toInt() );
    DSBInteres->setValue( p->value( "interes", 10.00 ).toDouble() );
    //CkBInicioPeriodo->setChecked( p->value( "inicio-periodo", false ).toBool() );
    p->endGroup();
    p->endGroup();
    p = 0;

    addAction( ActSimular );
    addAction( ActImprimir );
    addAction( ActPdf );
    addAction( new EActCerrar( this ) );

    _id_cliente = -1;
    _tipo_comprobante = MPlanCuota::Factura;
}

FormSimularCuotas::~FormSimularCuotas()
{
    if( !documento )
        delete documento;

}

void FormSimularCuotas::setearConfirmar(bool conf)
{
    if( conf ) {
        this->addAction( ActConfirmar );
    }
}

void FormSimularCuotas::changeEvent(QEvent *e)
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

/*!
 * \fn FormSimularCuotas::simular()
 * Genera todos los datos para la simulaciÃ³n
 */
void FormSimularCuotas::simular()
{
    DSBTotal->setValue( DSBImporte->value() - DSBEntrega->value() );
    modelo->setImporte    ( DSBTotal->value()   );
    modelo->setCuotas     ( SBCantidad->value() );
    modelo->setPeriodo    ( (MPlanCuota::Periodo) CBPeriodo->itemData( CBPeriodo->currentIndex() ).toInt() );
    modelo->setInteres    ( DSBInteres->value() );
    modelo->setFechaInicio( DEInicio->date()    );
    modelo->setPagoInicial( DSBEntrega->value() );
    TVSimulacion->setModel( modelo );
}

/*!
 * \fn FormSimularCuotas::imprimir()
 * Imprime el listado de cuotas tal cual se ingresaron en los datos.
 */
void FormSimularCuotas::imprimir()
{
    this->generaReporte();
    QPrinter *impresora = new QPrinter( QPrinterInfo::defaultPrinter() );
    QPrintDialog *pd = new QPrintDialog( impresora );
    if( pd->exec() == QDialog::Accepted ) {
        documento->print( pd->printer() );
    }
    delete impresora;
    delete pd;
}


void FormSimularCuotas::generaReporte()
{
    documento = new QTextDocument();
    QTextCursor cursor( documento );
    int cant_filas = 3 + SBCantidad->value();
    QTextTable *tabla = cursor.insertTable( cant_filas, 5 );
    QTextTableFormat formatoTabla = tabla->format();
    formatoTabla.setHeaderRowCount( 1 );
    formatoTabla.setWidth( QTextLength( QTextLength::PercentageLength, 100 ) );
    formatoTabla.setBorderStyle( QTextFrameFormat::BorderStyle_Solid );
    formatoTabla.setBorder( 1 );
    formatoTabla.setCellPadding( 3 );
    formatoTabla.setCellSpacing( 0 );
    tabla->setFormat( formatoTabla );
    tabla->cellAt( 0,0 ).firstCursorPosition().insertHtml( "<b> # Cuota</b>" );
    tabla->cellAt( 0,1 ).firstCursorPosition().insertHtml( "<b> Fecha de pago </b>" );
    tabla->cellAt( 0,2 ).firstCursorPosition().insertHtml( "<b> Cuota </b>" );
    tabla->cellAt( 0,3 ).firstCursorPosition().insertHtml( "<b> Pagado </b> " );
    tabla->cellAt( 0,4 ).firstCursorPosition().insertHtml( "<b> Subtotal </b>" );

    QTextBlockFormat bfizq = tabla->cellAt( 0, 3 ).firstCursorPosition().blockFormat();
    bfizq.setAlignment( Qt::AlignRight );
    // Ingreso los datos
    double subtotal = DSBImporte->value();
    double pagado = DSBEntrega->value();
    // Importe
    tabla->cellAt( 1, 0 ).firstCursorPosition().insertHtml( " " );
    tabla->cellAt( 1, 1 ).firstCursorPosition().insertHtml( "Importe a pagar en cuotas" );
    tabla->cellAt( 1, 2 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 1, 2 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );
    tabla->cellAt( 1, 3 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 1, 3 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( 0.0, 10, 'f', 2 ) );
    tabla->cellAt( 1, 4 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 1, 4 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );
    subtotal -= DSBEntrega->value();
    tabla->cellAt( 2, 0 ).firstCursorPosition().insertHtml( "" );
    tabla->cellAt( 2, 1 ).firstCursorPosition().insertHtml( "Entrega inicial" );
    tabla->cellAt( 2, 2 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 2, 2 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( DSBEntrega->value(), 10, 'f', 2 ) );
    tabla->cellAt( 2, 3 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 2, 3 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( pagado, 10, 'f', 2 ) );
    tabla->cellAt( 2, 4 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 2, 4 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );
    subtotal *= ( 1 + DSBInteres->value() / 100 );
    double valor_cuota = ( ( DSBTotal->value() ) * ( 1 + DSBInteres->value() / 100 ) ) / SBCantidad->value();
    QDate fch = DEInicio->date();
    for( int i = 1; i<=SBCantidad->value(); i++ ) {
        tabla->cellAt( i+2, 0 ).firstCursorPosition().insertHtml( QString( "#%1" ).arg( i ) );
        tabla->cellAt( i+2, 1 ).firstCursorPosition().insertHtml( QString( "%1" ).arg( fch.toString( Qt::SystemLocaleShortDate ) ) );
        fch.addDays( (i-1)*MPlanCuota::diasEnPeriodo( (MPlanCuota::Periodo) CBPeriodo->currentIndex(), fch ) );
        tabla->cellAt( i+2, 2 ).firstCursorPosition().setBlockFormat( bfizq );
        tabla->cellAt( i+2, 2 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( valor_cuota, 10, 'f', 2 ) );
        pagado += valor_cuota;
        tabla->cellAt( i+2, 3 ).firstCursorPosition().setBlockFormat( bfizq );
        tabla->cellAt( i+2, 3 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( pagado, 10, 'f', 2 ) );
        subtotal -= valor_cuota;
        tabla->cellAt( i+2, 4 ).firstCursorPosition().setBlockFormat( bfizq );
        tabla->cellAt( i+2, 4 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );
    }

    // Firma y aclaracion
    cursor.movePosition( QTextCursor::End );
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertText( "Firma del contrayente: ___________________________" );
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertText( QString::fromUtf8( "Aclaracion: ________________________________________________      DNI:___-__________-___" ) );
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertHtml( QString::fromUtf8( "<small>En caso de provocarse un atraso en la fecha de pago de cualquiera de las cuotas, se aplicara el recargo correspondiente tal cual se hace actualmenete con cualquier recibo emitido por nuestra entidad.</small>" ) );

    // Cabecera
    cursor.movePosition( QTextCursor::Start );
    cursor.insertBlock();
#ifdef Q_OS_WIN
    cursor.insertHtml( "<h1>HiComp Computación</h1><br />" );
#else
    cursor.insertHtml( "<h1>" + ERegistroPlugins::getInstancia()->pluginInfo()->empresa() + "</h1><br />" );
#endif
    cursor.insertHtml( "<h2>Plan de cuotas</h2><br /><br />" );
    cursor.insertBlock();
    cursor.insertHtml( QString( "<b>Fecha de Inicio:</b> %1 <br />" ).arg( DEInicio->date().toString( Qt::SystemLocaleLongDate ) ) );
    cursor.insertHtml( QString( "<b>Nombre del cliente:</b> %1 <br />").arg( MClientes::getRazonSocial( _id_cliente ) ) );
    return;
}

/*!
 * \fn FormSimularCuotas::pdf()
 * Genera un pdf con las cuotas tal cual se ingresaron en los datos.
 */
void FormSimularCuotas::pdf()
{
    this->generaReporte();
    QPrinter *imp = new QPrinter( QPrinterInfo::defaultPrinter() );
    imp->setOutputFormat( QPrinter::PdfFormat );
    QString nombreArchivo = QFileDialog::getSaveFileName( this, "Guardar como pdf", QDir::homePath(), "Archivos PDF (*.pdf)" );
    if( !nombreArchivo.isEmpty() )
    {
     // ver que tenga la extension
     imp->setOutputFileName( nombreArchivo );
     documento->print( imp );
    }
    delete imp;
}

/*!
 * \fn FormSimularCuotas::confirmar()
 *
 */
void FormSimularCuotas::confirmar()
{
    if( _id_cliente <= 0 ) {
        QMessageBox::information( this, "Error", "No se puede generar un plan de cuotas para un cliente Consumidor final o desconocido" );
        return;
    }
    // Consulto si quiere imprimir el plan de cuotas
    int ret = QMessageBox::question( this, QString::fromUtf8("¿Imprimir?"), QString::fromUtf8("¿Desea imprimir el resumen de cuotas para que el cliente lo firme?"), QMessageBox::Yes, QMessageBox::No );
    if( ret == QMessageBox::Yes ) {
        imprimir();
    }
    if( CkBEntregaEfectivo->isChecked() ) {
        QMessageBox::information( this, QString::fromUtf8("Impimir recibo"), QString::fromUtf8("A continuación se imprimirá el recibo correspondiente a la entrega inicial. Inserte el papel correspondiente y presiones Ok" ) );
    }
    // Genero el plan de cuotas
    int id_plan_cuota = -1;
    MPlanCuota *mpc = new MPlanCuota( this );
    if( ! mpc->agregarPlanCuota( _id_cliente,
                                 DSBTotal->value(),
                                 DSBInteres->value(),
                                 (MPlanCuota::Periodo) CBPeriodo->itemData( CBPeriodo->currentIndex() ).toInt(),
                                 DSBEntrega->value(),
                                 DEInicio->date(),
                                 SBCantidad->value(),
                                 &id_plan_cuota,
                                 CkBEntregaEfectivo->isChecked(),
                                 _tipo_comprobante ) ) {
        QMessageBox::information( this, "Error", "No se pudo ingresar el plan de cuotas" );
        return;
    }
    emit emitirIdPlanCuota( id_plan_cuota );
    this->close();
    return;
}

/*!
 * \fn FormSimularCuotas::cambioEntrega( double cant )
 * Setea el monto ingresado como entrega inicial.
 */
void FormSimularCuotas::cambioEntrega( double )
{
    DSBTotal->setValue( DSBImporte->value() - DSBEntrega->value() );
    modelo->setImporte( DSBTotal->value() );
    modelo->regenerar();
}

/*!
 * \fn FormSimularCuotas::cambioImporte( double cantidad )
 * Setea el monto de importe a pagar.
 */
void FormSimularCuotas::cambioImporte( double )
{
    DSBTotal->setValue( DSBImporte->value() - DSBEntrega->value() );
    modelo->setImporte( DSBTotal->value() );
    modelo->regenerar();
    TVSimulacion->update();
}

/*!
 * \fn FormSimularCuotas::cambioInteres( double cantidad )
 * Setea el interes a aplicar.
 */
void FormSimularCuotas::cambioInteres( double cantidad )
{
    modelo->setInteres( cantidad );
    modelo->regenerar();
    TVSimulacion->update();
}

/*!
 * \fn FormSimularCuotas::cambioCantidad( int cantidad )
 * Setea la cantidad de cuotas.
 */
void FormSimularCuotas::cambioCantidad( int cantidad )
{
    modelo->setCuotas( cantidad );
    modelo->regenerar();
    TVSimulacion->update();
}

/*!
 * \fn FormSimularCuotas::cambioPeriodo( int idx )
 * Llamado para cambiar el periodo de las cuotas.
 */
void FormSimularCuotas::cambioPeriodo( int idx )
{
    modelo->setPeriodo( (MPlanCuota::Periodo) idx );
    modelo->regenerar();
    TVSimulacion->update();
}

/*!
 * \fn FormSimularCoutas::cambioFechaInicio( QDate fecha )
 * Cambia la fecha de inicio.
 */
void FormSimularCuotas::cambioFechaInicio( QDate fecha )
{
    modelo->setFechaInicio( fecha );
    modelo->regenerar();
}
