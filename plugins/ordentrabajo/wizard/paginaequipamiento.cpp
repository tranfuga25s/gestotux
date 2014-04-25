#include "paginaequipamiento.h"

#include "ordentrabajowizard.h"
#include "mequipamiento.h"

PaginaEquipamiento::PaginaEquipamiento(QWidget *parent) :
    QWizardPage(parent)
{
    setupUi(this);
    setTitle( "Equipamiento" );
    setSubTitle( QString::fromUtf8( "Por favor, seleccione que opción desea utilizar respecto al equipamiento" ) );

    registerField( "equipamiento.existe", RBExistente );
    registerField( "equipamiento.nuevo", RBNuevo );
}

int PaginaEquipamiento::nextId() const
{
    if( RBExistente->isChecked() ) {
        return OrdenTrabajoWizard::Pagina_EquipamientoExistente;
    } else {
        return OrdenTrabajoWizard::Pagina_EquipamientoNuevo;
    }
}

void PaginaEquipamiento::initializePage()
{
    // Busco si se paso por la info de nuevo cliente
    if( wizard()->hasVisitedPage( OrdenTrabajoWizard::Pagina_ClienteNuevo ) ) {
        RBExistente->setEnabled( false );
        RBNuevo->setChecked( true );
    } else {
        // Buscar si el cliente actual tiene algún equipamiento
        if( !MEquipamiento::existeEquipamientoParaCliente( wizard()->field( "cliente.id_cliente" ).toInt() ) ) {
            RBExistente->setEnabled( false );
            RBNuevo->setChecked( true );
        }
    }

}