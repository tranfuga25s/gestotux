#include "dhistorialordentrabajo.h"
#include "ui_dhistorialordentrabajo.h"

#include "mtipooperacionordentrabajo.h"

DHistorialOrdenTrabajo::DHistorialOrdenTrabajo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DHistorialOrdenTrabajoBase)
{
    ui->setupUi( this );

    ui->DTEFechaHora->setDateTime( QDateTime::currentDateTime() );

    connect( ui->CBTipo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioTipo( int ) ) );
}

DHistorialOrdenTrabajo::~DHistorialOrdenTrabajo()
{
    delete ui;
}

/*!
 * \brief DHistorialOrdenTrabajo::setearComoCancelacion
 * Setea el dialogo para cancelar la orden de trabajo
 */
void DHistorialOrdenTrabajo::setearComoCancelacion()
{
    ui->CBTipo->setearId( MTipoOperacionOrdenTrabajo::Cancelacion );
}

/*!
 * \brief DHistorialOrdenTrabajo::setearComoFacturacion
 */
void DHistorialOrdenTrabajo::setearComoFacturacion()
{
    ui->CBTipo->setearId( MTipoOperacionOrdenTrabajo::Facturacion );
}

/*!
 * \brief DHistorialOrdenTrabajo::setearComoCierre
 */
void DHistorialOrdenTrabajo::setearComoCierre()
{
    ui->CBTipo->setearId( MTipoOperacionOrdenTrabajo::CierreOrden );
}

/*!
 * \brief DHistorialOrdenTrabajo::setearComoDevolverAlCliente
 */
void DHistorialOrdenTrabajo::setearComoDevolverAlCliente()
{
    ui->CBTipo->setearId( MTipoOperacionOrdenTrabajo::DevolucionCliente );
}

/*!
 * \brief DHistorialOrdenTrabajo::filtrarBloqueantes
 */
void DHistorialOrdenTrabajo::filtrarBloqueantes()
{
    QString filtro;
    filtro.append( QString( " WHERE id_tipo_operacion NOT IN( %1, %2, %3, %4 ) " )
                   .arg( MTipoOperacionOrdenTrabajo::Cancelacion       )
                   .arg( MTipoOperacionOrdenTrabajo::Facturacion       )
                   .arg( MTipoOperacionOrdenTrabajo::CierreOrden       )
                   .arg( MTipoOperacionOrdenTrabajo::DevolucionCliente ) );
    ui->CBTipo->setearFiltro( filtro, true );
}

/*!
 * \brief DHistorialOrdenTrabajo::accept
 */
void DHistorialOrdenTrabajo::accept()
{
    switch( ui->CBTipo->currentIndex() ) {
        case MTipoOperacionOrdenTrabajo::Cancelacion:
        {
            break;
        }
        default:
        { break; }
    }
    QDialog::accept();
}

/*!
 * \brief DHistorialOrdenTrabajo::cambioTipo
 * \param tipo
 */
void DHistorialOrdenTrabajo::cambioTipo( int tipo )
{
    switch( tipo ) {
        case MTipoOperacionOrdenTrabajo::Cancelacion:
        {
            ui->CBTipo->setEditable( false );
            ui->CBTipo->setEnabled( false );
            ui->CBTecnico->setVisible( false );
            ui->LTecnico->setVisible( false );
            setWindowTitle( "Cancelar Orden" );
            break;
        }
        case MTipoOperacionOrdenTrabajo::Facturacion:
        {
            ui->CBTipo->setEditable( false );
            ui->CBTipo->setEnabled( false );
            ui->CBTecnico->setVisible( false );
            ui->LTecnico->setVisible( false );
            setWindowTitle( "Facturar item" );
            break;
        }
        case MTipoOperacionOrdenTrabajo::CierreOrden:
        {
            ui->CBTipo->setEditable( false );
            ui->CBTipo->setEnabled( false );
            ui->CBTecnico->setVisible( false );
            ui->LTecnico->setVisible( false );
            setWindowTitle( "Cerrar orden" );
            break;
        }
        case MTipoOperacionOrdenTrabajo::DevolucionCliente:
        {
            ui->CBTipo->setEditable( false );
            ui->CBTipo->setEnabled( false );
            ui->CBTecnico->setVisible( false );
            ui->LTecnico->setVisible( false );
            setWindowTitle( "Devolver equipo al cliente" );
            break;
        }
        default:
        {
            setWindowTitle( ui->CBTipo->currentText() );
            break;
        }
    }
}
