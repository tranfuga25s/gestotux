#include "formremito.h"

#include "MRemito.h"
#include "mitemremito.h"
#include "NumeroComprobante.h"
#include "mproductostotales.h"

#include <QSqlField>
#include <QSqlRecord>


FormRemito::FormRemito(QWidget *parent) :
EVentana(parent), FormAgregarRemitoBase()
{
    setupUi( this );

    DEFecha->setReadOnly( true );
    CBCliente->setEditable( false );
    LEDireccion->setReadOnly( true );

    PBAgregarProducto->setVisible( false );
    PBEliminar->setVisible( false );
    PBEliminarTodo->setVisible( false );
    PBAgregarDescuento->setVisible( false );
    PBEliminarDescuento->setVisible( false );

    GBFormaPago->setEnabled( false );

    PTEObservaciones->setReadOnly( true );

    mpt = new MProductosTotales( TVProductos );
    TVProductos->setModel( mpt );
    TVProductos->setAlternatingRowColors( true );
    TVProductos->setSelectionBehavior( QAbstractItemView::SelectRows );
    TVProductos->horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    TVProductos->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    TVProductos->horizontalHeader()->setMinimumSectionSize( 140 );
    TVProductos->setSortingEnabled( false );
}

/*!
 *
 */
void FormRemito::setearValor( const int id_remito )
{

    MRemito *mr = new MRemito();
    QSqlRecord r = mr->obtenerDatos( id_remito );
    delete mr;
    mr = 0;

    DEFecha->setDate( r.field( "fecha" ).value().toDate() );
    CBCliente->setearId( r.field("id_cliente").value().toInt() );
    LEDireccion->setText( r.field("direccion").value().toString() );
    PTEObservaciones->setPlainText( r.field("observaciones").value().toString() );
    LNumeroComprobante->setText( NumeroComprobante( LNumeroComprobante,
                                                    r.field("serie").value().toInt(),
                                                    r.field("numero").value().toInt() ).aCadena() );


    switch( r.field("forma_pago").value().toInt() ) {
        case MRemito::Contado: {
            RBContado->setChecked( true );
            break;
        }
        case MRemito::CuentaCorriente: {
            RBCtaCte->setChecked( true );
            break;
        }
        case MRemito::Cuotas: {
            RBCuotas->setChecked( true );
            break;
        }
        case MRemito::Otro: {
            RBOtro->setChecked( true );
            break;
        }
        default: {
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
