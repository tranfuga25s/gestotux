#include "formremito.h"

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
}

/*!
 *
 */
void FormRemito::setearValor( const int id_remito )
{
    qWarning( "No implementado!" );
    return;
}
