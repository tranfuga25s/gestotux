#include "formmodificarproveedor.h"

#include "eactcerrar.h"
#include "eactguardar.h"

FormModificarProveedor::FormModificarProveedor( MProveedor *m, QWidget *parent) :
    EVentana(parent), Ui::FormProveedorBase()
{
    setupUi(this);
    setObjectName( "ModificarProveedor" );
    setWindowTitle( "Modificar Proveedor" );

    addAction( new EActGuardar( this ) );
    addAction( new EActCerrar( this ) );

    this->modelo = m;

    mapa = new QDataWidgetMapper( this );
    mapa->setModel( this->modelo );
    mapa->setOrientation( Qt::Horizontal );
    mapa->addMapping( LERazonSocial, modelo->fieldIndex( "nombre" ) );
    mapa->addMapping( TEDireccion, modelo->fieldIndex( "direccion" ), "plainText" );
    mapa->addMapping( LECUIT, modelo->fieldIndex( "cuit_cuil" ) );
    mapa->addMapping( LETelFijo, modelo->fieldIndex( "telefono_linea" ) );
    mapa->addMapping( LETelCel, modelo->fieldIndex( "telefono_celular" ) );
    mapa->addMapping( LEFax, modelo->fieldIndex( "fax" ) );
    mapa->addMapping( LEEmail, modelo->fieldIndex( "email" ) );
    mapa->setSubmitPolicy( QDataWidgetMapper::ManualSubmit );

    this->GBTitulo->setTitle( "Modificar proveedor" );
}

#include <QSqlError>
#include <QMessageBox>
bool FormModificarProveedor::guardar()
{
    if( LERazonSocial->text().isEmpty() ) {
        QMessageBox::information( this, "Error", "Por favor, ingrese una razon social para el proveedor" );
        return false;
    }
    // Verifico el Cuit
    if( !LECUIT->verificar() ) {
        QMessageBox::information( this, "Error", "Por favor, ingrese una razon CUIT/CUIL valido para el proveedor" );
        return false;
    }
    if( modelo->data( modelo->index( mapa->currentIndex(), modelo->fieldIndex( "nombre" ) ), Qt::EditRole ).toString() != LERazonSocial->text() ) {
        // Cambio el nombre del proveedor -> verifico
        if( modelo->existeProveedor( LERazonSocial->text() ) ) {
            qWarning( "El CUIT/CUIL ingresado ya existe en la base de datos en otro proveedor" );
            return false;
        }
    }
    if( modelo->data( modelo->index( mapa->currentIndex(), modelo->fieldIndex( "cuit_cuil" ) ), Qt::EditRole ).toString() != LECUIT->text() ) {
        // Cambio el cuit/cuil -> verifico
        if( modelo->existeCuitCuil( LECUIT->text() ) ) {
            qWarning( "El CUIT/CUIL ingresado ya existe en la base de datos en otro proveedor" );
            return false;
        }
    }
    if( mapa->submit() ) {
        QMessageBox::information( this, "Correcto", "Datos guardados correctamente" );
        this->close();
        return true;
    } else {
        qDebug( "Error al intentar guardar los datos" );
        qDebug( this->modelo->lastError().text().toLocal8Bit() );
        return false;
    }
}

void FormModificarProveedor::setearItem( const int id )
{
    if( id >= 0 )
    { this->mapa->setCurrentIndex( id ); }
}
