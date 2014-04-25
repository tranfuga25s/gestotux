#include "paginaclientenuevo.h"

#include "ordentrabajowizard.h"
#include "mestadofiscal.h"
#include "preferencias.h"
#include "paginacliente.h"

PaginaClienteNuevo::PaginaClienteNuevo( QWidget *parent ) :
QWizardPage( parent )
{
    setupUi(this);

    setTitle( "Nuevo cliente" );
    setSubTitle( QString::fromUtf8( "Por favor, ingrese los datos del nuevo cliente. Será dado de alta al completar los pasos." ) );

    connect( LENombre, SIGNAL( textChanged( QString ) ), this, SLOT( rehacerRazonSocial( QString ) ) );
    connect( LEApellido, SIGNAL( textChanged( QString ) ), this, SLOT( rehacerRazonSocial( QString ) ) );

    registerField( "cliente.razonsocial*", LERazonSocial );
    registerField( "cliente.nombre", LENombre );
    registerField( "cliente.apellido", LEApellido );
    registerField( "cliente.direccion*", LEDireccion );
    registerField( "cliente.inscripcion", CBInscripcion );
    registerField( "cliente.telefono", LETelefono );

    CBInscripcion->setModel( new MEstadoFiscal( CBInscripcion ) );
}

void PaginaClienteNuevo::initializePage() {
    // Cargo los datos del combo box de la pagina anterior
    LERazonSocial->setText( wizard()->field("cliente.texto_ingresado" ).toString() );

    // Cargo los datos predeterminados
    preferencias *p = preferencias::getInstancia();
    p->inicio(); p->beginGroup( "Preferencias" ); p->beginGroup( "Clientes" );
    CBInscripcion->setCurrentIndex( p->value( "estado-fiscal" ).toInt() );
    p->endGroup(); p->endGroup(); p=0;
}

int PaginaClienteNuevo::nextId() const
{
    return OrdenTrabajoWizard::Pagina_Equipamiento;
}

void PaginaClienteNuevo::rehacerRazonSocial( QString )
{
    LERazonSocial->setText( LEApellido->text() + ", " + LENombre->text() );
}
