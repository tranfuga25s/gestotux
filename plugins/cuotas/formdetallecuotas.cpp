#include "formdetallecuotas.h"

#include "mplancuota.h"
#include "eactcerrar.h"
#include "preferencias.h"
#include "eregistroplugins.h"
#include "einfoprogramainterface.h"
#include "../ventas/MFactura.h"
#include "../remitos/MRemito.h"
#include "NumeroComprobante.h"
#include "mitemplancuota.h"

#include <QTextTable>
#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

FormDetalleCuotas::FormDetalleCuotas(QWidget *parent) :
EVentana(parent), Ui::FormDetalleCuotasBase()
{
    setupUi(this);
    setWindowTitle( "Detalle de cuotas" );

    ActImprimir = new QAction( this );
    ActImprimir->setText( QString::fromUtf8( "Imprimir" ) );
    ActImprimir->setStatusTip( "Imprime la información detallada del estado de cuotas del plan" );
    ActImprimir->setIcon( QIcon( ":/imagenes/impresora.png" ) );
    connect( ActImprimir, SIGNAL( triggered() ), this, SLOT( imprimir() ) );

    ActPdf = new QAction( this );
    ActPdf->setText( "PDF" );
    ActPdf->setStatusTip( "Convierte a PDF la información actual" );
    ActPdf->setIcon( QIcon( ":/imagenes/acroread.png" ) );
    connect( ActPdf, SIGNAL( triggered() ), this, SLOT( pdf() ) );

    modelo = new MPlanCuota( this );

    CBPeriodo->insertItem( MPlanCuota::Semanal      , "Semanal"       );
    CBPeriodo->insertItem( MPlanCuota::Quincenal    , "Quincenal"     );
    CBPeriodo->insertItem( MPlanCuota::Mensual      , "Mensual"       );
    CBPeriodo->insertItem( MPlanCuota::BiMensual    , "Bimensual"     );
    CBPeriodo->insertItem( MPlanCuota::Trimestral   , "Trimestral"    );
    CBPeriodo->insertItem( MPlanCuota::Cuatrimestral, "Cuatrimestral" );
    CBPeriodo->insertItem( MPlanCuota::Seximestral  , "Semestral"     );
    CBPeriodo->insertItem( MPlanCuota::Anual        , "Anual"         );

    addAction( ActImprimir );
    addAction( ActPdf );
    addAction( new EActCerrar( this ) );

    DSBImporte->setReadOnly( true );
    DSBEntrega->setReadOnly( true );
    DSBInteres->setReadOnly( true );
    DSBTotal->setReadOnly( true );
    SBCantidad->setReadOnly( true );
    CBPeriodo->setEditable( false );
    DEInicio->setReadOnly( true );
    CBCliente->setEditable( false );

    GBAdelanto->setVisible( false );
}

FormDetalleCuotas::~FormDetalleCuotas()
{
    if( !documento )
        delete documento;

}

void FormDetalleCuotas::changeEvent(QEvent *e)
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
 * \fn FormDetalleCuotas::imprimir()
 * Imprime el listado de cuotas tal cual se ingresaron en los datos.
 */
void FormDetalleCuotas::imprimir()
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


void FormDetalleCuotas::generaReporte()
{
    documento = new QTextDocument();
    QTextCursor cursor( documento );
    int cant_filas = 3 + SBCantidad->value();
    QTextTable *tabla = cursor.insertTable( cant_filas, 4 );
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
    tabla->cellAt( 0,2 ).firstCursorPosition().insertHtml( "<b> Importe </b>" );
    tabla->cellAt( 0,3 ).firstCursorPosition().insertHtml( "<b> Pagado </b>" );
    tabla->cellAt( 0,4 ).firstCursorPosition().insertHtml( "<b> Subtotal </b>" );

    QTextBlockFormat bfizq = tabla->cellAt( 0, 3 ).firstCursorPosition().blockFormat();
    bfizq.setAlignment( Qt::AlignRight );
    // Ingreso los datos
    double subtotal = DSBTotal->value() * (-1);
    // Importe
    tabla->cellAt( 1, 0 ).firstCursorPosition().insertHtml( " " );
    tabla->cellAt( 1, 1 ).firstCursorPosition().insertHtml( "Importe a pagar en cuotas" );
    tabla->cellAt( 1, 2 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 1, 2 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal*(-1), 10, 'f', 2 ) );
    tabla->cellAt( 1, 3 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 1, 3 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );
    subtotal += DSBEntrega->value();
    tabla->cellAt( 2, 0 ).firstCursorPosition().insertHtml( "" );
    tabla->cellAt( 2, 1 ).firstCursorPosition().insertHtml( "Entrega inicial" );
    tabla->cellAt( 2, 2 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 2, 2 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( DSBEntrega->value(), 10, 'f', 2 ) );
    tabla->cellAt( 2, 3 ).firstCursorPosition().setBlockFormat( bfizq );
    tabla->cellAt( 2, 3 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );

    for( int i = 1; i<=modelo_item->rowCount(); i++ ) {
        tabla->cellAt( i+2, 0 ).firstCursorPosition().insertHtml( QString( "%1" ).arg( i ) );
        tabla->cellAt( i+2, 1 ).firstCursorPosition().insertHtml(
                    QString( "%1" ).arg( modelo_item->data( modelo_item->index( i, modelo_item->fieldIndex( "fecha_vencimiento" ) ), Qt::EditRole ).toDate().toString( Qt::SystemLocaleShortDate ) )
        );
        tabla->cellAt( i+2, 2 ).firstCursorPosition().setBlockFormat( bfizq );
        double valor_cuota = modelo_item->data( modelo_item->index( i, modelo_item->fieldIndex( "monto" ) ), Qt::EditRole ).toDouble();
        tabla->cellAt( i+2, 2 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( valor_cuota, 10, 'f', 2 ) );
        subtotal += valor_cuota;
        tabla->cellAt( i+2, 3 ).firstCursorPosition().setBlockFormat( bfizq );
        QDate fecha_pago = modelo_item->data( modelo_item->index( i, modelo_item->fieldIndex( "fecha_pago" ) ), Qt::EditRole ).toDate();
        if( fecha_pago.isValid() ) {
            subtotal -= valor_cuota;
            tabla->cellAt( i+2, 3 ).firstCursorPosition().insertHtml( fecha_pago.toString( Qt::LocaleDate ) );
        } else {
            tabla->cellAt( i+2, 3 ).firstCursorPosition().insertHtml( " " );
        }
        tabla->cellAt( i+2, 4 ).firstCursorPosition().setBlockFormat( bfizq );
        tabla->cellAt( i+2, 4 ).firstCursorPosition().insertHtml( QString( "$ %L1" ).arg( subtotal, 10, 'f', 2 ) );
    }

    // Firma y aclaracion
    cursor.movePosition( QTextCursor::End );
    cursor.insertBlock();
    cursor.insertBlock();
    cursor.insertHtml( QString::fromUtf8( "<small>En caso de provocarse un atraso en la fecha de pago de cualquiera de las cuotas, se aplicará el recargo correspondiente tal cual se hace actualmenete con cualquier recibo emitido por nuestra entidad.</small>" ) );

    // Cabecera
    cursor.movePosition( QTextCursor::Start );
    cursor.insertBlock();
    cursor.insertHtml( "<h1>" + ERegistroPlugins::getInstancia()->pluginInfo()->empresa() + "</h1><br />" );
    cursor.insertHtml( "<h2>Plan de cuotas</h2><br /><br />" );
    cursor.insertBlock();
    cursor.insertHtml( QString( "<b>Fecha de Inicio:</b> %1 <br />" ).arg( DEInicio->date().toString( Qt::SystemLocaleLongDate ) ) );
    cursor.insertHtml( QString( "<b>Nombre del cliente:</b> %1 <br />").arg( "Desconocido" ) );
    return;
}

/*!
 * \fn FormDetalleCuotas::pdf()
 * Genera un pdf con las cuotas tal cual se ingresaron en los datos.
 */
void FormDetalleCuotas::pdf()
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
 * \brief FormDetalleCuotas::setearIdPlanCuota
 * Setea el id de cuota correspondiente.
 * \param id Identificador del plan de cuotas
 */
void FormDetalleCuotas::setearIdPlanCuota(int id , bool inicializar_submodelo ) {
    // Cargo los datos de la cuota
    QSqlQuery cola;
    if( cola.exec( QString( "SELECT * FROM plan_cuota WHERE id_plan_cuota = %1" ).arg( id ) ) ) {
        if( cola.next() ) {
            SBCantidad->setValue(       cola.record().value("cantidad_cuotas").toInt()    );
            CBPeriodo ->setCurrentIndex(cola.record().value("tipo_periodo"   ).toInt()    );
            DEInicio  ->setDate(        cola.record().value("fecha_inicio"   ).toDate()   );
            DSBEntrega->setValue(       cola.record().value("entrega_inicial").toDouble() );
            DSBInteres->setValue(       cola.record().value("recargo"        ).toDouble() );
            DSBImporte->setValue(       cola.record().value("total"          ).toDouble() );
            DSBTotal  ->setValue( DSBImporte->value() - DSBEntrega->value() );
            MPlanCuota::TipoComprobante tipo_comprobante = (MPlanCuota::TipoComprobante)cola.record().value("tipo_comprobante").toInt();
            if( tipo_comprobante == MPlanCuota::Factura ) {
                int id_factura = cola.record().value("id_factura").toInt();
                if( cola.exec( QString( "SELECT serie, numero, id_cliente FROM factura WHERE id_factura = %1").arg( id_factura ) ) ){
                    if( cola.next() ) {
                        LFactura->setText( "Numero de factura: #"+MFactura::obtenerComprobante( id_factura ).aCadena() );
                        CBCliente->setearId( cola.record().value("id_cliente").toInt() );
                    } else {
                        qWarning() << "No se pudo cargar los datos de la factura - seguramente no existe";
                    }
                } else {
                    qWarning() << "Error al intentar cargar los datos de la factura asociada al plan de cuotas";
                    qDebug() << "Error de exec de la cola de obtención de datos de la factura de una cuota";
                    qDebug() << cola.lastError().text();
                    qDebug() << cola.lastQuery();
                    return;
                }
            } else if( tipo_comprobante == MPlanCuota::Remito ) {
                int id_factura = cola.record().value("id_factura").toInt();
                if( cola.exec( QString( "SELECT serie, numero, id_cliente FROM remito WHERE id_remito = %1").arg( id_factura ) ) ){
                    if( cola.next() ) {
                        LFactura->setText( "Numero de remito: #"+MRemito::obtenerComprobante( id_factura ).aCadena() );
                        CBCliente->setearId( cola.record().value("id_cliente").toInt() );
                    } else {
                        qWarning() << "No se pudo cargar los datos del remito - seguramente no existe";
                    }
                } else {
                    qWarning() << "Error al intentar cargar los datos de la factura asociada al plan de cuotas";
                    qDebug() << "Error de exec de la cola de obtención de datos del remito de una cuota";
                    qDebug() << cola.lastError().text();
                    qDebug() << cola.lastQuery();
                    return;
                }
            }
            // Cargo los datos de los pagos y no pagos
            if( inicializar_submodelo ) {
                modelo_item = new MItemPlanCuota( this );
                modelo_item->setearPlanCuota( id );
                modelo_item->sort( modelo_item->fieldIndex("fecha_vencimiento" ), Qt::DescendingOrder );
                TVSimulacion->setModel( modelo_item );
                TVSimulacion->setAlternatingRowColors( true );
                TVSimulacion->setSortingEnabled( true );
                TVSimulacion->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
                modelo_item->select();
                TVSimulacion->hideColumn( 0 );
                TVSimulacion->hideColumn( 1 );
                TVSimulacion->hideColumn( 2 );
            }
        } else {
            qWarning() << "Error de seleccion de datos - Seguramente el plan de cuota no existe";
            qDebug() << "Error de next al ejecutar la cola de carga de datos de un plan de cuotas";
            qDebug() << cola.lastQuery();
        }
    } else {
        qWarning() << "Existió un error al cargar los datos de la cuota";
        qDebug() << "Error al ejecutar la cola de carga de datos de un plan de cuotas";
        qDebug() << cola.lastError().text();
        qDebug() << cola.lastQuery();
    }
    return;
}
