#include "formresumencomprasporproductos.h"

#include <QInputDialog>

#include "eactcerrar.h"

FormResumenComprasPorProductos::FormResumenComprasPorProductos(QWidget *parent) :
    EVentana( parent )
{
    setupUi(this);
    this->setWindowTitle( "Compras por producto" );
    //this->setWindowIcon();
    this->setObjectName( "resumen_compras_productos" );

    _metodo_temporal = PorMes;

    ActCambiarDivisionTemporal = new QAction( this );
    ActCambiarDivisionTemporal->setText( QString::fromUtf8( "Período" ) );
    ActCambiarDivisionTemporal->setStatusTip( "Cambia el periodo en el cual se calculan los totales" );
    ActCambiarDivisionTemporal->setIcon( QIcon( ":/imagenes/periodo.png" ) );
    connect( ActCambiarDivisionTemporal, SIGNAL( triggered() ), this, SLOT( cambiarDivisionTemporal() ) );

    _modelo = new ResumenComprasPorProductos( this );
    TVLista->setModel( _modelo );
    TVLista->horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    TVLista->setSortingEnabled( true );
    TVLista->hideColumn( 0 );

    //this->addAction( ActCambiarDivisionTemporal );
    this->addAction( new EActCerrar( this ) );

    CBProductos->setCurrentIndex( -1 );
    connect( CBProductos, SIGNAL( currentIndexChanged( int ) ), this, SLOT( cambioProducto( int ) ) );

}

void FormResumenComprasPorProductos::changeEvent(QEvent *e)
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
 * \brief FormResumenComprasTotales::cambiarDivisionTemporal
 */
void FormResumenComprasPorProductos::cambiarDivisionTemporal()
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
 * \brief FormResumenComprasPorProductos::cambioProducto
 * \param id_producto
 */
void FormResumenComprasPorProductos::cambioProducto( int )
{
    _modelo->setearIdProducto( CBProductos->idActual() );
}

/*!
 * \brief FormResumenComprasTotales::cambioDivisionTemporal
 * \param texto
 */
void FormResumenComprasPorProductos::cambioDivisionTemporal( const QString &texto )
{
    if( texto == "Completo" ) {
        _metodo_temporal = SinDivision;
    } else if( texto == "Mensual" ) {
        _metodo_temporal = PorMes;
    } else if( texto == "Bimestral" ) {
        _metodo_temporal = PorBimestre;
    } else if( texto == "Cuatrimestral" ) {
        _metodo_temporal = PorCuatrimestre;
    } else if( texto == "Semestral" ) {
        _metodo_temporal = PorSeximestre;
    } else if( texto == "Anual" ) {
        _metodo_temporal = PorAno;
    } else {
        _metodo_temporal = PorMes;
    }
    _modelo->setearMetodoTemporal( (DivisionTemporal) this->_metodo_temporal );
}