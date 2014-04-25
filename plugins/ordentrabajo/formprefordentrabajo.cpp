#include "formprefordentrabajo.h"

#include "preferencias.h"

FormPrefOrdenTrabajo::FormPrefOrdenTrabajo(QWidget *parent) :
QWidget( parent ), Ui::FormPrefOrdenTrabajo()
{
        setupUi( this );
        setAttribute( Qt::WA_DeleteOnClose );

        setWindowTitle( "Orden Trabajo" );
        setWindowIcon( QIcon( ":/imagenes/orden_trabajo.png" ) );
}

void FormPrefOrdenTrabajo::cargar()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "OrdenTrabajo" );
    SBDias->setValue( p->value("dias",2).toInt() );
    CBTecnico->setearId( p->value("id_tecnico",0).toInt() );
    SBMeses->setValue( p->value("meses_vencimiento", 2 ).toInt() );
    p->endGroup();
    p->endGroup();
    p=0;
}

void FormPrefOrdenTrabajo::guardar()
{
    preferencias *p = preferencias::getInstancia();
    p->inicio();
    p->beginGroup( "Preferencias" );
    p->beginGroup( "OrdenTrabajo" );
    p->setValue( "dias", SBDias->value() );
    p->setValue( "id_tecnico", CBTecnico->idActual() );
    p->setValue( "meses_vencimiento", SBMeses->value() );
    p->endGroup();
    p->endGroup();
    p=0;
}
