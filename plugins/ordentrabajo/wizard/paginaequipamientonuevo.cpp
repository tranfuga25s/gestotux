#include "paginaequipamientonuevo.h"

#include "ordentrabajowizard.h"

PaginaEquipamientoNuevo::PaginaEquipamientoNuevo(QWidget *parent) :
    QWizardPage(parent)
{
    setupUi( this );

    setTitle( QString::fromUtf8( "Nuevo equipamiento" ) );
    setSubTitle( QString::fromUtf8( "Por favor, ingrese los datos correspondientes al equipamiento a reparar." ) );

    registerField( "equipamiento.descripcion*", LEDescripcion );
    registerField( "equipamiento.modelo", LEModelo );
    registerField( "equipamiento.marca*", LEMarca );
    registerField( "equipamiento.observaciones", PTEObservaciones, "plainText" );
    registerField( "equipamiento.numero_serie", LENumeroSerie );
}

int PaginaEquipamientoNuevo::nextId() const
{
    return OrdenTrabajoWizard::Pagina_Final;
}

