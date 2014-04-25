#include "paginafinal.h"

#include "preferencias.h"

PaginaFinal::PaginaFinal(QWidget *parent) :
    QWizardPage(parent)
{
    setupUi(this);

    setTitle( "Final: Datos de la orden" );
    setSubTitle( QString::fromUtf8( "Por favor, ingrese los datos necesarios para la orden de trabajo." ) );

    registerField( "orden_trabajo.requerente"       , LERequerente  ); // Persona que ingresa el pedido si no es el cliente.
    registerField( "orden_trabajo.ingresante*"      , LEIngresante  ); // Ingresante de la orden ( empleado )
    registerField( "orden_trabajo.fecha_ingreso*"   , DEIngreso     ); // Fecha de ingreso de la orden
    registerField( "orden_trabajo.fecha_entrega"    , DTEDevolucion ); // Fecha posible de devolucion de la orden
    registerField( "orden_trabajo.causa_ingreso"    , PTEFallas     , "plainText" ); // Causa o falla del ingreso
    registerField( "orden_trabajo.id_tecnico"       , CBTecnico     , "idActual"  ); // Tecnico asignado
    registerField( "orden_trabajo.fecha_vencimiento", DEVencimiento ); // Fecha de vencimiento
}

int PaginaFinal::nextId() const
{ return -1; }

void PaginaFinal::initializePage()
{
    // El requerente de la orden debería de ser el cliente elegido.
    LERequerente->setText( wizard()->field( "cliente.texto_ingresado" ).toString() );
    DEIngreso->setDateTime( QDateTime::currentDateTime() );
    preferencias *p = preferencias::getInstancia();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "OrdenTrabajo" );
    DTEDevolucion->setDate( QDate::currentDate().addDays( p->value("dias",2 ).toInt() ) );
    CBTecnico->setearId( p->value( "id_tecnico", 1 ).toInt() );
    p->endGroup();
    p->endGroup();
    p=0;
}

