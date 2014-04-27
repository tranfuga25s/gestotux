#include "formresumencomprastotales.h"

#include "eactcerrar.h"

#include <QInputDialog>

FormResumenComprasTotales::FormResumenComprasTotales(QWidget *parent) :
EVentana(parent)
{
    setupUi(this);
    this->setObjectName( "resumen_compras_totales" );
    this->setWindowTitle( "Total de compras" );

    _metodo_temporal = ResumenComprasTotales::PorMes;

    ActCambiarDivisionTemporal = new QAction( this );
    ActCambiarDivisionTemporal->setText( QString::fromUtf8( "Período" ) );
    ActCambiarDivisionTemporal->setStatusTip( "Cambia el periodo en el cual se calculan los totales" );
    ActCambiarDivisionTemporal->setIcon( QIcon( ":/imagenes/periodo.png" ) );
    connect( ActCambiarDivisionTemporal, SIGNAL( triggered() ), this, SLOT( cambiarDivisionTemporal() ) );

    _modelo = new ResumenComprasTotales( this );
    TVCompras->setModel( _modelo );
    _modelo->select();
    TVCompras->hideColumn( _modelo->fieldIndex( "id_proveedor" ) );
    TVCompras->hideColumn( _modelo->fieldIndex( "contado" ) );
    TVCompras->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    TVCompras->setSortingEnabled( true );

    this->addAction( ActCambiarDivisionTemporal );
    this->addAction( new EActCerrar( this ) );
}

/*!
 * \brief FormResumenComprasTotales::cambiarDivisionTemporal
 */
void FormResumenComprasTotales::cambiarDivisionTemporal()
{
    QInputDialog dialogo;
    QStringList valores;
    valores << "Completo" << "Mensual" << "Bimestral" << "Cuatrimestral" << "Semestral" << "Anual";

    QObject::connect( &dialogo, SIGNAL( textValueChanged( const QString & ) ),
                      this    , SLOT  ( cambioDivisionTemporal( const QString & ) ) );
    dialogo.setOptions( QInputDialog::UseListViewForComboBoxItems );
    dialogo.setComboBoxItems(valores);
    dialogo.setLabelText( QString::fromUtf8( "Seleccione un período" ) );
    dialogo.setOkButtonText( QString::fromUtf8( "Elegir" ) );
    dialogo.setCancelButtonText( QString::fromUtf8( "Cancelar" ) );
    dialogo.exec();
}

/*!
 * \brief FormResumenComprasTotales::changeEvent
 * \param e
 */
void FormResumenComprasTotales::changeEvent(QEvent *e)
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
 * \brief FormResumenComprasTotales::cambioDivisionTemporal
 * \param texto
 */
void FormResumenComprasTotales::cambioDivisionTemporal( const QString &texto )
{
    if( texto == "Completo" ) {
        _metodo_temporal = ResumenComprasTotales::SinDivision;
    } else if( texto == "Mensual" ) {
        _metodo_temporal = ResumenComprasTotales::PorMes;
    } else if( texto == "Bimestral" ) {
        _metodo_temporal = ResumenComprasTotales::PorBimestre;
    } else if( texto == "Cuatrimestral" ) {
        _metodo_temporal = ResumenComprasTotales::PorCuatrimestre;
    } else if( texto == "Semestral" ) {
        _metodo_temporal = ResumenComprasTotales::PorSeximestre;
    } else if( texto == "Anual" ) {
        _metodo_temporal = ResumenComprasTotales::PorAno;
    } else {
        _metodo_temporal = ResumenComprasTotales::PorMes;
    }
    _modelo->setearMetodoTemporal( (ResumenComprasTotales::DivisionTemporal) this->_metodo_temporal );
}
