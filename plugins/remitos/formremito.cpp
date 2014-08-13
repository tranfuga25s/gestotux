#include "formremito.h"

#include "MRemito.h"
#include "mitemremito.h"
#include "NumeroComprobante.h"
#include "mproductostotales.h"
#include "mclientes.h"

#include <QDebug>
#include <QSqlField>
#include <QSqlRecord>


FormRemito::FormRemito(QWidget *parent) :
EVentana(parent), FormAgregarRemitoBase()
{
    setupUi( this );

    setWindowTitle( "Ver remito" );
    setWindowIcon( QIcon( ":/imagenes/remito.png" ) );
    setObjectName( "ver_remito" );

    DEFecha->setVisible( false );
    CBCliente->setVisible( false );
    LEDireccion->setVisible( false );

    PBAgregarProducto->setVisible( false );
    PBEliminar->setVisible( false );
    PBEliminarTodo->setVisible( false );
    PBAgregarDescuento->setVisible( false );
    PBEliminarDescuento->setVisible( false );
    DSBCant->setVisible( false );
    CBProducto->setVisible( false );

    PTEObservaciones->setReadOnly( true );
    PTEObservaciones->setPlainText( "Observaciones: " );

    mpt = new MProductosTotales( TVProductos );
    mpt->calcularTotales( true );
    TVProductos->setModel( mpt );
    TVProductos->setAlternatingRowColors( true );
    TVProductos->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVProductos->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    TVProductos->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    TVProductos->horizontalHeader()->setMinimumSectionSize( 140 );
    TVProductos->setSortingEnabled( false );

}

/*!
 * \fn FormRemito::setearValor()
 * Setear los valores del remito
 * \param id_remito Identificador del remito
 */
void FormRemito::setearValor( const int id_remito )
{

    MRemito *mr = new MRemito();
    QSqlRecord r = mr->obtenerDatos( id_remito );
    delete mr;
    mr = 0;

    LFecha->setText( QString( "<b>Fecha:</b> %1" ).arg( r.value( "fecha" ).toDate().toString( Qt::SystemLocaleShortDate ) ) );
    LCliente->setText( QString( "<b>Cliente:</b> %1" ).arg( r.value( "razon_social" ).toString() ) );
    LDireccion->setText( QString::fromUtf8( "<b>Dirección:</b> %1" ).arg( r.value("direccion").toString() ) );
    PTEObservaciones->setPlainText( r.value("observaciones").toString().prepend("Observaciones: ") );
    LComprobante->setText( NumeroComprobante( LComprobante,
                                              r.value("serie").toInt(),
                                              r.value("numero").toInt() ).aCadena() );


    switch( r.value("id_forma_pago").toInt() ) {
        case MRemito::Contado: {
            RBContado->setChecked( true );
            RBOtro->setEnabled( false );
            RBCtaCte->setEnabled( false );
            RBCuotas->setEnabled( false );
            break;
        }
        case MRemito::CuentaCorriente: {
            RBCtaCte->setChecked( true );
            RBContado->setEnabled( false );
            RBOtro->setEnabled( false );
            RBCuotas->setEnabled( false );
            break;
        }
        case MRemito::Cuotas: {
            RBCuotas->setChecked( true );
            RBContado->setEnabled( false );
            RBOtro->setEnabled( false );
            RBCtaCte->setEnabled( false );
            break;
        }
        case MRemito::Otro: {
            RBOtro->setChecked( true );
            RBContado->setEnabled( false );
            RBCtaCte->setEnabled( false );
            RBCuotas->setEnabled( false );
            break;
        }
        default: {
            RBContado->setEnabled( false );
            RBOtro->setEnabled( false );
            RBCtaCte->setEnabled( false );
            RBCuotas->setEnabled( false );
            break;
        }
    }

    MItemRemito *ir = new MItemRemito();
    ir->cargarItems( id_remito );
    for( int i = 0; i < ir->rowCount(); i++ ) {
        // Cargo los datos al modelo
        mpt->agregarItem(
                    ir->data( ir->index( i, ir->fieldIndex( "cantidad" ) )       , Qt::EditRole ).toDouble(),
                    ir->data( ir->index( i, ir->fieldIndex( "item" ) )           , Qt::EditRole ).toString(),
                    ir->data( ir->index( i, ir->fieldIndex( "precio_unitario" ) ), Qt::EditRole ).toDouble()
        );
    }
    delete ir;
    ir = 0;

}
